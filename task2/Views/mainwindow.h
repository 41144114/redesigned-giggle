#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GeneralView;
class SignalView;
class SettingsWindow;
class QPushButton;
class QMdiSubWindow;
class SignalGenerator;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onPbOnOffClick();
    void onScreenShotClick();
    void onPb0Click();
    void onPb1Click();
    void onPb2Click();
    void onPb3Click();
    void onPb4Click();
    void onAboutQt();
    void onAboutProgram();
    void onSetupSignals();

    void onShowStatus(QString status, int type);

signals:
    void signalStart();
    void signalStop();

private:
    Ui::MainWindow* ui;

    SettingsWindow* _settingsWindow;
    SignalView* _signalView1;
    SignalView* _signalView2;
    SignalView* _signalView3;
    SignalView* _signalView4;
    GeneralView* _signalView0;

    SignalGenerator* generator;

    QMdiSubWindow* _subWindow0;
    QMdiSubWindow* _subWindow1;
    QMdiSubWindow* _subWindow2;
    QMdiSubWindow* _subWindow3;
    QMdiSubWindow* _subWindow4;

    QPushButton* _pb0;
    QPushButton* _pb1;
    QPushButton* _pb2;
    QPushButton* _pb3;
    QPushButton* _pb4;
    QPushButton* _pbOnOff;
    QPushButton* _pbScreenShot;

    int curMessageType;

    bool checkPriority(int curMsg, int newMsg);

    void setupToolBar();
    void setupMainMenu();
    void setupSubWindows();
    void setupConnections();
    void allocateMemory();
};

#endif // MAINWINDOW_H
