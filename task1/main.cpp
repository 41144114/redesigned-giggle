#include "mainwindow.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");//Изменяет немного внешний вид виджетов (более скруглённые углы, да и вообще вид более приятный)
    qsrand((uint)QTime::currentTime().msec());//Задаём стартовое значение для генерации случайных чисел
                                              //чтобы каждый запуск программы генерировались не одни и
                                            //те же случайные числа, а разные
    MainWindow w;
    w.show();

    return a.exec();
}
