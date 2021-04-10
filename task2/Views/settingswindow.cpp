#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QSettings>

SettingsWindow::SettingsWindow(QWidget* parent) : QWidget(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setWindowTitle("Настройки сигналов");

    loadFromRegistry();
}

SettingsWindow::~SettingsWindow() { delete ui; }

void SettingsWindow::on_pushButton_2_clicked() { close(); }

void SettingsWindow::on_pushButton_clicked()
{
    setCurParams();
    emit signalSetLimits(_curLimits, _curParams);
    emit signalSetParameters(_curParams);
    close();
}

void SettingsWindow::setCurParams()
{
    _curParams.generateFreq = ui->spinBox->value();
    _curParams.sinFreq = 1.0 / ui->doubleSpinBoxPeriod->value();
    _curParams.sinAmp = ui->doubleSpinBoxSinAmp->value();

    _curParams.randomMo = ui->doubleSpinBoxRandomMo->value();
    _curParams.randomSko = ui->doubleSpinBoxRandomSko->value();

    _curParams.traingleMax = ui->doubleSpinBoxTriangleMax->value();
    _curParams.traingleMin = ui->doubleSpinBoxTriangleMin->value();
    _curParams.traingleRaiseK = ui->doubleSpinBoxTriangleRaiseSpeed->value();
    _curParams.traingleUnriseK = ui->doubleSpinBoxTriangleUnriseSpeed->value();

    _curParams.variantMax = ui->doubleSpinBoxVariantMax->value();
    _curParams.variantMin = ui->doubleSpinBoxVariantMin->value();
    _curParams.variantPeriod = ui->spinBoxVariantPeriod->value();

    _curLimits.sinMax = ui->doubleSpinBoxSinLimit->value();
    _curLimits.traingleMin = ui->doubleSpinBoxTraingleLimit->value();
    _curLimits.randomMax = ui->doubleSpinBoxRandomUpLimit->value();
    _curLimits.randomMin = ui->doubleSpinBoxRandomLowLimit->value();
    _curLimits.variantMax = ui->doubleSpinBoxVariantUpLimit->value();
    _curLimits.variantMin = ui->doubleSpinBoxVariantLowLimit->value();
    saveToRegistry();
}

void SettingsWindow::showCurParams()
{
    loadFromRegistry();
    ui->spinBox->setValue(_curParams.generateFreq);
    ui->doubleSpinBoxPeriod->setValue(1.0 / _curParams.sinFreq);
    ui->doubleSpinBoxSinAmp->setValue(_curParams.sinAmp);

    ui->doubleSpinBoxRandomMo->setValue(_curParams.randomMo);
    ui->doubleSpinBoxRandomSko->setValue(_curParams.randomSko);

    ui->doubleSpinBoxTriangleMax->setValue(_curParams.traingleMax);
    ui->doubleSpinBoxTriangleMin->setValue(_curParams.traingleMin);
    ui->doubleSpinBoxTriangleRaiseSpeed->setValue(_curParams.traingleRaiseK);
    ui->doubleSpinBoxTriangleUnriseSpeed->setValue(_curParams.traingleUnriseK);

    ui->doubleSpinBoxVariantMax->setValue(_curParams.variantMax);
    ui->doubleSpinBoxVariantMin->setValue(_curParams.variantMin);
    ui->spinBoxVariantPeriod->setValue(_curParams.variantPeriod);

    ui->doubleSpinBoxSinLimit->setValue(_curLimits.sinMax);
    ui->doubleSpinBoxTraingleLimit->setValue(_curLimits.traingleMin);
    ui->doubleSpinBoxRandomUpLimit->setValue(_curLimits.randomMax);
    ui->doubleSpinBoxRandomLowLimit->setValue(_curLimits.randomMin);
    ui->doubleSpinBoxVariantUpLimit->setValue(_curLimits.variantMax);
    ui->doubleSpinBoxVariantLowLimit->setValue(_curLimits.variantMin);

    show();
}

