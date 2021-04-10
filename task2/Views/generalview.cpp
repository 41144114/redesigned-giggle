#include "generalview.h"
#include "ui_generalview.h"

#include "qcustomplot.h"

const int indexGraph1 = 2;
const int indexGraph2 = 3;
const int indexGraph3 = 0;
const int indexGraph4 = 1;

GeneralView::GeneralView(QWidget* parent) : QWidget(parent), ui(new Ui::GeneralView)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot(this); //Создали объект графика

    //Добавили графики
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//3
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//4
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//1
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//2

    _pPlot->yAxis->setRange(-150, 150);

    ui->plotLayout->addWidget(_pPlot);

    on_comboBoxSignal1Color_currentIndexChanged(ui->comboBoxSignal1Color->currentIndex());
    on_comboBoxSignal2Color_currentIndexChanged(ui->comboBoxSignal2Color->currentIndex());
    on_comboBoxSignal3Color_currentIndexChanged(ui->comboBoxSignal3Color->currentIndex());
    on_comboBoxSignal4Color_currentIndexChanged(ui->comboBoxSignal4Color->currentIndex());

    on_checkBoxSignal1_clicked();
    on_checkBoxSignal2_clicked();
    on_checkBoxSignal3_clicked();
    on_checkBoxSignal4_clicked();
}

GeneralView::~GeneralView() { delete ui; }

void GeneralView::closeEvent(QCloseEvent* event) { emit closed(); }

void GeneralView::updateSignals(double val1, double val2, double val3, double val4, double time)
{
    _timeData.append(time);
    _data1.append(val1);
    _data2.append(val2);
    _data3.append(val3);
    _data4.append(val4);
    while (_timeData.last() - _timeData.first() >= ui->spinBox->value())
    {
        _timeData.takeFirst();
        _data1.takeFirst();
        _data2.takeFirst();
        _data3.takeFirst();
        _data4.takeFirst();
    }

    _pPlot->graph(indexGraph1)->setData(_timeData, _data1);
    _pPlot->graph(indexGraph2)->setData(_timeData, _data2);
    _pPlot->graph(indexGraph3)->setData(_timeData, _data3);
    _pPlot->graph(indexGraph4)->setData(_timeData, _data4);
    _pPlot->xAxis->setRange(_timeData.first(), _timeData.last());
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal1Color_currentIndexChanged(int index)
{
    QPen pen;
    switch(index)
    {
        case 0: pen.setColor(Qt::black); break;
        case 1: pen.setColor(Qt::blue); break;
        case 2: pen.setColor(Qt::red); break;
        case 3: pen.setColor(Qt::green); break;
        case 4: pen.setColor(Qt::yellow); break;
    }
    _pPlot->graph(indexGraph1)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal2Color_currentIndexChanged(int index)
{
    QPen pen;
    switch(index)
    {
        case 0: pen.setColor(Qt::black); break;
        case 1: pen.setColor(Qt::blue); break;
        case 2: pen.setColor(Qt::red); break;
        case 3: pen.setColor(Qt::green); break;
        case 4: pen.setColor(Qt::yellow); break;
    }
    _pPlot->graph(indexGraph2)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal3Color_currentIndexChanged(int index)
{
    QPen pen;
    switch(index)
    {
        case 0: pen.setColor(Qt::black); break;
        case 1: pen.setColor(Qt::blue); break;
        case 2: pen.setColor(Qt::red); break;
        case 3: pen.setColor(Qt::green); break;
        case 4: pen.setColor(Qt::yellow); break;
    }
    _pPlot->graph(indexGraph3)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal4Color_currentIndexChanged(int index)
{
    QPen pen;
    switch(index)
    {
        case 0: pen.setColor(Qt::black); break;
        case 1: pen.setColor(Qt::blue); break;
        case 2: pen.setColor(Qt::red); break;
        case 3: pen.setColor(Qt::green); break;
        case 4: pen.setColor(Qt::yellow); break;
    }
    _pPlot->graph(indexGraph4)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_checkBoxSignal1_clicked()
{
    _pPlot->graph(indexGraph1)->setVisible(ui->checkBoxSignal1->isChecked());
}

void GeneralView::on_checkBoxSignal2_clicked()
{
    _pPlot->graph(indexGraph2)->setVisible(ui->checkBoxSignal2->isChecked());
}

void GeneralView::on_checkBoxSignal3_clicked()
{
     _pPlot->graph(indexGraph3)->setVisible(ui->checkBoxSignal3->isChecked());
}

void GeneralView::on_checkBoxSignal4_clicked()
{
     _pPlot->graph(indexGraph4)->setVisible(ui->checkBoxSignal4->isChecked());
}
