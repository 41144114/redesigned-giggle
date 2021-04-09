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

    ui->frame->layout()->addWidget(_pPlot);
    repaint();
}

SimpleSignalView::~SimpleSignalView() { delete ui; }

void SimpleSignalView::addData(double value, double time)
{
    _timeData.append(time);
    _valueData.append(value);
    while (_timeData.last() - _timeData.first() >= ui->spinBox->value())
    {
        _timeData.takeFirst();
        _valueData.takeFirst();
    }

    _pPlot->graph(0)->setData(_timeData, _valueData);
    _pPlot->xAxis->setRange(_timeData.first(), _timeData.last());
    _pPlot->replot();
}

void SimpleSignalView::on_checkBox_clicked()
{
    _pPlot->graph(1)->setVisible(ui->checkBox->isChecked());
    _pPlot->graph(2)->setVisible(ui->checkBox->isChecked());

    _pPlot->replot();
}

void SimpleSignalView::setValueRanges(double max, double min)
{
    _timeData.clear();
    _valueData.clear();
    _pPlot->yAxis->setRange(max, min);
    _pPlot->replot();
}
