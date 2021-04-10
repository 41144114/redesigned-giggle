#include "signalview.h"
#include "ui_signalview.h"

#include "simplesignalview.h"
#include <QDebug>

SignalView::SignalView(QWidget* parent) : QWidget(parent), ui(new Ui::SignalView)
{
    ui->setupUi(this);
    _isFailure = false; //Пока не получили данных считаем что всё хорошо и не авария

    //Настроили иконки отображающие состояние
    _failurePixmap = new QPixmap(":/failure.png");
    _goodPixmap = new QPixmap(":/good.png");

    //Добавили виджет с графиком и его настройками
    _pGraph = new SimpleSignalView();
    ui->plotLayout->addWidget(_pGraph);

    //Перерисовали окно
    repaint();
}

SignalView::~SignalView() { delete ui; }

void SignalView::setType(int type)
{
    _type = type; //Указали номер графика
}

void SignalView::closeEvent(QCloseEvent* event)
{
    emit closed();  //Сообщили главному окну, что тут так то закрыто уже)
}

void SignalView::updateSignal(double val1, double val2, double val3, double val4, double time)
{
    //Смотрим какой номер у нашего графика и добавляем определённое значение к ряду данных
    //Проверяем превыщение пределов тоже здесь
    switch (_type)
    {
        case 1:
            checkLimits(val1);
            _pGraph->addData(val1, time);
            break;
        case 2:
            checkLimits(val2);
            _pGraph->addData(val2, time);
            break;
        case 3:
            checkLimits(val3);
            _pGraph->addData(val3, time);
            break;
        case 4:
            checkLimits(val4);
            _pGraph->addData(val4, time);
            break;
        default:
            break;
    }
}

void SignalView::setLimits(signalsLimits_t limits, signalsParams_t params)
{
    //Здесь определяем какие должны быть размеры у оси y на графике и задаем эти размеры в
    //соответствующий виджет, да ещё задаем пределы, чтоб и тоже можно было отрисовывать
    if (_type == 1)
    {
        _upLimit = limits.sinMax;
        _pGraph->setValueRanges(params.sinAmp > _upLimit ? params.sinAmp : _upLimit, -params.sinAmp);
        _pGraph->setLimit(_upLimit, _lowLimit, true, false);
    }
    else if (_type == 2)
    {
        _lowLimit = limits.traingleMin;
        _pGraph->setValueRanges(params.traingleMax, params.traingleMin < _lowLimit ? params.traingleMin : _lowLimit);
        _pGraph->setLimit(_upLimit, _lowLimit, false, true);
    }
    else if (_type == 3)
    {
        _upLimit = limits.randomMax;
        _lowLimit = limits.randomMin;
        double max = params.randomMo + 3 * params.randomSko;
        double min = params.randomMo - 3 * params.randomSko;
        max = max > _upLimit ? max : _upLimit;
        min = min < _lowLimit ? min : _lowLimit;
        _pGraph->setValueRanges(max, min);
        _pGraph->setLimit(_upLimit, _lowLimit, true, true);
    }
    else
    {
        _upLimit = limits.variantMax;
        _lowLimit = limits.variantMin;
        double max = params.variantMax;
        double min = params.variantMin;
        max = max > _upLimit ? max : _upLimit;
        min = min < _lowLimit ? min : _lowLimit;
        _pGraph->setValueRanges(max, min);
        _pGraph->setLimit(_upLimit, _lowLimit, true, true);
    }
}

void SignalView::checkLimits(double val)    //Проверяем пределы
{
    bool lastIsFailure = _isFailure;    //Сохранили прошлое состояние - пригодится.
    //Дальше проверяем в зависимости от номера графика
    //Определяем аварийное ли состояние
    //Если нужно, сигнализируем в главное окно, издаем звук.
    if (_type == 1)
    {
        _isFailure = val < _upLimit ? false : true;
        if (_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();   //Звуковой сигнал
            emit showStatus("Превышение порога (сигнал 1)", 1);//Сигналим в статус бар
        }
    }
    else if (_type == 2)
    {
        _isFailure = val > _upLimit ? false : true;
        if (_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();//Звуковой сигнал
            emit showStatus("Занижение порога (сигнал 2)", 2);//Сигналим в статус бар
        }
    }

    else if (_type == 3)
    {
        if (_isFailure == false)
            _isFailure = val < _upLimit ? false : true;
        else
            _isFailure = val < _lowLimit ? false : true;

        if (_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();//Звуковой сигнал
            emit showStatus("Авария (сигнал 3)", 3);//Сигналим в статус бар
        }
    }
    else
    {
        _isFailure = (val < _upLimit && val > _lowLimit) ? false : true;

        if(_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();//Звуковой сигнал
            if (val > _upLimit)
                emit showStatus("Превышение порога (сигнал 4)", 4);//Сигналим в статус бар
            else if (val < _lowLimit)
                emit showStatus("Занижение порога (сигнал 4)", 4);//Сигналим в статус бар
        }
    }

    if (lastIsFailure != _isFailure)//Если состояние сменилось
        ui->label->setPixmap(_isFailure ? *_failurePixmap : *_goodPixmap);  //Поставили либо зеленый либо красный индикатор
}
