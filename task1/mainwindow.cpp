#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <qcustomplot.h>
#include <QPen>
#include <QColor>
#include <QPixmap>
#include <QClipboard>

const double pi = 3.1415926535897932384626433832795;
const int mainGraphIndex = 0;
const int approxGraphIndex = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pPlot = new QCustomPlot(this); //Создали объект графика
    _pBarPlotObj = new QCPBars(_pPlot->xAxis, _pPlot->yAxis);   //Создали объект для столбчатой диаграммы (прицепили его к осям графика
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//Добавили график основной
    _pPlot->addGraph(_pPlot->xAxis, _pPlot->yAxis);//Добавили аппроксимирующий график

    ui->plotLayout->addWidget(_pPlot);  //Закинули виджет графика в окно, теперь он будет отбражаться
    _updateTimer = new QTimer();                        //Выделили память под объект таймера
    _updateTimer->setSingleShot(false);                 //Таймауты таймера теперь случаются периодически а не один раз
    connect(_updateTimer, &QTimer::timeout, this, &MainWindow::update); //Присоединили сигнал окончания таймера
                                                                        //к слоту обновления графика (проведения испытания

    //Задали шрифт подписей осей
    _pPlot->xAxis->setTickLabelFont(QFont("Times", 12));
    _pPlot->yAxis->setTickLabelFont(QFont("Times", 12));
    _pPlot->xAxis->setLabelFont(QFont("Times", 12));
    _pPlot->yAxis->setLabelFont(QFont("Times", 12));

    //Добавили подписи осей графика
    _pPlot->xAxis->setLabel("Значение случайной величины");
    _pPlot->yAxis->setLabel("Процент испытаний в котором\nполучено данное значение");

    //Задали максимальное и минимальное значение, которого может достигнуть график вдоль оси Y
    _pPlot->yAxis->setRange(100.0, 0.0);

    //Настроили цвет и тип линии графика
    QPen mainGraphPen;
    QPen approxGraphPen;

        // --//-- для аппроксимирующего графика
    approxGraphPen.setWidthF(2.0);
    approxGraphPen.setStyle(Qt::SolidLine);
    approxGraphPen.setColor(Qt::gray);
    _pPlot->graph(approxGraphIndex)->setPen(approxGraphPen);

        // --//-- для основного графика
    mainGraphPen.setWidthF(2.0);
    mainGraphPen.setStyle(Qt::SolidLine);
    mainGraphPen.setColor(Qt::black);
    _pPlot->graph(mainGraphIndex)->setPen(mainGraphPen);
    _pBarPlotObj->setPen(mainGraphPen);
    _pBarPlotObj->setBrush(Qt::black);

    //Показали, что испытаний пока не проведено
    ui->labelNTests->setText("0");
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
    //Заблокировали/разблокировали некоторые элементы управления в окне
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

void MainWindow::update()   //Здесь проводим испытания и обновляем график. Функция вызывается по тику таймера
{
    if(_nTestsReady < _nTests)  //Если провели испытаний меньше чем задано
    {
        _nTestsReady++;
        int y = getRandomValue();   //Сгенерировали случайную величину
        _yValues[y]++;  //инкрементировали счётчик выпадения данного значения случайной величины
        for(int i = 0; i < _nRandomValues; ++i)
            _yValuesForView[i] = static_cast<double>(_yValues.at(i) * 100) / _nTestsReady;  //Пересчитали процент выпадения для всех значений


        _pPlot->graph(mainGraphIndex)->setData(_xValues, _yValuesForView);  //Закинули данные в линейный график
        _pBarPlotObj->setData(_xValues, _yValuesForView);   //Закинули значения в столбчатую диаграмму

        resizeValueAxis();  //Пересмотрели размер оси

        //Определяем какой график отображать
        if(ui->comboBoxGraphShape->currentIndex() == 0)
        {
            _pPlot->graph(mainGraphIndex)->setVisible(true);
            _pBarPlotObj->setVisible(false);
        }
        else
        {
            _pPlot->graph(mainGraphIndex)->setVisible(false);
            _pBarPlotObj->setVisible(true);
        }

        _pPlot->replot();//Обновляем виджет графика
        ui->labelNTests->setText(QString::number(_nTestsReady));    //Отобразили количество проделанных испытаний
    }
    else
    {
        on_pushButtonFinish_clicked();  //Закончили)
    }
}

void MainWindow::countGauss()
{
    //Посчитали параметры распределения
    double n = ui->spinBoxNRandomValues->value();
    double p = ui->doubleSpinBoxProbability->value();
    double mo = n*p;
    double D = n * p * (1 - p);
    double Amp = 1 / sqrt(D*2*pi)*100;

    _xGaussValues.clear();
    _yGaussValues.clear();
    for(int i = 0; i < n*10; ++i) //Рассчитаем аппроксимирующий гауссиан с шагом 0.1, чтоб поплавнее был плавнее
    {
        _xGaussValues.append(static_cast<double>(i)/10);    //Добавляем узлы в которых известно значение графика
        _yGaussValues.append(Amp * exp(-(static_cast<double>(i)/10 - mo) * (static_cast<double>(i)/10 - mo) / (2*D)));//считаем значения графика по формуле
    }
}

