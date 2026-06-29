#include "DataSimulator.h"
#include <cstdlib>
#include <cmath>

DataSimulator::DataSimulator(const ConfigReader& config, QObject* parent)
    : QObject(parent)
{
    for (int i = 0; i < config.sensorCount(); i++) {
        const SensorConfig& sc = config.sensor(i);
        m_configs.append(sc);
        SimState st;
        st.value = (sc.min + sc.max) / 2.0;
        st.phase = i * 1.5;
        m_states.append(st);
    }
    connect(&m_timer, &QTimer::timeout, this, &DataSimulator::onTick);
}

void DataSimulator::start(int intervalMs) {
    m_timer.start(intervalMs);
}

void DataSimulator::stop() {
    m_timer.stop();
}

int DataSimulator::sensorCount() const {
    return m_configs.size();
}

SensorData DataSimulator::currentData(int index) const {
    SensorData d;
    d.type = m_configs[index].type;
    d.value = m_states[index].value;
    d.unit = m_configs[index].unit;
    d.name = m_configs[index].name;
    return d;
}

void DataSimulator::onTick() {
    for (int i = 0; i < m_configs.size(); i++) {
        const SensorConfig& cfg = m_configs[i];
        SimState& st = m_states[i];

        double range = cfg.max - cfg.min;
        double center = (cfg.max + cfg.min) / 2.0;

        st.phase += 0.08;
        double noise = ((std::rand() % 100) / 100.0 - 0.5) * range * 0.03;
        st.value = center + (range * 0.35) * std::sin(st.phase) + noise;
        st.value = std::max(cfg.min, std::min(cfg.max, st.value));
    }
    emit dataUpdated();
}
