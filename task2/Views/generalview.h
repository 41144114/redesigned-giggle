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
    void clearData();

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_comboBoxSignal1Color_currentIndexChanged(int index);

    void on_comboBoxSignal2Color_currentIndexChanged(int index);

    void on_comboBoxSignal3Color_currentIndexChanged(int index);

    void on_comboBoxSignal4Color_currentIndexChanged(int index);

    void on_checkBoxSignal1_clicked();

    void on_checkBoxSignal2_clicked();

    void on_checkBoxSignal3_clicked();

    void on_checkBoxSignal4_clicked();

private:
    Ui::GeneralView* ui;
    QCustomPlot* _pPlot;
    QVector<double> _timeData;
    QVector<double> _data1;
    QVector<double> _data2;
    QVector<double> _data3;
    QVector<double> _data4;
};

#endif // GENERALVIEW_H
