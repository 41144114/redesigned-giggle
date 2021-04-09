#ifndef GENERALVIEW_H
#define GENERALVIEW_H

#include <QWidget>
class QCustomPlot;

namespace Ui
{
class GeneralView;
}

class GeneralView : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralView(QWidget* parent = nullptr);
    ~GeneralView();

public slots:
    void updateSignals(double val1, double val2, double val3, double val4, double time);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::GeneralView* ui;
    QCustomPlot* _pPlot;
};

#endif // GENERALVIEW_H
