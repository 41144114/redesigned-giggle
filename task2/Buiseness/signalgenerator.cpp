#include "signalgenerator.h"

#include <QTimer>
const int updateInterval = 50;

SignalGenerator::SignalGenerator(QObject* parent) : QObject(parent) {}

void SignalGenerator::startProcess()
{
    _pUpdateTimer = new QTimer();
    _pUpdateTimer->setInterval(updateInterval);
    connect(_pUpdateTimer, &QTimer::timeout, this, &SignalGenerator::generateSignal);
}

void SignalGenerator::generateSignal()
{
    generateSignal1();
    generateSignal2();
    generateSignal3();
    generateSignal4();
}

void SignalGenerator::startGenerateSignal() { _pUpdateTimer->start(); }

void SignalGenerator::generateSignal1() {}

void SignalGenerator::generateSignal2() {}

void SignalGenerator::generateSignal3() {}

void SignalGenerator::generateSignal4() {}

void SignalGenerator::setSignal1Param() {}

void SignalGenerator::setSignal2Param() {}

void SignalGenerator::setSignal3Param() {}

void SignalGenerator::setSignal4Param() {}
