#include "mainwindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    qsrand((uint)QTime::currentTime().msec());
    MainWindow w;
    w.show();

    return a.exec();
}
