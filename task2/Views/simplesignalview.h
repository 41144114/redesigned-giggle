#ifndef SIMPLESIGNALVIEW_H
#define SIMPLESIGNALVIEW_H

#include <QWidget>
class QCustomPlot;

namespace Ui
{
class SimpleSignalView;
}

class SimpleSignalView : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleSignalView(QWidget* parent = nullptr);
    ~SimpleSignalView();
    void addData(double value, double time);
    void setValueRanges(double max, double min);

private slots:
    void on_checkBox_clicked();

private:
    Ui::SimpleSignalView* ui;
    QCustomPlot* _pPlot;
    QVector<double> _timeData;
    QVector<double> _valueData;
};

#endif // SIMPLESIGNALVIEW_H
