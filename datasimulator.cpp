#include "datasimulator.h"

DataSimulator::DataSimulator(QObject* parent) : IDataSource(parent) {
    m_sensors.append(SensorChannel("Ambiente", 18.0,  25.0, QColor("#2196F3")));
    m_sensors.append(SensorChannel("Proceso",  60.0,  80.0, QColor("#4CAF50")));
    m_sensors.append(SensorChannel("Critico",  90.0, 110.0, QColor("#F44336")));

    connect(&m_timer, &QTimer::timeout, this, &DataSimulator::onTick);
}

void DataSimulator::start() {
    m_timer.start(500);
}

void DataSimulator::stop() {
    m_timer.stop();
}

int DataSimulator::sensorCount() const {
    return m_sensors.size();
}

const SensorChannel& DataSimulator::sensor(int index) const {
    return m_sensors[index];
}

void DataSimulator::onTick() {
    for (auto& s : m_sensors)
        s.update();
    emit dataUpdated();
}