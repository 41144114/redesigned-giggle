#ifndef SIGNALVIEW_H
#define SIGNALVIEW_H

#include <QWidget>
#include <QMdiSubWindow>
class QCustomPlot;

namespace Ui {
class SignalView;
}

class SignalView : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit SignalView(QMdiSubWindow *parent = 0);
    ~SignalView();

private:
    Ui::SignalView *ui;
    QCustomPlot* _pPlot;
};

#endif // SIGNALVIEW_H
