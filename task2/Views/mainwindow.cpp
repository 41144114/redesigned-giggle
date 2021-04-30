#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QDebug>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QThread>
#include <QWindow>

#include "Buiseness/signalgenerator.h"
#include "generalview.h"
#include "settingswindow.h"
#include "signalview.h"

const QVector<int> priority = {2, 1, 3, 4};

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Мониторинг сигналов");

    allocateMemory();
    setupToolBar();
    setupMainMenu();
    setupSubWindows();
    setupConnections();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::allocateMemory()
{
    //Выделили память под все необходимые объекты
    _settingsWindow = new SettingsWindow();
    _signalView0 = new GeneralView();
    _signalView1 = new SignalView();
    _signalView2 = new SignalView();
    _signalView3 = new SignalView();
    _signalView4 = new SignalView();

    generator = new SignalGenerator();

    //Закинули в отдельный поток генератор сигналов
    QThread* signalGeneratorThread = new QThread();
    generator->moveToThread(signalGeneratorThread);
    connect(signalGeneratorThread, &QThread::started, generator, &SignalGenerator::startProcess);
    signalGeneratorThread->start();
}

void MainWindow::setupToolBar()
{
    //Добавили кнопок на панель инструментов
    _pb0 = new QPushButton();
    _pb1 = new QPushButton();
    _pb2 = new QPushButton();
    _pb3 = new QPushButton();
    _pb4 = new QPushButton();
    _pbOnOff = new QPushButton();
    _pbScreenShot = new QPushButton();

    //Установили шрифт для кнопок
    QFont font;
    font.setFamily("Century");
    font.setPointSize(12);
    _pb0->setFont(font);
    _pb1->setFont(font);
    _pb2->setFont(font);
    _pb3->setFont(font);
    _pb4->setFont(font);

    //Установили текст для кнопок
    _pb0->setText("0");
    _pb1->setText("1");
    _pb2->setText("2");
    _pb3->setText("3");
    _pb4->setText("4");

    //Сделали кнопки выбираемыми (постоянно может находится в нажатом положении)
    _pb0->setCheckable(true);
    _pb1->setCheckable(true);
    _pb2->setCheckable(true);
    _pb3->setCheckable(true);
    _pb4->setCheckable(true);
    _pbOnOff->setCheckable(true);

    //Добавили иконки на кнопки скриншота и включения
    _pbOnOff->setIcon(QIcon(":/onOffIcon.png"));
    _pbScreenShot->setIcon(QIcon(":/photo.png"));

    //Закинули кнопки на панель инструментов
    ui->mainToolBar->addWidget(_pbOnOff);
    ui->mainToolBar->addWidget(_pb0);
    ui->mainToolBar->addWidget(_pb1);
    ui->mainToolBar->addWidget(_pb2);
    ui->mainToolBar->addWidget(_pb3);
    ui->mainToolBar->addWidget(_pb4);
    ui->mainToolBar->addWidget(_pbScreenShot);

    //Настроили размеры кнопок
    _pb0->setMaximumHeight(20);
    _pb1->setMaximumHeight(20);
    _pb2->setMaximumHeight(20);
    _pb3->setMaximumHeight(20);
    _pb4->setMaximumHeight(20);
    _pbScreenShot->setMaximumHeight(20);
    _pbOnOff->setMaximumHeight(20);

    _pb0->setMaximumWidth(20);
    _pb1->setMaximumWidth(20);
    _pb2->setMaximumWidth(20);
    _pb3->setMaximumWidth(20);
    _pb4->setMaximumWidth(20);
    _pbScreenShot->setMaximumWidth(20);
    _pbOnOff->setMaximumWidth(20);

    //Присоединили слоты к кнопкам
    connect(_pb0, &QPushButton::clicked, this, &MainWindow::onPb0Click);
    connect(_pb1, &QPushButton::clicked, this, &MainWindow::onPb1Click);
    connect(_pb2, &QPushButton::clicked, this, &MainWindow::onPb2Click);
    connect(_pb3, &QPushButton::clicked, this, &MainWindow::onPb3Click);
    connect(_pb4, &QPushButton::clicked, this, &MainWindow::onPb4Click);
    connect(_pbOnOff, &QPushButton::clicked, this, &MainWindow::onPbOnOffClick);
    connect(_pbScreenShot, &QPushButton::clicked, this, &MainWindow::onScreenShotClick);
}

