#include "signalview.h"
#include "ui_signalview.h"

#include "simplesignalview.h"
#include <QDebug>

SignalView::SignalView(QWidget* parent) : QWidget(parent), ui(new Ui::SignalView)
{
    ui->setupUi(this);
    _isFailure = false;
    _failurePixmap = new QPixmap(":/failure.png");
    _goodPixmap = new QPixmap(":/good.png");
    _pGraph = new SimpleSignalView();
    ui->plotLayout->addWidget(_pGraph);
    repaint();
}

SignalView::~SignalView() { delete ui; }

void SignalView::setType(int type) { _type = type; }

void SignalView::closeEvent(QCloseEvent* event) { emit closed(); }

void SignalView::updateSignal(double val1, double val2, double val3, double val4, double time)
{
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

void SignalView::checkLimits(double val)
{
    bool lastIsFailure = _isFailure;
    if (_type == 1)
    {
        _isFailure = val < _upLimit ? false : true;
        if (_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();
            emit showStatus("Превышение порога (сигнал 1)", 1);
        }
    }
    else if (_type == 2)
    {
        _isFailure = val > _upLimit ? false : true;
        if (_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();
            emit showStatus("Занижение порога (сигнал 2)", 2);
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
            qApp->beep();
            emit showStatus("Авария (сигнал 3)", 3);
        }
    }
    else
    {
        _isFailure = (val < _upLimit && val > _lowLimit) ? false : true;

        if(_isFailure && ui->checkBox->isChecked() && lastIsFailure == false)
        {
            qApp->beep();
            if (val > _upLimit)
                emit showStatus("Превышение порога (сигнал 4)", 4);
            else if (val < _lowLimit)
                emit showStatus("Занижение порога (сигнал 4)", 4);
        }
    }

    if (lastIsFailure != _isFailure)
        ui->label->setPixmap(_isFailure ? *_failurePixmap : *_goodPixmap);
}
