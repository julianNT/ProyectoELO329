#include "sensorcard.h"
#include <QVBoxLayout>
#include <QMouseEvent>

SensorCard::SensorCard(const SensorChannel& sensor, QWidget* parent)
    : QWidget(parent), m_sensor(sensor), m_selected(false)
{
    setFixedHeight(80);
    setCursor(Qt::PointingHandCursor);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);

    m_nameLabel = new QLabel(sensor.name(), this);
    m_nameLabel->setStyleSheet("color: #aaaaaa; font-size: 11px; font-weight: bold;");

    m_valueLabel = new QLabel("--.- °C", this);
    m_valueLabel->setStyleSheet(
        QString("color: %1; font-size: 22px; font-weight: bold;")
            .arg(sensor.color().name())
        );

    layout->addWidget(m_nameLabel);
    layout->addWidget(m_valueLabel);

    updateStyle();
}

void SensorCard::updateValue(double value) {
    m_valueLabel->setText(QString("%1 °C").arg(value, 0, 'f', 1));
}

void SensorCard::setSelected(bool selected) {
    m_selected = selected;
    updateStyle();
}

void SensorCard::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    emit clicked();
}

void SensorCard::updateStyle() {
    QString border = m_selected
                         ? QString("border: 2px solid %1;").arg(m_sensor.color().name())
                         : "border: 1px solid #333333;";

    setStyleSheet(QString(
                      "SensorCard {"
                      "  background-color: #1e1e2e;"
                      "  border-radius: 8px;"
                      "  %1"
                      "}"
                      ).arg(border));
}