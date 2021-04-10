#include "simplesignalview.h"
#include "ui_simplesignalview.h"

#include "qcustomplot.h"

SimpleSignalView::SimpleSignalView(QWidget* parent) : QWidget(parent), ui(new Ui::SimpleSignalView)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot();
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //Основной график

    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //Пределы
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);

    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //Cмещённый сигнал

    _pPlot->graph(1)->setVisible(false);
    _pPlot->graph(2)->setVisible(false);
    _pPlot->graph(3)->setVisible(false);

    ui->frame->layout()->addWidget(_pPlot);
    setLineType();
}

SimpleSignalView::~SimpleSignalView() { delete ui; }

void SimpleSignalView::addData(double value, double time)
{
    _timeData.append(time);
    _valueData.append(value);
    _lowData.append(_lowLimit);
    _upData.append(_upLimit);
    while (_timeData.last() - _timeData.first() >= ui->spinBox->value())
    {
        _timeData.takeFirst();
        _valueData.takeFirst();
        _lowData.takeFirst();
        _upData.takeFirst();
    }

    if(_timeData.count() > ui->spinBox_2->value())
    {
        _offsetData = _valueData;
        _offsetTime = _timeData;
        for(int i = 0; i < ui->spinBox_2->value(); ++i)
        {
            _offsetData.takeLast();
            _offsetTime.takeFirst();
        }
    }

    _pPlot->graph(0)->setData(_timeData, _valueData);
    _pPlot->graph(1)->setData(_timeData, _upData);
    _pPlot->graph(2)->setData(_timeData, _lowData);
    _pPlot->graph(3)->setData(_offsetTime, _offsetData);
    _pPlot->xAxis->setRange(_timeData.first(), _timeData.last());
    _pPlot->replot();
}

void SimpleSignalView::on_checkBox_clicked()
{

    if(_isUpLimit) _pPlot->graph(1)->setVisible(ui->checkBox->isChecked());
    if(_isLowLimit) _pPlot->graph(2)->setVisible(ui->checkBox->isChecked());

    _pPlot->replot();
}

void SimpleSignalView::setValueRanges(double max, double min)
{
    _timeData.clear();
    _valueData.clear();
    _pPlot->yAxis->setRange(max, min);
    _pPlot->replot();
}

void SimpleSignalView::setLimit(double upLimit, double lowLimit, bool isUp, bool isLow)
{
    _lowLimit = lowLimit;
    _upLimit = upLimit;
    _isLowLimit = isLow;
    _isUpLimit = isUp;
}

void SimpleSignalView::setLineType()
{
    QPen pen;
    switch(ui->comboBoxColor->currentIndex())
    {
        case 0: pen.setColor(Qt::black);break;
        case 1: pen.setColor(Qt::blue);break;
        case 2: pen.setColor(Qt::red);break;
        case 3: pen.setColor(Qt::green);break;
        case 4: pen.setColor(Qt::yellow);break;
    }

    if(ui->comboBox->currentIndex() == 0)
        pen.setStyle(Qt::SolidLine);
    else
        pen.setStyle(Qt::DashLine);

    _pPlot->graph(0)->setPen(pen);
    _pPlot->replot();
}

void SimpleSignalView::on_comboBox_currentIndexChanged(int index)
{
    setLineType();
}

void SimpleSignalView::on_comboBoxColor_currentIndexChanged(int index)
{
    setLineType();
}

void SimpleSignalView::on_checkBox_2_clicked()
{
    _pPlot->graph(3)->setVisible(ui->checkBox_2->isChecked());
}
