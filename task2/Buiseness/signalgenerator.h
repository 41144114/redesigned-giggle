#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include "Views/settingswindow.h"
#include <QObject>
class QTimer;

class SignalGenerator : public QObject
{
    Q_OBJECT

public:
    explicit SignalGenerator(QObject* parent = nullptr);

    void startGenerateSignal();

public slots:
    void startProcess();
    void setParams(signalsParams_t params);
    void startGenerate();
    void stopGenerate();

signals:
    void updateSignals(double val1, double val2, double val3, double val4, double time);

private slots:
    void generateSignal();

private:
    QTimer* _pUpdateTimer;
    signalsParams_t _curParams;
    double _curTime;
    bool isTraingleRaising;
    double traingleLastValue;
    double nPeriod;

    double _newValue1;
    double _newValue2;
    double _newValue3;
    double _newValue4;

    void generateSignal1();
    void generateSignal2();
    void generateSignal3();
    void generateSignal4();
};

#endif // SIGNALGENERATOR_H
