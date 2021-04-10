#include "Views/mainwindow.h"
#include <QApplication>

#include "Views/settingswindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(
        "Fusion"); //Изменяет немного внешний вид виджетов (более скруглённые углы, да и вообще вид более приятный)

    //Теперь можно передавать эти структуры через сигналы/слоты
    qRegisterMetaType<signalsParams_t>("signalsParams_t");
    qRegisterMetaType<signalsLimits_t>("signalsLimits_t");

    //Создали и вывели главное окошечко
    MainWindow w;
    w.show();

    return a.exec();
}
