#include "ConfigReader.h"
#include <QFile>
#include <QTextStream>

ConfigReader::ConfigReader(const QString& path) {
    parseFile(path);
}

QString ConfigReader::diverName() const { return m_diverName; }
int ConfigReader::sensorCount() const { return m_sensors.size(); }
const SensorConfig& ConfigReader::sensor(int index) const { return m_sensors[index]; }

void ConfigReader::parseFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    bool inSensor = false;
    SensorConfig current;

    auto flushSensor = [&]() {
        if (inSensor) {
            m_sensors.append(current);
            current = SensorConfig();
            inSensor = false;
        }
    };

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;

        if (line.startsWith('[')) {
            flushSensor();
            QString section = line.mid(1, line.length() - 2);
            if (section == "diver") {
            } else if (section == "sensor:pressure") {
                inSensor = true;
                current.type = SensorData::Pressure;
            } else if (section == "sensor:temperature") {
                inSensor = true;
                current.type = SensorData::Temperature;
            }
            continue;
        }

        int eq = line.indexOf('=');
        if (eq < 0) continue;

        QString key = line.left(eq).trimmed();
        QString val = line.mid(eq + 1).trimmed();

        if (key == "name") {
            if (inSensor) current.name = val;
            else m_diverName = val;
        } else if (key == "unit") {
            current.unit = val;
        } else if (key == "min") {
            current.min = val.toDouble();
        } else if (key == "max") {
            current.max = val.toDouble();
        } else if (key == "alarm_warning_low") {
            current.alarms.warningLow = val.toDouble();
        } else if (key == "alarm_warning_high") {
            current.alarms.warningHigh = val.toDouble();
        } else if (key == "alarm_critical_low") {
            current.alarms.criticalLow = val.toDouble();
        } else if (key == "alarm_critical_high") {
            current.alarms.criticalHigh = val.toDouble();
        }
    }

    flushSensor();
}
