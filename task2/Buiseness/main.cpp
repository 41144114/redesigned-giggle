#include "Views/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");//Изменяет немного внешний вид виджетов (более скруглённые углы, да и вообще вид более приятный)
    MainWindow w;
    w.show();

    return a.exec();
}
