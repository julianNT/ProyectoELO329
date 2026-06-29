#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include "SensorData.h"
#include "ConfigReader.h"

class DataSimulator : public QObject {
    Q_OBJECT

public:
    explicit DataSimulator(const ConfigReader& config, QObject* parent = nullptr);

    void start(int intervalMs = 200);
    void stop();
    int sensorCount() const;
    SensorData currentData(int index) const;

signals:
    void dataUpdated();

private slots:
    void onTick();

private:
    struct SimState {
        double value;
        double phase;
    };

    QTimer m_timer;
    QVector<SensorConfig> m_configs;
    QVector<SimState> m_states;
};

#endif
