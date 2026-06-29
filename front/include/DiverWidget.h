#ifndef DIVERWIDGET_H
#define DIVERWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include "ConfigReader.h"
#include "DataSimulator.h"
#include "AlarmMonitor.h"

class DiverWidget : public QWidget {
    Q_OBJECT

public:
    explicit DiverWidget(const ConfigReader& config, DataSimulator* sim,
                         AlarmMonitor* alarm, QWidget* parent = nullptr);

    void refresh();

public slots:
    void onAlarmChanged(int sensorIndex, AlarmLevel level, const QString& msg);

private:
    QWidget* createChartCard(const QString& title, const QString& unit,
                             double axisMin, double axisMax,
                             QChart*& chart, QChartView*& view,
                             QLineSeries*& series, QValueAxis*& axisY,
                             QLabel*& alarmLabel);

    DataSimulator* m_simulator;
    AlarmMonitor* m_alarmMonitor;

    QChart* m_pressureChart;
    QChartView* m_pressureView;
    QLineSeries* m_pressureSeries;
    QValueAxis* m_pressureAxisY;
    QLabel* m_pressureAlarm;

    QChart* m_tempChart;
    QChartView* m_tempView;
    QLineSeries* m_tempSeries;
    QValueAxis* m_tempAxisY;
    QLabel* m_tempAlarm;

    int m_tick;
    static constexpr int MAX_POINTS = 100;
};

#endif
