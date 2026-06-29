#include "MainWindow.h"
#include "DiverWidget.h"

MainWindow::MainWindow(DataSimulator* sim, AlarmMonitor* alarm,
                       const ConfigReader& config, QWidget* parent)
    : QMainWindow(parent), m_simulator(sim), m_alarmMonitor(alarm)
{
    setWindowTitle("HMI Buceo - " + config.diverName());
    resize(1000, 700);

    m_diverWidget = new DiverWidget(config, sim, alarm, this);
    setCentralWidget(m_diverWidget);

    connect(m_simulator, &DataSimulator::dataUpdated,
            this, &MainWindow::onDataUpdated);
}

void MainWindow::onDataUpdated() {
    for (int i = 0; i < m_simulator->sensorCount(); i++) {
        SensorData data = m_simulator->currentData(i);
        m_alarmMonitor->evaluate(i, data.value);
    }
    m_diverWidget->refresh();
}
