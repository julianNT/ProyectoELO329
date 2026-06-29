#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include "idatasource.h"
#include "datasimulator.h"
#include "serialsource.h"
#include "sensorcard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onDataUpdated();
    void onSensorCardClicked(int index);
    void onConnectClicked();

private:
    void setupUI();
    void setupChart();
    void switchSource(IDataSource* source);

    Ui::MainWindow* ui;

    // Fuente de datos
    IDataSource*    m_source;
    DataSimulator*  m_simulator;
    SerialSource*   m_serial;

    // Panel izquierdo
    QVector<SensorCard*> m_cards;
    int m_selectedSensor;

    // Controles
    QRadioButton* m_rbSimulator;
    QRadioButton* m_rbSerial;
    QComboBox*    m_portCombo;
    QPushButton*  m_btnConnect;
    QLabel*       m_statusLabel;

    // Chart
    QChart*      m_chart;
    QChartView*  m_chartView;
    QLineSeries* m_series;
    QValueAxis*  m_axisX;
    QValueAxis*  m_axisY;

    // Stats
    QLabel* m_lblMin;
    QLabel* m_lblMax;
    QLabel* m_lblAvg;

    int m_tick;
    double m_min, m_max, m_sum;
    int m_count;
    static constexpr int MAX_POINTS = 100;
};

#endif // MAINWINDOW_H