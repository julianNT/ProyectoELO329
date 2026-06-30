#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_selectedSensor(0), m_tick(0),
    m_min(1e9), m_max(-1e9), m_sum(0), m_count(0)
{
    ui->setupUi(this);
    setWindowTitle("HMI Sensores");
    resize(1100, 650);

    m_simulator = new DataSimulator(this);
    m_serial    = new SerialSource(this);
    m_source    = m_simulator;

    setupUI();
    setupChart();

    connect(m_source, &IDataSource::dataUpdated, this, &MainWindow::onDataUpdated);
    m_source->start();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupUI() {
    // Widget central
    QWidget* central = new QWidget(this);
    central->setStyleSheet("background-color: #12121f;");
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Panel izquierdo ──────────────────────────────
    QWidget* leftPanel = new QWidget();
    leftPanel->setFixedWidth(180);
    leftPanel->setStyleSheet("background-color: #0f0f1a; border-right: 1px solid #2a2a3a;");

    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(10, 15, 10, 15);
    leftLayout->setSpacing(8);

    QLabel* titleLabel = new QLabel("SENSORES");
    titleLabel->setStyleSheet("color: #555577; font-size: 10px; font-weight: bold; letter-spacing: 2px;");
    leftLayout->addWidget(titleLabel);

    // Tarjetas de sensores
    for (int i = 0; i < m_simulator->sensorCount(); i++) {
        SensorCard* card = new SensorCard(m_simulator->sensor(i), this);
        connect(card, &SensorCard::clicked, this, [this, i]() {
            onSensorCardClicked(i);
        });
        m_cards.append(card);
        leftLayout->addWidget(card);
    }
    m_cards[0]->setSelected(true);

    leftLayout->addStretch();

    // Controles de conexión
    QLabel* connLabel = new QLabel("FUENTE");
    connLabel->setStyleSheet("color: #555577; font-size: 10px; font-weight: bold; letter-spacing: 2px;");
    leftLayout->addWidget(connLabel);

    m_rbSimulator = new QRadioButton("Simulador");
    m_rbSimulator->setChecked(true);
    m_rbSimulator->setStyleSheet("color: #aaaaaa; font-size: 12px;");
    leftLayout->addWidget(m_rbSimulator);

    m_rbSerial = new QRadioButton("Serial");
    m_rbSerial->setStyleSheet("color: #aaaaaa; font-size: 12px;");
    leftLayout->addWidget(m_rbSerial);

    m_portCombo = new QComboBox();
    m_portCombo->setStyleSheet(
        "background-color: #1e1e2e; color: #aaaaaa;"
        "border: 1px solid #333; border-radius: 4px; padding: 3px;"
        );
    for (auto& port : QSerialPortInfo::availablePorts())
        m_portCombo->addItem(port.portName());
    if (m_portCombo->count() == 0)
        m_portCombo->addItem("Sin puertos");
    leftLayout->addWidget(m_portCombo);

    m_btnConnect = new QPushButton("Conectar");
    m_btnConnect->setStyleSheet(
        "background-color: #2d2d5e; color: white; border-radius: 6px;"
        "padding: 6px; font-weight: bold;"
        );
    connect(m_btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    leftLayout->addWidget(m_btnConnect);

    m_statusLabel = new QLabel("● Simulando");
    m_statusLabel->setStyleSheet("color: #4CAF50; font-size: 11px;");
    leftLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(leftPanel);

    // ── Panel derecho ────────────────────────────────
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(15, 15, 15, 10);
    rightLayout->setSpacing(10);

    // Chart view (se llena en setupChart)
    m_chartView = new QChartView();
    m_chartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(m_chartView, 1);

    // Barra de stats
    QWidget* statsBar = new QWidget();
    statsBar->setStyleSheet(
        "background-color: #1e1e2e; border-radius: 8px;"
        );
    QHBoxLayout* statsLayout = new QHBoxLayout(statsBar);
    statsLayout->setContentsMargins(20, 10, 20, 10);

    auto makeStatLabel = [](const QString& title) -> QLabel* {
        QLabel* lbl = new QLabel(title);
        lbl->setStyleSheet("color: #aaaaaa; font-size: 13px;");
        lbl->setAlignment(Qt::AlignCenter);
        return lbl;
    };

    m_lblMin = makeStatLabel("MIN: --.-°C");
    m_lblMax = makeStatLabel("MAX: --.-°C");
    m_lblAvg = makeStatLabel("AVG: --.-°C");

    statsLayout->addWidget(m_lblMin);
    statsLayout->addWidget(m_lblMax);
    statsLayout->addWidget(m_lblAvg);

    rightLayout->addWidget(statsBar);
    mainLayout->addWidget(rightPanel, 1);
}

void MainWindow::setupChart() {
    m_chart = new QChart();
    m_chart->setBackgroundBrush(QBrush(QColor("#12121f")));
    m_chart->setPlotAreaBackgroundBrush(QBrush(QColor("#1a1a2e")));
    m_chart->setPlotAreaBackgroundVisible(true);
    m_chart->legend()->hide();
    m_chart->setMargins(QMargins(5, 5, 5, 5));

    m_series = new QLineSeries();
    QColor c = m_simulator->sensor(0).color();
    QPen pen(c);
    pen.setWidth(2);
    m_series->setPen(pen);
    m_chart->addSeries(m_series);

    m_axisX = new QValueAxis();
    m_axisX->setRange(0, MAX_POINTS);
    m_axisX->setLabelFormat("%d");
    m_axisX->setLabelsColor(QColor("#555577"));
    m_axisX->setGridLineColor(QColor("#2a2a3a"));
    m_axisX->setTitleText("Tiempo");
    m_axisX->setTitleBrush(QBrush(QColor("#555577")));

    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 150);
    m_axisY->setLabelFormat("%.1f");
    m_axisY->setLabelsColor(QColor("#555577"));
    m_axisY->setGridLineColor(QColor("#2a2a3a"));
    m_axisY->setTitleText("°C");
    m_axisY->setTitleBrush(QBrush(QColor("#555577")));

    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    m_chartView->setChart(m_chart);
}

void MainWindow::onDataUpdated() {
    double val = m_source->sensor(m_selectedSensor).currentValue();

    // Actualizar tarjetas
    for (int i = 0; i < m_source->sensorCount(); i++)
        m_cards[i]->updateValue(m_source->sensor(i).currentValue());

    // Actualizar serie
    m_series->append(m_tick, val);
    if (m_series->count() > MAX_POINTS)
        m_series->remove(0);

    m_axisX->setRange(
        qMax(0, m_tick - MAX_POINTS),
        qMax(MAX_POINTS, m_tick)
        );

    // Stats
    m_min = qMin(m_min, val);
    m_max = qMax(m_max, val);
    m_sum += val;
    m_count++;

    m_lblMin->setText(QString("MIN: %1°C").arg(m_min, 0, 'f', 1));
    m_lblMax->setText(QString("MAX: %1°C").arg(m_max, 0, 'f', 1));
    m_lblAvg->setText(QString("AVG: %1°C").arg(m_sum / m_count, 0, 'f', 1));

    m_tick++;
}

void MainWindow::onSensorCardClicked(int index) {
    m_selectedSensor = index;

    for (int i = 0; i < m_cards.size(); i++)
        m_cards[i]->setSelected(i == index);

    // Cambiar color de la serie
    QColor c = m_source->sensor(index).color();
    QPen pen(c);
    pen.setWidth(2);
    m_series->setPen(pen);

    // Resetear gráfico y stats
    m_series->clear();
    m_tick = 0;
    m_min = 1e9; m_max = -1e9; m_sum = 0; m_count = 0;
    m_lblMin->setText("MIN: --.-°C");
    m_lblMax->setText("MAX: --.-°C");
    m_lblAvg->setText("AVG: --.-°C");

  /*  // Ajustar rango Y según sensor
    const SensorChannel& s = m_source->sensor(index);
    m_axisY->setRange(s.currentValue() - 20, s.currentValue() + 20);
*/
    m_axisY->setRange(0, 150);
}

void MainWindow::onConnectClicked() {
    if (m_rbSimulator->isChecked()) {
        switchSource(m_simulator);
        m_statusLabel->setText("● Simulando");
        m_statusLabel->setStyleSheet("color: #4CAF50; font-size: 11px;");
    } else {
        m_serial->setPort(m_portCombo->currentText());
        switchSource(m_serial);
        m_statusLabel->setText("● Serial: " + m_portCombo->currentText());
        m_statusLabel->setStyleSheet("color: #2196F3; font-size: 11px;");
    }
}

void MainWindow::switchSource(IDataSource* source) {
    if (m_source) {
        m_source->stop();
        disconnect(m_source, &IDataSource::dataUpdated, this, &MainWindow::onDataUpdated);
    }
    m_source = source;
    connect(m_source, &IDataSource::dataUpdated, this, &MainWindow::onDataUpdated);
    m_source->start();

    m_series->clear();
    m_tick = 0;
    m_min = 1e9; m_max = -1e9; m_sum = 0; m_count = 0;
}