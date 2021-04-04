#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onPbOnOffClick();
    void onScreenShotClick();
    void onPb1Click();
    void onPb2Click();
    void onPb3Click();
    void onPb4Click();

private:
    Ui::MainWindow *ui;

    void setupToolBar();
    void setupMainMenu();
};

#endif // MAINWINDOW_H
