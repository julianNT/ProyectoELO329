#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DataSimulator.h"
#include "AlarmMonitor.h"
#include "ConfigReader.h"

class DiverWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(DataSimulator* sim, AlarmMonitor* alarm,
                        const ConfigReader& config, QWidget* parent = nullptr);

private slots:
    void onDataUpdated();

private:
    DataSimulator* m_simulator;
    AlarmMonitor* m_alarmMonitor;
    DiverWidget* m_diverWidget;
};

#endif
