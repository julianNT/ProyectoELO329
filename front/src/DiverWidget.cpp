#include "DiverWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

DiverWidget::DiverWidget(const ConfigReader& config, DataSimulator* sim,
                         AlarmMonitor* alarm, QWidget* parent)
    : QWidget(parent), m_simulator(sim), m_alarmMonitor(alarm), m_tick(0)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QLabel* title = new QLabel(config.diverName());
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #ffffff;"
                         "background-color: #12121f; padding: 8px 12px;"
                         "border-radius: 8px;");
    mainLayout->addWidget(title);

    const SensorConfig& pressCfg = config.sensor(0);
    const SensorConfig& tempCfg  = config.sensor(1);

    QWidget* pressureCard = createChartCard(
        "Presión", "bar", pressCfg.min, pressCfg.max,
        m_pressureChart, m_pressureView,
        m_pressureSeries, m_pressureAxisY, m_pressureAlarm);
    mainLayout->addWidget(pressureCard, 1);

    QWidget* tempCard = createChartCard(
        "Temperatura", "°C", tempCfg.min, tempCfg.max,
        m_tempChart, m_tempView,
        m_tempSeries, m_tempAxisY, m_tempAlarm);
    mainLayout->addWidget(tempCard, 1);

    connect(m_alarmMonitor, &AlarmMonitor::alarmStateChanged,
            this, &DiverWidget::onAlarmChanged);

    setStyleSheet("background-color: #12121f;");
}

QWidget* DiverWidget::createChartCard(
    const QString& title, const QString& unit,
    double axisMin, double axisMax,
    QChart*& chart, QChartView*& view,
    QLineSeries*& series, QValueAxis*& axisY,
    QLabel*& alarmLabel)
{
    QFrame* card = new QFrame();
    card->setStyleSheet("QFrame { background-color: #1a1a2e; border-radius: 10px; }");
    QVBoxLayout* layout = new QVBoxLayout(card);
    layout->setContentsMargins(12, 10, 12, 10);

    QHBoxLayout* header = new QHBoxLayout();
    QLabel* titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("color: #aaaaaa; font-size: 13px; font-weight: bold;");
    header->addWidget(titleLabel);
    header->addStretch();

    alarmLabel = new QLabel("\u25CF Normal");
    alarmLabel->setStyleSheet("color: #4CAF50; font-size: 12px; font-weight: bold;");
    header->addWidget(alarmLabel);
    layout->addLayout(header);

    chart = new QChart();
    chart->setBackgroundBrush(QBrush(QColor("#12121f")));
    chart->setPlotAreaBackgroundBrush(QBrush(QColor("#0f0f1a")));
    chart->setPlotAreaBackgroundVisible(true);
    chart->legend()->hide();
    chart->setMargins(QMargins(5, 5, 5, 5));

    series = new QLineSeries();
    QPen pen(title == "Presión" ? QColor("#2196F3") : QColor("#F44336"));
    pen.setWidth(2);
    series->setPen(pen);
    chart->addSeries(series);

    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, MAX_POINTS);
    axisX->setLabelsVisible(false);
    axisX->setGridLineColor(QColor("#2a2a3a"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(axisMin, axisMax);
    axisY->setLabelFormat("%.1f");
    axisY->setLabelsColor(QColor("#555577"));
    axisY->setGridLineColor(QColor("#2a2a3a"));
    axisY->setTitleText(unit);
    axisY->setTitleBrush(QBrush(QColor("#555577")));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumHeight(200);
    layout->addWidget(view);

    return card;
}

void DiverWidget::refresh() {
    for (int i = 0; i < m_simulator->sensorCount(); i++) {
        SensorData data = m_simulator->currentData(i);
        QLineSeries* series = (data.type == SensorData::Pressure)
                                  ? m_pressureSeries : m_tempSeries;
        QValueAxis* axisY = (data.type == SensorData::Pressure)
                                ? m_pressureAxisY : m_tempAxisY;

        series->append(m_tick, data.value);
        if (series->count() > MAX_POINTS)
            series->remove(0);
    }
    m_tick++;
}

void DiverWidget::onAlarmChanged(int sensorIndex, AlarmLevel level, const QString& msg) {
    QLabel* label = (sensorIndex == 0) ? m_pressureAlarm : m_tempAlarm;
    label->setText(msg);

    switch (level) {
        case AlarmLevel::Normal:
            label->setStyleSheet("color: #4CAF50; font-size: 12px; font-weight: bold;");
            break;
        case AlarmLevel::Warning:
            label->setStyleSheet("color: #FF9800; font-size: 12px; font-weight: bold;");
            break;
        case AlarmLevel::Critical:
            label->setStyleSheet("color: #F44336; font-size: 12px; font-weight: bold;");
            break;
    }
}
