#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <qcustomplot.h>
#include <QPen>
#include <QColor>
#include <QPixmap>
#include <QClipboard>

const double pi = 3.1415926535897932384626433832795;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot(this);
    ui->plotLayout->addWidget(_pPlot);
    _updateTimer = new QTimer();                        //Выделили память под объект таймера
    _updateTimer->setSingleShot(false);                 //Таймауты таймера теперь случаются периодически а не один раз
    connect(_updateTimer, &QTimer::timeout, this, &MainWindow::update); //Присоединили сигнал окончания таймера
                                                                        //к слоту обновления графика (проведения испытания

    //Задали шрифт подписей осей
    _pPlot->xAxis->setTickLabelFont(QFont("Times", 12));
    _pPlot->yAxis->setTickLabelFont(QFont("Times", 12));
    _pPlot->xAxis->setLabelFont(QFont("Times", 12));
    _pPlot->yAxis->setLabelFont(QFont("Times", 12));

    //Добили подписи осей графика
    _pPlot->xAxis->setLabel("Значение случайной величины");
    _pPlot->yAxis->setLabel("Процент испытаний в котором\nполучено данное значение");

    //Задали максимальное и минимальное значение, которого может достигнуть график вдоль оси Y
    _pPlot->yAxis->setRange(100.0, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}



//Функция, которая выключает/включает некоторую функциональность
//внутри окна, чтобы во время испытаний у пользователя
//не было возможности менять настройки
void MainWindow::switchMode(bool isWorking)
{
    ui->pushButtonFinish->setEnabled(isWorking);
    ui->pushButtonStart->setEnabled(!isWorking);
    ui->pushButtonClean->setEnabled(!isWorking);
    ui->spinBoxNRandomValues->setEnabled(!isWorking);
    ui->spinBoxNumTests->setEnabled(!isWorking);
    ui->doubleSpinBoxDelay->setEnabled(!isWorking);
    ui->doubleSpinBoxProbability->setEnabled(!isWorking);
    ui->label_2->setEnabled(!isWorking);
    ui->label->setEnabled(!isWorking);
    ui->label_3->setEnabled(!isWorking);
    ui->label_4->setEnabled(!isWorking);
}

void MainWindow::update()
{
    if(_nTestsReady < _nTests)  //Если провели испытаний меньше чем задано
    {

    }
    else
    {
        on_pushButtonFinish_clicked();
    }
}

void MainWindow::countGauss()
{
    double n = ui->spinBoxNRandomValues->value();
    double p = ui->doubleSpinBoxProbability->value();
    double mo = n*p;
    double D = n * p * (1 - p);
    double Amp = 1 / sqrt(D*2*pi)*100;

    _xGaussValues.clear();
    _yGaussValues.clear();
    for(int i = 0; i < n*10; ++i) //Рассчитаем аппроксимирующий гауссиан с шагом 0.1, чтоб поплавнее был
    {
        _xGaussValues.append(static_cast<double>(i)/10);
        _yGaussValues.append(Amp * exp(-(static_cast<double>(i)/10 - mo) * (static_cast<double>(i)/10 - mo) / (2*D)));
    }
}


//================= Слоты от оконных элементов =================================================
void MainWindow::on_pushButtonStart_clicked()
{
    qDebug() << "in start";
    switchMode(true);   //Заблокировали/разблокировали поля окошка
    _nTestsReady = 0;   //Инициализировали переменную, показывающую сколько испытаний прошло

    //Вот это так то не обязательно, просто удобнее, чтобы не из окна постоянно эти параметры вытаскивать, а чтоб в
    //переменных-членах клаасса лежали.
    _curProbability = ui->doubleSpinBoxProbability->value();
    _nTests = ui->spinBoxNumTests->value();
    _nRandomValues = ui->spinBoxNRandomValues->value();

    _pPlot->xAxis->setRange(_nRandomValues, 0.0);
    _pPlot->xAxis->rescale();
    _pPlot->repaint();
    _updateTimer->start(ui->doubleSpinBoxDelay->value()*1000);  //Запустили таймер
}

void MainWindow::on_pushButtonFinish_clicked()
{
    switchMode(false);
    _updateTimer->stop();
}

void MainWindow::on_comboBoxGraphShape_currentIndexChanged(int index)
{

}

void MainWindow::on_checkBoxGaussApproximation_clicked()
{
    if(ui->checkBoxGaussApproximation->isChecked())
    {
        countGauss();
        QPointer<QCPGraph> mGraph1;
        mGraph1 = _pPlot->addGraph(_pPlot->xAxis, _pPlot->axisRect()->axis(QCPAxis::atRight, 0));
        QPen pen;
        pen.setWidthF(2.0);
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::blue);

        _pPlot->graph(0)->setPen(pen);
        _pPlot->graph(0)->setData(_xGaussValues, _yGaussValues);

        double max = _yGaussValues.at(0);
        double min = _yGaussValues.at(0);
        for(int i = 1; i < _yGaussValues.count(); ++i)
        {
            if(_yGaussValues.at(i) > max) max = _yGaussValues.at(i);
            if(_yGaussValues.at(i) < min) min = _yGaussValues.at(i);
        }
        _pPlot->yAxis->setRange(max, min);
        _pPlot->graph(0)->rescaleValueAxis(false, true);

        _pPlot->replot();
    }
}

void MainWindow::on_pushButtonClean_clicked()
{

}

void MainWindow::on_spinBoxNRandomValues_valueChanged(int arg1)
{
    _pPlot->xAxis->setRange(0.0, arg1);
    _pPlot->replot();
}
