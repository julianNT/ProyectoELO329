#ifndef SENSORCARD_H
#define SENSORCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "sensorchannel.h"

class SensorCard : public QWidget {
    Q_OBJECT

public:
    explicit SensorCard(const SensorChannel& sensor, QWidget* parent = nullptr);
    void updateValue(double value);
    void setSelected(bool selected);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    const SensorChannel& m_sensor;
    QLabel* m_nameLabel;
    QLabel* m_valueLabel;
    bool m_selected;

    void updateStyle();
};

#endif // SENSORCARD_H