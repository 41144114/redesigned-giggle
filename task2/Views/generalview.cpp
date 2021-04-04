#include "generalview.h"
#include "ui_generalview.h"

GeneralView::GeneralView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralView)
{
    ui->setupUi(this);
}

GeneralView::~GeneralView()
{
    delete ui;
}
