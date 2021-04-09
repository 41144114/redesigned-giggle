#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
typedef struct signalsParams_t
{
    int generateFreq;

    double sinFreq;
    double sinAmp;

    double traingleMax;
    double traingleMin;
    double traingleRaiseK;
    double traingleUnriseK;

    double randomMo;
    double randomSko;

    int variantPeriod;
    double variantMax;
    double variantMin;
} signalsParams_t;

typedef struct signalsLimits_t
{
    double sinMax;
    double traingleMin;

    double randomMax;
    double randomMin;

    double variantMax;
    double variantMin;
} signalsLimits_t;

namespace Ui
{
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget* parent = nullptr);
    ~SettingsWindow();
    void showCurParams();
    void autoSetParams();

signals:
    void signalSetLimits(signalsLimits_t, signalsParams_t);
    void signalSetParameters(signalsParams_t);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SettingsWindow* ui;

    signalsLimits_t _curLimits;
    signalsParams_t _curParams;

    void setCurParams();
    void loadFromRegistry();
    void saveToRegistry();
};

#endif // SETTINGSWINDOW_H
