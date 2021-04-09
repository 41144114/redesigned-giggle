#include "signalgenerator.h"

#include <QTimer>
#include <QDebug>
const double pi = 3.1415926535897932384626433832795;

SignalGenerator::SignalGenerator(QObject* parent) : QObject(parent) {}

void SignalGenerator::startProcess()
{
    _pUpdateTimer = new QTimer();
    connect(_pUpdateTimer, &QTimer::timeout, this, &SignalGenerator::generateSignal);
}

void SignalGenerator::startGenerate()
{
    isTraingleRaising = true;
    traingleLastValue = 0;
    nPeriod = 0;
    _curTime = 0;
    _pUpdateTimer->setInterval(1000 / _curParams.generateFreq);
    _pUpdateTimer->start();
}

void SignalGenerator::stopGenerate() { _pUpdateTimer->stop(); }

void SignalGenerator::generateSignal()
{
    _curTime += 1.0 / _curParams.generateFreq;
    generateSignal1();
    generateSignal2();
    generateSignal3();
    generateSignal4();
    emit updateSignals(_newValue1, _newValue2, _newValue3, _newValue4, _curTime);
}

void SignalGenerator::startGenerateSignal() { _pUpdateTimer->start(); }

void SignalGenerator::generateSignal1()
{
    _newValue1 = _curParams.sinAmp * sin(2 * pi * _curParams.sinFreq * _curTime);
}

void SignalGenerator::generateSignal2()
{
    double temp;
    if (isTraingleRaising)
    {
        temp = traingleLastValue + _curParams.traingleRaiseK / _curParams.generateFreq;
        if (temp < _curParams.traingleMax)
            _newValue2 = temp;
        else
        {
            isTraingleRaising = false;
            _newValue2 = traingleLastValue - _curParams.traingleUnriseK / _curParams.generateFreq;
        }
    }
    else
    {
        temp = traingleLastValue - _curParams.traingleUnriseK / _curParams.generateFreq;
        if (temp > _curParams.traingleMin)
            _newValue2 = temp;
        else
        {
            isTraingleRaising = true;
            _newValue2 = traingleLastValue + _curParams.traingleRaiseK / _curParams.generateFreq;
        }
    }
    traingleLastValue = _newValue2;
}

void SignalGenerator::generateSignal3()
{
    double ret = 0;
    double temp = 0;

    for (int i = 0; i < 12; i++)
    {
        temp = 0.1 * (qrand() % 10) + 0.01 * (qrand() % 10) + 0.001 * (qrand() % 10) + 0.0001 * (qrand() % 10) +
               0.00001 * (qrand() % 10);

        ret += temp;
    }

    ret -= 6;
    ret *= _curParams.randomSko;
    ret += _curParams.randomMo;
    _newValue3 = ret;
}

void SignalGenerator::generateSignal4()
{
    double x = _curTime - nPeriod * _curParams.variantPeriod - 0.5*_curParams.variantPeriod;
    double k = 4 * (_curParams.variantMax - _curParams.variantMin) /
               (_curParams.variantPeriod * _curParams.variantPeriod * _curParams.variantPeriod);

    double b = _curParams.variantMax  - k * (_curParams.variantPeriod * _curParams.variantPeriod * _curParams.variantPeriod) / 8;
    if(_curTime > (nPeriod + 1)* _curParams.variantPeriod)
        nPeriod++;
    _newValue4 = k * x * x * x + b /*+ _curParams.variantMin*/;
}

void SignalGenerator::setParams(signalsParams_t params)
{
    _curParams = params;
    _pUpdateTimer->stop();
    _pUpdateTimer->setInterval(1000 / _curParams.generateFreq);
}
