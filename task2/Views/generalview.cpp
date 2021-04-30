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

    //Добавили графики для каждого сигнала
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //3
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //4
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //1
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis); //2

    _pPlot->yAxis->setRange(-150, 150);

    //Закинули объект графика в слой
    ui->plotLayout->addWidget(_pPlot);

    //Изначальная настройка графиков, чтобы их вид соответствовал тому, что указано в окне
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

void GeneralView::closeEvent(QCloseEvent* event)
{
    emit closed(); // Сообщили в главное окно, что мы тут так то закрылись уже.
}

void GeneralView::clearData()
{
    _timeData.clear();
    _data1.clear();
    _data2.clear();
    _data3.clear();
    _data4.clear();
}

void GeneralView::updateSignals(double val1, double val2, double val3, double val4, double time)
{
    //Закинули данные в структуру вектора
    _timeData.append(time);
    _data1.append(val1);
    _data2.append(val2);
    _data3.append(val3);
    _data4.append(val4);

    //Если у нас данных больше, чем окно, надо убирать пришедшие вначале
    while (_timeData.last() - _timeData.first() >= ui->spinBox->value())
    {
        _timeData.takeFirst();
        _data1.takeFirst();
        _data2.takeFirst();
        _data3.takeFirst();
        _data4.takeFirst();
    }

    //Закинули данные в графики соответствующие
    _pPlot->graph(indexGraph1)->setData(_timeData, _data1);
    _pPlot->graph(indexGraph2)->setData(_timeData, _data2);
    _pPlot->graph(indexGraph3)->setData(_timeData, _data3);
    _pPlot->graph(indexGraph4)->setData(_timeData, _data4);

    //Переделали границы оси x
    _pPlot->xAxis->setRange(_timeData.first(), _timeData.last());

    //Перерисовали график
    _pPlot->replot();
}

//Дальше все функции по одному принципу работают:
void GeneralView::on_comboBoxSignal1Color_currentIndexChanged(int index)
{
    QPen pen;
    switch (index) //Посмотрели какой цвет желает юзер
    {              //Установили цвет в объект pen
        case 0:
            pen.setColor(Qt::black);
            break;
        case 1:
            pen.setColor(Qt::blue);
            break;
        case 2:
            pen.setColor(Qt::red);
            break;
        case 3:
            pen.setColor(Qt::green);
            break;
        case 4:
            pen.setColor(Qt::yellow);
            break;
    }

    //Теперь нужный график рисуем нужной ручкой
    _pPlot->graph(indexGraph1)->setPen(pen);

    //Перерисовали всё
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal2Color_currentIndexChanged(int index)
{
    QPen pen;
    switch (index)
    {
        case 0:
            pen.setColor(Qt::black);
            break;
        case 1:
            pen.setColor(Qt::blue);
            break;
        case 2:
            pen.setColor(Qt::red);
            break;
        case 3:
            pen.setColor(Qt::green);
            break;
        case 4:
            pen.setColor(Qt::yellow);
            break;
    }
    _pPlot->graph(indexGraph2)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal3Color_currentIndexChanged(int index)
{
    QPen pen;
    switch (index)
    {
        case 0:
            pen.setColor(Qt::black);
            break;
        case 1:
            pen.setColor(Qt::blue);
            break;
        case 2:
            pen.setColor(Qt::red);
            break;
        case 3:
            pen.setColor(Qt::green);
            break;
        case 4:
            pen.setColor(Qt::yellow);
            break;
    }
    _pPlot->graph(indexGraph3)->setPen(pen);
    _pPlot->replot();
}

void GeneralView::on_comboBoxSignal4Color_currentIndexChanged(int index)
{
    QPen pen;
    switch (index)
    {
        case 0:
            pen.setColor(Qt::black);
            break;
        case 1:
            pen.setColor(Qt::blue);
            break;
        case 2:
            pen.setColor(Qt::red);
            break;
        case 3:
            pen.setColor(Qt::green);
            break;
        case 4:
            pen.setColor(Qt::yellow);
            break;
    }
    _pPlot->graph(indexGraph4)->setPen(pen);
    _pPlot->replot();
}

//Тут похожая история, если галочка поставлена - график будет видимым, иначе - нет
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
