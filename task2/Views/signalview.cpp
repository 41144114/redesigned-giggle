#include "signalview.h"
#include "ui_signalview.h"

#include "qcustomplot.h"

SignalView::SignalView(QMdiSubWindow *parent) :
    QMdiSubWindow(parent),
    ui(new Ui::SignalView)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot();
    ui->plotLayout->addWidget(_pPlot);
}

SignalView::~SignalView()
{
    delete ui;
}
