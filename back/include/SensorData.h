#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QString>

struct SensorData {
    enum Type { Pressure, Temperature };
    Type type;
    double value;
    QString unit;
    QString name;
};

#endif