void SettingsWindow::saveToRegistry()
{
    QSettings obj("PNIPU_ETF", "task2");
    obj.setValue("generateFreq", _curParams.generateFreq);
    obj.setValue("sinFreq", _curParams.sinFreq);
    obj.setValue("sinAmp", _curParams.sinAmp);
    obj.setValue("randomMo", _curParams.randomMo);
    obj.setValue("randomSko", _curParams.randomSko);

    obj.setValue("TraingleMax", _curParams.traingleMax);
    obj.setValue("TraingleMin", _curParams.traingleMin);
    obj.setValue("TraingleRaiseK", _curParams.traingleRaiseK);
    obj.setValue("TraingleUnriseK", _curParams.traingleUnriseK);

    obj.setValue("VariantMax", _curParams.variantMax);
    obj.setValue("VariantMin", _curParams.variantMin);
    obj.setValue("VariantPeriod", _curParams.variantPeriod);

    obj.setValue("SinMax", _curLimits.sinMax);
    obj.setValue("TraingleMin", _curLimits.traingleMin);
    obj.setValue("RandomMax", _curLimits.randomMax);
    obj.setValue("RandomMin", _curLimits.randomMin);
    obj.setValue("VariantMaxLimit", _curLimits.variantMax);
    obj.setValue("VariantMinLimit", _curLimits.variantMin);
}

void SettingsWindow::loadFromRegistry()
{
    QSettings obj("PNIPU_ETF", "task2");
    if (obj.contains("generateFreq"))
        _curParams.generateFreq = obj.value("generateFreq").toInt();
    else
        _curParams.generateFreq = 50;

    if (obj.contains("sinFreq"))
        _curParams.sinFreq = obj.value("sinFreq").toDouble();
    else
        _curParams.sinFreq = 1;

    if (obj.contains("sinAmp"))
        _curParams.sinAmp = obj.value("sinAmp").toInt();
    else
        _curParams.sinAmp = 50;

    if (obj.contains("randomMo"))
        _curParams.randomMo = obj.value("randomMo").toDouble();
    else
        _curParams.randomMo = 0;

    if (obj.contains("randomSko"))
        _curParams.randomSko = obj.value("randomSko").toDouble();
    else
        _curParams.randomSko = 70;

    if (obj.contains("TraingleMax"))
        _curParams.traingleMax = obj.value("TraingleMax").toDouble();
    else
        _curParams.traingleMax = 110;

    if (obj.contains("TraingleMax"))
        _curParams.traingleMin = obj.value("TraingleMin").toDouble();
    else
        _curParams.traingleMin = -110;

    if (obj.contains("TraingleRaiseK"))
        _curParams.traingleRaiseK = obj.value("TraingleRaiseK").toDouble();
    else
        _curParams.traingleRaiseK = 4;

    if (obj.contains("TraingleUnriseK"))
        _curParams.traingleUnriseK = obj.value("TraingleUnriseK").toDouble();
    else
        _curParams.traingleUnriseK = 2;

    if (obj.contains("sinFreqrandomVariantMax"))
        _curParams.variantMax = obj.value("VariantMax").toDouble();
    else
        _curParams.variantMax = 90;

    if (obj.contains("VariantMin"))
        _curParams.variantMin = obj.value("VariantMin").toDouble();
    else
        _curParams.variantMin = -90;

    if (obj.contains("VariantPeriod"))
        _curParams.variantPeriod = obj.value("VariantPeriod").toInt();
    else
        _curParams.variantPeriod = 1;





    if (obj.contains("SinMax"))
        _curLimits.sinMax = obj.value("SinMax").toDouble();
    else
        _curLimits.sinMax = 110;

    if (obj.contains("TraingleMin"))
        _curLimits.traingleMin = obj.value("TraingleMin").toDouble();
    else
        _curLimits.traingleMin = -120;

    if (obj.contains("RandomMax"))
        _curLimits.randomMax = obj.value("RandomMax").toDouble();
    else
        _curLimits.randomMax = 100;

    if (obj.contains("RandomMin"))
        _curLimits.randomMin = obj.value("RandomMin").toDouble();
    else
        _curLimits.randomMin = 0;

    if (obj.contains("VariantMaxLimit"))
        _curLimits.variantMax = obj.value("VariantMaxLimit").toDouble();
    else
        _curLimits.variantMax = 110;

    if (obj.contains("VariantMinLimit"))
        _curLimits.variantMin = obj.value("VariantMinLimit").toDouble();
    else
        _curLimits.variantMin = -10;
}

void SettingsWindow::autoSetParams()
{
    emit signalSetLimits(_curLimits, _curParams);
    emit signalSetParameters(_curParams);
}
