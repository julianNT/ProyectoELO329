#ifndef ALARMMONITOR_H
#define ALARMMONITOR_H

#include <QObject>
#include <QVector>
#include <QString>
#include "ConfigReader.h"

enum class AlarmLevel { Normal, Warning, Critical };

class AlarmMonitor : public QObject {
    Q_OBJECT

public:
    explicit AlarmMonitor(const ConfigReader& config, QObject* parent = nullptr);

    void evaluate(int sensorIndex, double value);
    AlarmLevel level(int sensorIndex) const;
    QString message(int sensorIndex) const;

signals:
    void alarmStateChanged(int sensorIndex, AlarmLevel level, const QString& msg);

private:
    struct SensorState {
        QString name;
        AlarmThresholds thresholds;
        AlarmLevel currentLevel;
        QString currentMessage;
    };

    QVector<SensorState> m_sensors;
    QString buildMessage(int sensorIndex, AlarmLevel lvl) const;
};

#endif