void MainWindow::resizeValueAxis()
{
    double max = 0;//Находим максимум:
    if(_yValuesForView.isEmpty() == false) //Ну, вдруг тут нет значений, надо проверить
    {
        for(int i = 0; i < _yValuesForView.count(); ++i)    //Пробегаемся по всем значениям сгенерированным
            if(max < _yValuesForView.at(i))
                max = _yValuesForView.at(i);
    }

    if(ui->checkBoxGaussApproximation->isChecked()) //Если у нас отображается гауссиан, его тоже надо брать в учёт
    {
        for(int i = 1; i < _yGaussValues.count(); ++i)  //Пробегаем по всем значениям гауссиана и ищем максимум
            if(max < _yGaussValues.at(i))
                max = _yGaussValues.at(i);
    }
    _pPlot->yAxis->setRange(0.0, max*1.1); //Устанавливаем максиум по оси чуть выше максимально высокой точки графиков
    _pPlot->replot();   //Обновляем виджет графика
}

int MainWindow::getRandomValue()
{
    int ret = 0;    //Возвращаемое значение - значение случайной величины распределения
    int x;      //Одна из составляющих
    for(int i = 0; i < _nRandomValues; ++i) //Проводим n испытаний
    {
        x = qrand() % 100;  //Генерируем равномерно распределённое случайное число в диапазоне 0..99
        if(x < _curProbabilityBarrier)  //Если число меньше чем заданый барьер (определяется вероятностью p)
            ret++;  //Прибавляем единицу
    }
    return ret;
}

//================= Слоты от оконных элементов =================================================
void MainWindow::on_pushButtonStart_clicked()
{
    switchMode(true);   //Заблокировали/разблокировали поля окошка
    _nTestsReady = 0;   //Инициализировали переменную, показывающую сколько испытаний прошло

    //Вот это так то не обязательно, просто удобнее, чтобы не из окна постоянно эти параметры вытаскивать, а чтоб в
    //переменных-членах клаасса лежали.
    _curProbabilityBarrier = ui->doubleSpinBoxProbability->value()*100;
    _nTests = ui->spinBoxNumTests->value();
    _nRandomValues = ui->spinBoxNRandomValues->value();

    on_pushButtonClean_clicked();   //Чистим что там было до текущих испытаний

    update();
    _updateTimer->start(ui->doubleSpinBoxDelay->value()*1000);  //Запустили таймер
}

void MainWindow::on_pushButtonFinish_clicked()
{
    _updateTimer->stop();   //Остановили таймер, чтобы испытания прекратились и график больше не изменялся
    switchMode(false);      //Перешли в нужный резим окна
}

void MainWindow::on_comboBoxGraphShape_currentIndexChanged(int index)
{
    //Тут меняем вид графика. Так то они оба строятся, но в один и тот же момент показывается только один, а второй скрыт
    if(ui->comboBoxGraphShape->currentIndex() == 0)
    {
        _pPlot->graph(mainGraphIndex)->setVisible(true);
        _pBarPlotObj->setVisible(false);
    }
    else
    {
        _pPlot->graph(mainGraphIndex)->setVisible(false);
        _pBarPlotObj->setVisible(true);
    }
    _pPlot->replot();//Обновляем виджет графика
}

void MainWindow::on_checkBoxGaussApproximation_clicked()
{
    if(ui->checkBoxGaussApproximation->isChecked())
    {
        countGauss();   //Рассчитываем аппроксимирующий график (получаем данные)

        _pPlot->graph(approxGraphIndex)->setData(_xGaussValues, _yGaussValues);//Закидываем данные в график

        resizeValueAxis();  //Перестроили ось Y, чтобы она была подходящего масштаба и ничего лишнего не отображала
        _pPlot->graph(approxGraphIndex)->setVisible(true);
        _pPlot->replot();//Обновляем виджет графика
    }
    else
    {
        //Просто скрываем график, раз он не нужен
        _pPlot->graph(approxGraphIndex)->setVisible(false);
        _pPlot->replot();//Обновляем виджет графика
    }
}

void MainWindow::on_pushButtonClean_clicked()
{    
    _xValues.resize(_nRandomValues + 1);//Нужно, чтобы мы могли впихнуть в массив элемент с индексом
    _yValues.resize(_nRandomValues + 1);//_nRandomValues, так как это максимально возможное
    _yValuesForView.resize(_nRandomValues + 1);//значение случайной величины.

    for(int i = 0; i < _nRandomValues; ++i) //Заполняем массивы значенями, справедливыми на момент начала испытаний
    {
        _xValues[i] = i;    //Задаём все возможные значения случайной величины
        _yValues[i] = 0;    //У нас не было ещё испытаний, поэтому каждое значение случайная величина принимало ровно 0 раз
        _yValuesForView[i] = 0.0;   //ТО же, что и на строчку выше, но в процентах
    }

    //Скрываем графики
    _pPlot->graph(mainGraphIndex)->setVisible(false);
    _pBarPlotObj->setVisible(false);
    _pPlot->replot();//Обновляем виджет графика

    //Показываем, что теперь на графике отображается результат нуля испытаний
    ui->labelNTests->setText("0");
}

void MainWindow::on_spinBoxNRandomValues_valueChanged(int arg1)
{
    //При изменении параметра n меняем ось x графика, так как там должны отображаться все возможные значения случайной величины
    //Минимальное значение случайной величины - 0, максимальное равно n, очевидно.
    _pPlot->xAxis->setRange(0.0, arg1);
    _pPlot->replot();//Обновляем виджет графика
}
