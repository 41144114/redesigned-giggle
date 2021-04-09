#include "Views/mainwindow.h"
#include <QApplication>

#include "Views/settingswindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(
        "Fusion"); //Изменяет немного внешний вид виджетов (более скруглённые углы, да и вообще вид более приятный)

    qRegisterMetaType<signalsParams_t>("signalsParams_t");
    qRegisterMetaType<signalsLimits_t>("signalsLimits_t");

    MainWindow w;
    w.show();

    return a.exec();
}