void MainWindow::setupMainMenu()
{
    //Присоединили слоты к пунктам меню
    connect(ui->action_Qt, &QAction::triggered, this, &MainWindow::onAboutQt);
    connect(ui->actionAboutProgramm, &QAction::triggered, this, &MainWindow::onAboutProgram);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onSetupSignals);
}

void MainWindow::setupSubWindows()
{
    //Установили внутри объектов их номера, чтоб они знали какие данные для них и всё такое
    _signalView1->setType(1);
    _signalView2->setType(2);
    _signalView3->setType(3);
    _signalView4->setType(4);
}

void MainWindow::setupConnections()
{
    //Прицепили сигналы закрытия окон к слотам, которые бы разблокировали соответствующие кнопки на панели и удаляли окно
    connect(_signalView0, &GeneralView::closed, [=](void) {
        _pb0->setChecked(false);
        ui->mdiArea->removeSubWindow(_signalView0);
        _subWindow0->close();
    });

    connect(_signalView1, &SignalView::closed, [=](void) {
        _pb1->setChecked(false);
        ui->mdiArea->removeSubWindow(_signalView1);
        _subWindow1->close();
    });

    connect(_signalView2, &SignalView::closed, [=](void) {
        _pb2->setChecked(false);
        ui->mdiArea->removeSubWindow(_signalView2);
        _subWindow2->close();
    });

    connect(_signalView3, &SignalView::closed, [=](void) {
        _pb3->setChecked(false);
        ui->mdiArea->removeSubWindow(_signalView3);
        _subWindow3->close();
    });

    connect(_signalView4, &SignalView::closed, [=](void) {
        _pb4->setChecked(false);
        ui->mdiArea->removeSubWindow(_signalView4);
        _subWindow4->close();
    });

    //Посоединяли различные объекты между собой. Чтоб сгенерированный сигнал доходил до интерфейса
    //а заданные настройки доходили до генератора сигналов
    connect(this, &MainWindow::signalStart, generator, &SignalGenerator::startGenerate);
    connect(this, &MainWindow::signalStart, _signalView0, &GeneralView::clearData);
    connect(this, &MainWindow::signalStop, generator, &SignalGenerator::stopGenerate);

    connect(generator, &SignalGenerator::updateSignals, _signalView1, &SignalView::updateSignal);
    connect(generator, &SignalGenerator::updateSignals, _signalView2, &SignalView::updateSignal);
    connect(generator, &SignalGenerator::updateSignals, _signalView3, &SignalView::updateSignal);
    connect(generator, &SignalGenerator::updateSignals, _signalView4, &SignalView::updateSignal);
    connect(generator, &SignalGenerator::updateSignals, _signalView0, &GeneralView::updateSignals);

    connect(_settingsWindow, &SettingsWindow::signalSetParameters, generator, &SignalGenerator::setParams);
    connect(_settingsWindow, &SettingsWindow::signalSetLimits, _signalView1, &SignalView::setLimits);
    connect(_settingsWindow, &SettingsWindow::signalSetLimits, _signalView2, &SignalView::setLimits);
    connect(_settingsWindow, &SettingsWindow::signalSetLimits, _signalView3, &SignalView::setLimits);
    connect(_settingsWindow, &SettingsWindow::signalSetLimits, _signalView4, &SignalView::setLimits);

    connect(_signalView1, &SignalView::showStatus, this, &MainWindow::onShowStatus);
    connect(_signalView2, &SignalView::showStatus, this, &MainWindow::onShowStatus);
    connect(_signalView3, &SignalView::showStatus, this, &MainWindow::onShowStatus);
    connect(_signalView4, &SignalView::showStatus, this, &MainWindow::onShowStatus);
}
//================ Кнопки открытия/закрытия суб окон ====================================
void MainWindow::onPb0Click()
{
    if (_pb0->isChecked())
    {
        _subWindow0 = ui->mdiArea->addSubWindow(_signalView0); //Добавили виджет в суб окно
        _subWindow0->setWindowTitle("Все сигналы");            //Настроили заголовок суб окна
        _subWindow0->show();                                   //Показали суб окно
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView0); //Удалили виджет из суб окна
        _subWindow0->close();                       //Закрыли суб окно
    }
}

void MainWindow::onPb1Click()
{
    if (_pb1->isChecked())
    {
        _subWindow1 = ui->mdiArea->addSubWindow(_signalView1);
        _subWindow1->setWindowTitle("Сигнал 1");
        _subWindow1->show();
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView1);
        _subWindow1->close();
    }
}

