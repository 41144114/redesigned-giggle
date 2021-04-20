#include "signalgenerator.h"

#include <QTimer>
#include <QDebug>
#include "math.h"
const double pi = 3.1415926535897932384626433832795;

SignalGenerator::SignalGenerator(QObject* parent) : QObject(parent) {}

void SignalGenerator::startProcess()
{
    _pUpdateTimer = new QTimer();   //Создали объект таймера в новом потоке
    connect(_pUpdateTimer, &QTimer::timeout, this, &SignalGenerator::generateSignal);//Присоединили таймер к функции
}

void SignalGenerator::startGenerate()
{
    //Настроили начальные параметры для генерации сигналов
    isTraingleRaising = true;
    traingleLastValue = 0;
    nPeriod = 0;
    _curTime = 0;

    //Настроили и запустили таймер
    _pUpdateTimer->setInterval(1000 / _curParams.generateFreq);
    _pUpdateTimer->start();
}

void SignalGenerator::stopGenerate()
{
    _pUpdateTimer->stop();  //Остановили таймер, чтоб новых данных не считал и в интерфейс не отправлял.
}

void SignalGenerator::generateSignal()
{
    //Инкрементировали текущее время. (прибавили период, то есть  1/частота )
    _curTime += 1.0 / _curParams.generateFreq;
    //Посчитали значения сигналов (записали полученные значения в переменные-члены класса)
    generateSignal1();
    generateSignal2();
    generateSignal3();
    generateSignal4();
    //Кидаем данные в интерфейс
    emit updateSignals(_newValue1, _newValue2, _newValue3, _newValue4, _curTime);               // <--------------
}                                                                                                       //       |
                                                                                                        //       |

void SignalGenerator::generateSignal1() // Генерируем синус
{
    //Получаем новое значение как произведение заданной амплитуды на значение синуса с заданой частотой в текущий момент времени
    _newValue1 = _curParams.sinAmp * sin(2 * pi * _curParams.sinFreq * _curTime);
}

void SignalGenerator::generateSignal2()
{
    double temp;
    if (isTraingleRaising)  //Если треугольник растёт
    {
        temp = traingleLastValue + _curParams.traingleRaiseK / _curParams.generateFreq; //Посчитали то что предположительно должно быть
        if (temp < _curParams.traingleMax)  //Если не превысили верхнюю границу треугольника - ок
            _newValue2 = temp;
        else    //Если превысили верхнюю границу треугольника
        {
            isTraingleRaising = false;  //Установили флаг, что надо теперь падать
            _newValue2 = traingleLastValue - _curParams.traingleUnriseK / _curParams.generateFreq; //Пересчитали значение
        }
    }
    else
    {
        temp = traingleLastValue - _curParams.traingleUnriseK / _curParams.generateFreq;//Посчитали то что предположительно должно быть
        if (temp > _curParams.traingleMin) //Если не занизили минимальное значение треугольника
            _newValue2 = temp;
        else    //Если предполагаемое значение меньше нижней границы
        {
            isTraingleRaising = true;   //Установили флаг, чтод надо расти
            _newValue2 = traingleLastValue + _curParams.traingleRaiseK / _curParams.generateFreq;   //Пересчитали значение
        }
    }
    traingleLastValue = _newValue2; //Запомнили последнее значение треугольника для будущих рассчетов
}

void SignalGenerator::generateSignal3()
{
    double ret = 0;
    double temp = 0;

    //Генерируем случайное число от 0 до 1 (12 раз) и складываем их
    for (int i = 0; i < 12; i++)
    {
        temp = 0.1 * (qrand() % 10) + 0.01 * (qrand() % 10) + 0.001 * (qrand() % 10) + 0.0001 * (qrand() % 10) +
               0.00001 * (qrand() % 10);

        ret += temp;
    }

    ret -= 6;   //Вычли смещение - получили нормированное нормальное распределение
    ret *= _curParams.randomSko;    //Установили нужное нам СКО
    ret += _curParams.randomMo;     //Установили нужное нам мат. ожидание
    _newValue3 = ret;   //Закинули в переменную, откуда значение отправится в интерфейс
}

void SignalGenerator::generateSignal4()
{
    //Посчитали координату x переболы, относительно середины отрезка текущего периода
    double x = _curTime - nPeriod * _curParams.variantPeriod - 0.5*_curParams.variantPeriod;

    //Посчитали коэффициент при x^3 параболы
    double k = 4 * (_curParams.variantMax - _curParams.variantMin) /
               (_curParams.variantPeriod * _curParams.variantPeriod * _curParams.variantPeriod);

    //Посчитали свободный член параболы
    double b = _curParams.variantMax  - k * (_curParams.variantPeriod * _curParams.variantPeriod * _curParams.variantPeriod) / 8;

    //Проверили, не перешли ли мы через период
    if(_curTime > (nPeriod + 1) * _curParams.variantPeriod)
        nPeriod++;
    _newValue4 = k * x * x * x + b; //Посчитали сигнал по формуле y = k*x^3 + b
}

void SignalGenerator::setParams(signalsParams_t params) //Здесь нам прилетает от интерфейса какие параметры нам нужно использовать
{                                                           //для генерации
    _curParams = params;    //Запомнили внутри класса эти параметры
    if(_pUpdateTimer->isActive())//Глянули, работаем ли мы уже
    {
        //Если работаем - остановили таймер, настроили на нужную частоту и снова запустили. Лучше так.
        _pUpdateTimer->stop();
        _pUpdateTimer->setInterval(1000 / _curParams.generateFreq);
        _pUpdateTimer->start();
    }
    else
        //Если не работаем - просто настроили таймер.
        _pUpdateTimer->setInterval(1000 / _curParams.generateFreq);
}
