#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "signalview.h"
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QClipboard>
#include <QWindow>
#include <QScreen>
#include "settingswindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SignalView* testWindow = new SignalView();
    ui->mdiArea->addSubWindow(testWindow);
    setupToolBar();
    setupMainMenu();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupToolBar()
{
    QPushButton* pb0 = new QPushButton();
    QPushButton* pb1 = new QPushButton();
    QPushButton* pb2 = new QPushButton();
    QPushButton* pb3 = new QPushButton();
    QPushButton* pb4 = new QPushButton();
    QPushButton* pbOnOff = new QPushButton();
    QPushButton* pbScreenShot = new QPushButton();

    QFont font;
    font.setFamily("Century");
    font.setPointSize(12);
    pb0->setFont(font);
    pb1->setFont(font);
    pb2->setFont(font);
    pb3->setFont(font);
    pb4->setFont(font);


    pb0->setText("0");
    pb1->setText("1");
    pb2->setText("2");
    pb3->setText("3");
    pb4->setText("4");


    pb0->setCheckable(true);
    pb1->setCheckable(true);
    pb2->setCheckable(true);
    pb3->setCheckable(true);
    pb4->setCheckable(true);
    pbOnOff->setIcon(QIcon(":/onOffIcon.png"));
    pbScreenShot->setIcon(QIcon(":/photo.png"));

    ui->mainToolBar->addWidget(pbOnOff);
    ui->mainToolBar->addWidget(pb0);
    ui->mainToolBar->addWidget(pb1);
    ui->mainToolBar->addWidget(pb2);
    ui->mainToolBar->addWidget(pb3);
    ui->mainToolBar->addWidget(pb4);
    ui->mainToolBar->addWidget(pbScreenShot);

    pb0->setMaximumHeight(20);
    pb1->setMaximumHeight(20);
    pb2->setMaximumHeight(20);
    pb3->setMaximumHeight(20);
    pb4->setMaximumHeight(20);
    pbScreenShot->setMaximumHeight(20);
    pbOnOff->setMaximumHeight(20);

    pb0->setMaximumWidth(20);
    pb1->setMaximumWidth(20);
    pb2->setMaximumWidth(20);
    pb3->setMaximumWidth(20);
    pb4->setMaximumWidth(20);
    pbScreenShot->setMaximumWidth(20);
    pbOnOff->setMaximumWidth(20);

    connect(pb1, &QPushButton::clicked, this, &MainWindow::onPb1Click);
    connect(pb2, &QPushButton::clicked, this, &MainWindow::onPb2Click);
    connect(pb3, &QPushButton::clicked, this, &MainWindow::onPb3Click);
    connect(pb4, &QPushButton::clicked, this, &MainWindow::onPb4Click);
    connect(pbOnOff, &QPushButton::clicked, this, &MainWindow::onPbOnOffClick);
    connect(pbScreenShot, &QPushButton::clicked, this, &MainWindow::onScreenShotClick);
}

void MainWindow::setupMainMenu()
{
    connect(ui->action_Qt, &QAction::triggered, this, &MainWindow::onAboutQt);
    connect(ui->actionAboutProgramm, &QAction::triggered, this, &MainWindow::onAboutProgram);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onSetupSignals);
}

void MainWindow::onPb1Click()
{

}

void MainWindow::onPb2Click()
{

}

void MainWindow::onPb3Click()
{

}

void MainWindow::onPb4Click()
{

}

void MainWindow::onPbOnOffClick()
{
    qDebug() << "here";
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

void MainWindow::onAboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onSetupSignals()
{

}
