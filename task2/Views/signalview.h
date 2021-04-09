#ifndef SIGNALVIEW_H
#define SIGNALVIEW_H

#include "settingswindow.h"
#include <QCloseEvent>
#include <QWidget>
class SimpleSignalView;

namespace Ui
{
class SignalView;
}

class SignalView : public QWidget
{
    Q_OBJECT

public:
    explicit SignalView(QWidget* parent = nullptr);
    ~SignalView();
    void setType(int type);

public slots:
    void updateSignal(double val1, double val2, double val3, double val4, double time);
    void setLimits(signalsLimits_t limits, signalsParams_t params);

signals:
    void closed();
    void showStatus(QString status, int type);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::SignalView* ui;
    SimpleSignalView* _pGraph;
    QPixmap* _failurePixmap;
    QPixmap* _goodPixmap;
    int _type;
    bool _isFailure;

    double _lowLimit;
    double _upLimit;

    void checkLimits(double val);
};

#endif // SIGNALVIEW_H
