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

    allocateMemory();
    setupToolBar();
    setupMainMenu();
    setupSubWindows();
    setupConnections();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::allocateMemory()
{
    _settingsWindow = new SettingsWindow();
    _signalView0 = new GeneralView();
    _signalView1 = new SignalView();
    _signalView2 = new SignalView();
    _signalView3 = new SignalView();
    _signalView4 = new SignalView();

    generator = new SignalGenerator();

    QThread* signalGeneratorThread = new QThread();
    generator->moveToThread(signalGeneratorThread);
    connect(signalGeneratorThread, &QThread::started, generator, &SignalGenerator::startProcess);
    signalGeneratorThread->start();
}

void MainWindow::setupToolBar()
{
    _pb0 = new QPushButton();
    _pb1 = new QPushButton();
    _pb2 = new QPushButton();
    _pb3 = new QPushButton();
    _pb4 = new QPushButton();
    _pbOnOff = new QPushButton();
    _pbScreenShot = new QPushButton();

    QFont font;
    font.setFamily("Century");
    font.setPointSize(12);
    _pb0->setFont(font);
    _pb1->setFont(font);
    _pb2->setFont(font);
    _pb3->setFont(font);
    _pb4->setFont(font);

    _pb0->setText("0");
    _pb1->setText("1");
    _pb2->setText("2");
    _pb3->setText("3");
    _pb4->setText("4");

    _pb0->setCheckable(true);
    _pb1->setCheckable(true);
    _pb2->setCheckable(true);
    _pb3->setCheckable(true);
    _pb4->setCheckable(true);
    _pbOnOff->setCheckable(true);

    _pbOnOff->setIcon(QIcon(":/onOffIcon.png"));
    _pbScreenShot->setIcon(QIcon(":/photo.png"));

    ui->mainToolBar->addWidget(_pbOnOff);
    ui->mainToolBar->addWidget(_pb0);
    ui->mainToolBar->addWidget(_pb1);
    ui->mainToolBar->addWidget(_pb2);
    ui->mainToolBar->addWidget(_pb3);
    ui->mainToolBar->addWidget(_pb4);
    ui->mainToolBar->addWidget(_pbScreenShot);

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
    connect(ui->action_Qt, &QAction::triggered, this, &MainWindow::onAboutQt);
    connect(ui->actionAboutProgramm, &QAction::triggered, this, &MainWindow::onAboutProgram);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onSetupSignals);
}

void MainWindow::setupSubWindows()
{
    _signalView1->setType(1);
    _signalView2->setType(2);
    _signalView3->setType(3);
    _signalView4->setType(4);
}

void MainWindow::setupConnections()
{
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

    connect(this, &MainWindow::signalStart, generator, &SignalGenerator::startGenerate);
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

void MainWindow::onPb0Click()
{
    if (_pb0->isChecked())
    {
        _subWindow0 = ui->mdiArea->addSubWindow(_signalView0);
        _subWindow0->setWindowTitle("Все сигналы");
        _subWindow0->show();
    }
    else
    {
        ui->mdiArea->removeSubWindow(_signalView0);
        _subWindow0->close();
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

void MainWindow::onPbOnOffClick()
{
    _settingsWindow->autoSetParams();
    if (_pbOnOff->isChecked())
        emit signalStart();
    else
        emit signalStop();
}

void MainWindow::onScreenShotClick()
{
    this->raise();
    QApplication::clipboard()->setPixmap(QPixmap(this->windowHandle()->screen()->grabWindow(QWidget::winId())));
}

void MainWindow::onAboutProgram()
{
    QMessageBox msg;
    msg.setWindowTitle("О программе");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Задание №2 студентки .....");
    msg.exec();
}

void MainWindow::onAboutQt() { QMessageBox::aboutQt(this); }

void MainWindow::onSetupSignals() { _settingsWindow->showCurParams(); }

void MainWindow::onShowStatus(QString status, int type)
{
    if (ui->statusBar->currentMessage().isEmpty())
    {
        curMessageType = type;
        ui->statusBar->showMessage(status);
    }
    else
    {
        if (status.isEmpty() && !checkPriority(curMessageType, type))
            return;
        else
        {
            if (checkPriority(curMessageType, type))
            {
                ui->statusBar->showMessage(status);
            }
        }
    }

    if(status.contains("1"))
    {
        if(_pb1->isChecked() == false)
        {
            _pb1->setChecked(true);
            onPb1Click();
        }
        _subWindow1->raise();
        _subWindow1->setFocus();
    }
    else if(status.contains("2"))
    {
        if(_pb2->isChecked() == false)
        {
            _pb2->setChecked(true);
            onPb2Click();
        }
        _subWindow2->raise();
        _subWindow2->setFocus();
    }
    else if(status.contains("3"))
    {
        if(_pb3->isChecked() == false)
        {
            _pb3->setChecked(true);
            onPb3Click();
        }
        _subWindow3->raise();
        _subWindow3->setFocus();
    }
    else if(status.contains("4"))
    {
        if(_pb4->isChecked() == false)
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
    if (priority.contains(curMsg) && priority.contains(newMsg))
    {
        return priority.indexOf(newMsg) <= priority.indexOf(curMsg);
    }
    else
        return false;
}
