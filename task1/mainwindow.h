#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
class QCustomPlot;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonStart_clicked();
    void on_pushButtonFinish_clicked();
    void on_comboBoxGraphShape_currentIndexChanged(int index);
    void on_checkBoxGaussApproximation_clicked();
    void update();

    void on_pushButtonClean_clicked();

    void on_spinBoxNRandomValues_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QCustomPlot* _pPlot;
    QTimer* _updateTimer;
    QPen* _mainGaphPen;
    QPen* _approxGraphPen;
    int _curProbabilityBarrier; //Параметр p
    int _nTests;            //Параметр N
    int _nRandomValues;     //Параметр n
    int _nTestsReady;       //Количество пройденых испытаний

    QVector<double> _xValues;
    QVector<double> _yValues;
    QVector<double> _xGaussValues;
    QVector<double> _yGaussValues;


    void switchMode(bool isWorking);
    void countGauss();
    void resizeValueAxis();
    int getRandomValue();
};

#endif // MAINWINDOW_H
