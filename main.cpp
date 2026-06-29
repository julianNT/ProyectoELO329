#include <QApplication>
#include "MainWindow.h"
#include "ConfigReader.h"
#include "DataSimulator.h"
#include "AlarmMonitor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ConfigReader config("config.txt");
    DataSimulator simulator(config);
    AlarmMonitor alarm(config);
    MainWindow w(&simulator, &alarm, config);

    simulator.start();
    w.show();

    return app.exec();
}
