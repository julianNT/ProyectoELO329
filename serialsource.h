    #ifndef SERIALSOURCE_H
    #define SERIALSOURCE_H

    #include <QSerialPort>
    #include <QVector>
    #include "idatasource.h"

    class SerialSource : public IDataSource {
        Q_OBJECT

    public:
        explicit SerialSource(QObject* parent = nullptr);

        void setPort(const QString& portName, int baudRate = 9600);

        void start() override;
        void stop() override;
        int sensorCount() const override;
        const SensorChannel& sensor(int index) const override;

    private slots:
        void onDataReady();

    private:
        QSerialPort m_port;
        QVector<SensorChannel> m_sensors;
        QString m_buffer;
    };

    #endif // SERIALSOURCE_H