void MainWindow::onPb2Click()
{
    if (_pb2->isChecked())
    {
        _subWindow2 = ui->mdiArea->addSubWindow(_signalView2);
        _subWindow2->setWindowTitle("Сигнал 2");
        _subWindow2->show();
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView2);
        _subWindow2->close();
    }
}

void MainWindow::onPb3Click()
{
    if (_pb3->isChecked())
    {
        _subWindow3 = ui->mdiArea->addSubWindow(_signalView3);
        _subWindow3->setWindowTitle("Сигнал 3");
        _subWindow3->show();
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView3);
        _subWindow3->close();
    }
}

void MainWindow::onPb4Click()
{
    if (_pb4->isChecked())
    {
        _subWindow4 = ui->mdiArea->addSubWindow(_signalView4);
        _subWindow4->setWindowTitle("Сигнал 4");
        _subWindow4->show();
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView4);
        _subWindow4->close();
    }
}
//==========================================================================================

void MainWindow::onPbOnOffClick()
{
    _settingsWindow->autoSetParams(); //На всякий установили параметры
    if (_pbOnOff->isChecked())
        emit signalStart(); //Стартуем!
    else
        emit signalStop(); //Останавливаем генерацию сигнала
}

void MainWindow::onScreenShotClick()
{
    this->raise(); //Подняли текущее окно (чтоб ничего его не перегораживало и сделали скрин (след. строка)
                   //Тут скриншот падает в буфер обмена сразу
    QApplication::clipboard()->setPixmap(QPixmap(this->windowHandle()->screen()->grabWindow(QWidget::winId())));
}

void MainWindow::onAboutProgram()
{
    //Показываем информацию о программе
    QMessageBox msg;
    msg.setWindowTitle("О программе");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Задание №2 \r\n"
                "по дисциплине \"Разработка пользовательского интерфейса в компьютерных управляющих системах\"\r\n"
                "студента группы АТ1-19-1М\r\n"
                "Полыгаловой Дарьи Константиновны");
    msg.exec();
}

void MainWindow::onAboutQt()
{
    QMessageBox::aboutQt(this); //Показали стандартное окно о Qt
}

void MainWindow::onSetupSignals()
{
    _settingsWindow
        ->showCurParams(); //Вместо обычного show, чтоб сначала считать параметры из реестра и отобразить их в окне
}

void MainWindow::onShowStatus(QString status, int type)
{
    //Отображение статуса (аварийных ситуаций)
    if (ui->statusBar->currentMessage()
            .isEmpty()) //Если сейчас никакой статус не отображается, то можно писать что угодно
    {
        curMessageType = type; //Запомнили от какого окна пришел текущий статус для сравнения приоритетов
        ui->statusBar->showMessage(status); //Отобразили статус
    }
    else
    {
        if (status.isEmpty() && !checkPriority(curMessageType, type)) //Если статус пустой, и не приоритетный
            return;
        else //Если статус не пустой и приоритет достаточный, чтобы отображаться, то
        {
            if (checkPriority(curMessageType, type))
            {
                ui->statusBar->showMessage(status); //Отображаем
            }
        }
    }

    // Смотрим какое окно надо поднять, а может и открыть
    if (status.contains("1"))
    {
        if (_pb1->isChecked() == false)
        {
            _pb1->setChecked(true);
            onPb1Click();
        }
        _subWindow1->raise();
        _subWindow1->setFocus();
    }
    else if (status.contains("2"))
    {
        if (_pb2->isChecked() == false)
        {
            _pb2->setChecked(true);
            onPb2Click();
        }
        _subWindow2->raise();
        _subWindow2->setFocus();
    }
    else if (status.contains("3"))
    {
        if (_pb3->isChecked() == false)
        {
            _pb3->setChecked(true);
            onPb3Click();
        }
        _subWindow3->raise();
        _subWindow3->setFocus();
    }
    else if (status.contains("4"))
    {
        if (_pb4->isChecked() == false)
        {
            _pb4->setChecked(true);
            onPb4Click();
        }
        _subWindow4->raise();
        _subWindow4->setFocus();
    }
}

bool MainWindow::checkPriority(int curMsg, int newMsg)
{
    //Сравниваем приоритеты сообщений
    if (priority.contains(curMsg) && priority.contains(newMsg)) //Если такие индексы вообще есть в списке приоритетов
    {
        return priority.indexOf(newMsg) <= priority.indexOf(curMsg); //Возвращаем результат сравнения
    }
    else
        return false;
}
