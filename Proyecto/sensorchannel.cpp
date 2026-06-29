#include "sensorchannel.h"
#include <cmath>
#include <cstdlib>

SensorChannel::SensorChannel(const QString& name, double minVal, double maxVal, QColor color)
    : m_name(name), m_min(minVal), m_max(maxVal), m_color(color),
    m_currentValue((minVal + maxVal) / 2.0), m_phase(0.0)
{}

QString SensorChannel::name() const { return m_name; }
QColor SensorChannel::color() const { return m_color; }
double SensorChannel::currentValue() const { return m_currentValue; }

void SensorChannel::update() {
    double range = m_max - m_min;
    double center = (m_max + m_min) / 2.0;

    // oscilación senoidal + ruido aleatorio
    m_phase += 0.05;
    double noise = ((rand() % 100) / 100.0 - 0.5) * range * 0.05;
    m_currentValue = center + (range * 0.4) * std::sin(m_phase) + noise;
}

void SensorChannel::setValue(double val) {
    m_currentValue = val;
}