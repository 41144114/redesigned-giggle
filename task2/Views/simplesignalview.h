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
    void setLimit(double upLimit, double lowLimit, bool isUp, bool isLow);

private slots:
    void on_checkBox_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBoxColor_currentIndexChanged(int index);

    void on_checkBox_2_clicked();

private:
    Ui::SimpleSignalView* ui;
    QCustomPlot* _pPlot;
    QVector<double> _timeData;
    QVector<double> _valueData;
    QVector<double> _lowData;
    QVector<double> _upData;

    QVector<double> _offsetData;
    QVector<double> _offsetTime;

    bool _isUpLimit;
    bool _isLowLimit;
    double _upLimit;
    double _lowLimit;
    void setLineType();
};

#endif // SIMPLESIGNALVIEW_H
