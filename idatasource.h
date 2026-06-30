#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#include <QObject>
#include "sensorchannel.h"

class IDataSource : public QObject {
    Q_OBJECT

public:
    explicit IDataSource(QObject* parent = nullptr) : QObject(parent) {}
    virtual ~IDataSource() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual int sensorCount() const = 0;
    virtual const SensorChannel& sensor(int index) const = 0;

signals:
    void dataUpdated();
};

#endif // IDATASOURCE_H