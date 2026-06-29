#include "serialsource.h"

SerialSource::SerialSource(QObject* parent) : IDataSource(parent) {
    // Mismos canales que el simulador, se llenan con datos reales al conectar
    m_sensors.append(SensorChannel("Ambiente", 18.0,  25.0, QColor("#2196F3")));
    m_sensors.append(SensorChannel("Proceso",  60.0,  80.0, QColor("#4CAF50")));
    m_sensors.append(SensorChannel("Critico",  90.0, 110.0, QColor("#F44336")));

    connect(&m_port, &QSerialPort::readyRead, this, &SerialSource::onDataReady);
}

void SerialSource::setPort(const QString& portName, int baudRate) {
    m_port.setPortName(portName);
    m_port.setBaudRate(baudRate);
}

void SerialSource::start() {
    if (!m_port.isOpen())
        m_port.open(QIODevice::ReadOnly);
}

void SerialSource::stop() {
    if (m_port.isOpen())
        m_port.close();
}

int SerialSource::sensorCount() const {
    return m_sensors.size();
}

const SensorChannel& SerialSource::sensor(int index) const {
    return m_sensors[index];
}

void SerialSource::onDataReady() {
    // Protocolo esperado: "21.3,71.2,98.4\n"
    m_buffer += m_port.readAll();

    while (m_buffer.contains('\n')) {
        int idx = m_buffer.indexOf('\n');
        QString line = m_buffer.left(idx).trimmed();
        m_buffer = m_buffer.mid(idx + 1);

        QStringList parts = line.split(',');
        for (int i = 0; i < parts.size() && i < m_sensors.size(); i++)
            m_sensors[i].setValue(parts[i].toDouble());

        emit dataUpdated();
    }
}