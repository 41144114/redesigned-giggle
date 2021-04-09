#include "generalview.h"
#include "ui_generalview.h"

#include "qcustomplot.h"

const int indexGraph1 = 0;
const int indexGraph2 = 1;
const int indexGraph3 = 2;
const int indexGraph4 = 3;

GeneralView::GeneralView(QWidget* parent) : QWidget(parent), ui(new Ui::GeneralView)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot(this); //Создали объект графика

    //Добавили графики
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);

    ui->plotLayout->addWidget(_pPlot);
}

GeneralView::~GeneralView() { delete ui; }

void GeneralView::closeEvent(QCloseEvent* event) { emit closed(); }

void GeneralView::updateSignals(double val1, double val2, double val3, double val4, double time) {}
