#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QString>
#include <QVector>
#include "SensorData.h"

struct AlarmThresholds {
    double warningLow;
    double warningHigh;
    double criticalLow;
    double criticalHigh;
};

struct SensorConfig {
    QString name;
    QString unit;
    SensorData::Type type;
    double min;
    double max;
    AlarmThresholds alarms;
};

class ConfigReader {
public:
    explicit ConfigReader(const QString& path);

    QString diverName() const;
    int sensorCount() const;
    const SensorConfig& sensor(int index) const;

private:
    QString m_diverName;
    QVector<SensorConfig> m_sensors;

    void parseFile(const QString& path);
};

#endif
