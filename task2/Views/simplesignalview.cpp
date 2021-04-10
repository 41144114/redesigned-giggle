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

    _pPlot->graph(1)->setVisible(false);//Изначально смещённый график и пределы не видны
    _pPlot->graph(2)->setVisible(false);
    _pPlot->graph(3)->setVisible(false);

    ui->frame->layout()->addWidget(_pPlot); //Добавили график в окошко
    setLineType();//Установили тип линии в соответствии с информацией в окне
}

SimpleSignalView::~SimpleSignalView() { delete ui; }

void SimpleSignalView::addData(double value, double time)   //Тут всё самое интересное, тут рисуется график
{
    //Добавили данных во все ряды данных, которые есть (кроме смещённого)
    _timeData.append(time);
    _valueData.append(value);
    _lowData.append(_lowLimit);
    _upData.append(_upLimit);

    //Проверяем, если между начальной и конечной точкой времени у нас промежуток больше заданного окна
    //значит нужно выкинуть данные, которые пришли самыми ранними.
    while (_timeData.last() - _timeData.first() >= ui->spinBox->value())
    {
        _timeData.takeFirst();
        _valueData.takeFirst();
        _lowData.takeFirst();
        _upData.takeFirst();
    }

    //Если у нас есть достаточно данных, чтобы пришёл сигнал с задержкой
    if(_timeData.count() > ui->spinBox_2->value())
    {
        _offsetData = _valueData;   //Данные то те же, только их сместить надо будет
        _offsetTime = _timeData;
        for(int i = 0; i < ui->spinBox_2->value(); ++i) //Тут происходит смещение на нужное количество тактов.
        {
            _offsetTime.takeFirst();//Сместили по времени
            _offsetData.takeLast(); //У нас сигнал не опережает исходный, поэтому в конце точки будут лишними
        }
    }

    //Закинули данные в виджет для отображения
    _pPlot->graph(0)->setData(_timeData, _valueData);
    _pPlot->graph(1)->setData(_timeData, _upData);
    _pPlot->graph(2)->setData(_timeData, _lowData);
    _pPlot->graph(3)->setData(_offsetTime, _offsetData);

    //Ресайзим ось X
    _pPlot->xAxis->setRange(_timeData.first(), _timeData.last());
    //Перерисовываем
    _pPlot->replot();
}

void SimpleSignalView::on_checkBox_clicked()    //Вкл/выкл пределы
{
    if(_isUpLimit) _pPlot->graph(1)->setVisible(ui->checkBox->isChecked());
    if(_isLowLimit) _pPlot->graph(2)->setVisible(ui->checkBox->isChecked());

    _pPlot->replot();
}

void SimpleSignalView::setValueRanges(double max, double min)
{
    //Ресайзим ось Y, попутно очищаем график, так как если мы сюда зашли, то значит настройки поменялись
    _timeData.clear();
    _valueData.clear();
    _pPlot->yAxis->setRange(max, min);
    _pPlot->replot();
}

void SimpleSignalView::setLimit(double upLimit, double lowLimit, bool isUp, bool isLow)
{
    //Просто устанавливаем лимиты и флаги, нужны они или нет
    _lowLimit = lowLimit;
    _upLimit = upLimit;
    _isLowLimit = isLow;
    _isUpLimit = isUp;
}

void SimpleSignalView::setLineType()
{
    QPen pen;
    //Определяем и устанавливаем нужный цвет
    switch(ui->comboBoxColor->currentIndex())
    {
        case 0: pen.setColor(Qt::black);break;
        case 1: pen.setColor(Qt::blue);break;
        case 2: pen.setColor(Qt::red);break;
        case 3: pen.setColor(Qt::green);break;
        case 4: pen.setColor(Qt::yellow);break;
    }

    //Определяем и устанавливаем нужный тип линии
    if(ui->comboBox->currentIndex() == 0)
        pen.setStyle(Qt::SolidLine);
    else
        pen.setStyle(Qt::DashLine);

    //Применяем ручку к графику
    _pPlot->graph(0)->setPen(pen);

    //Перерисовываем всё
    _pPlot->replot();
}

void SimpleSignalView::on_comboBox_currentIndexChanged(int index)   //Реакция на комбо-бокс
{                                                                   //Просто переходим в функцию где все делаем
    setLineType();
}

void SimpleSignalView::on_comboBoxColor_currentIndexChanged(int index)  //Реакция на комбо-бокс
{                                                                       //Просто переходим в функцию где все делаем
    setLineType();
}

void SimpleSignalView::on_checkBox_2_clicked()  //Вкл/выкл смещенного сигнала
{
    _pPlot->graph(3)->setVisible(ui->checkBox_2->isChecked());
    _pPlot->replot();
}
