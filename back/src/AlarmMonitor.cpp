#include "AlarmMonitor.h"

AlarmMonitor::AlarmMonitor(const ConfigReader& config, QObject* parent)
    : QObject(parent)
{
    for (int i = 0; i < config.sensorCount(); i++) {
        const SensorConfig& sc = config.sensor(i);
        SensorState st;
        st.name = sc.name;
        st.thresholds = sc.alarms;
        st.currentLevel = AlarmLevel::Normal;
        st.currentMessage = buildMessage(i, AlarmLevel::Normal);
        m_sensors.append(st);
    }
}

void AlarmMonitor::evaluate(int sensorIndex, double value) {
    if (sensorIndex < 0 || sensorIndex >= m_sensors.size())
        return;

    SensorState& st = m_sensors[sensorIndex];
    AlarmLevel newLevel = AlarmLevel::Normal;
    const AlarmThresholds& t = st.thresholds;

    if (value < t.criticalLow || value > t.criticalHigh)
        newLevel = AlarmLevel::Critical;
    else if (value < t.warningLow || value > t.warningHigh)
        newLevel = AlarmLevel::Warning;

    if (newLevel != st.currentLevel) {
        st.currentLevel = newLevel;
        st.currentMessage = buildMessage(sensorIndex, newLevel);
        emit alarmStateChanged(sensorIndex, newLevel, st.currentMessage);
    }
}

AlarmLevel AlarmMonitor::level(int sensorIndex) const {
    return m_sensors[sensorIndex].currentLevel;
}

QString AlarmMonitor::message(int sensorIndex) const {
    return m_sensors[sensorIndex].currentMessage;
}

QString AlarmMonitor::buildMessage(int sensorIndex, AlarmLevel lvl) const {
    const QString& name = m_sensors[sensorIndex].name;
    switch (lvl) {
        case AlarmLevel::Normal:   return name + ": Normal";
        case AlarmLevel::Warning:  return name + ": Alerta";
        case AlarmLevel::Critical: return name + ": Crítico";
    }
    return name + ": Normal";
}
