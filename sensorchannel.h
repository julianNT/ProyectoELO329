#ifndef SENSORCHANNEL_H
#define SENSORCHANNEL_H

#include <QString>
#include <QColor>

class SensorChannel {
public:
    SensorChannel(const QString& name, double minVal, double maxVal, QColor color);

    QString name() const;
    QColor color() const;
    double currentValue() const;
    void update();  // genera nuevo valor simulado
    void setValue(double val);

private:
    QString m_name;
    double m_min;
    double m_max;
    QColor m_color;
    double m_currentValue;
    double m_phase;  // para la oscilación
};

#endif // SENSORCHANNEL_H