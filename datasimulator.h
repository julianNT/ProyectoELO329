#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QTimer>
#include <QVector>
#include "idatasource.h"

class DataSimulator : public IDataSource {
    Q_OBJECT

public:
    explicit DataSimulator(QObject* parent = nullptr);

    void start() override;
    void stop() override;
    int sensorCount() const override;
    const SensorChannel& sensor(int index) const override;

private slots:
    void onTick();

private:
    QTimer m_timer;
    QVector<SensorChannel> m_sensors;
};

#endif // DATASIMULATOR_H