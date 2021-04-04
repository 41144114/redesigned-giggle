#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
class QTimer;

class SignalGenerator : public QObject
{
    Q_OBJECT

public:
    explicit SignalGenerator(QObject* parent = nullptr);
    void setSignal1Param();
    void setSignal2Param();
    void setSignal3Param();
    void setSignal4Param();
    void startGenerateSignal();

public slots:
    void startProcess();

signals:
    void updateSignal1(double val, double time);
    void updateSignal2(double val, double time);
    void updateSignal3(double val, double time);
    void updateSignal4(double val, double time);

private slots:
    void generateSignal();

private:
    QTimer* _pUpdateTimer;

    void generateSignal1();
    void generateSignal2();
    void generateSignal3();
    void generateSignal4();
};

#endif // SIGNALGENERATOR_H
