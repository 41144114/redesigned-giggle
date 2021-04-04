#ifndef GENERALVIEW_H
#define GENERALVIEW_H

#include <QWidget>

namespace Ui {
class GeneralView;
}

class GeneralView : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralView(QWidget *parent = 0);
    ~GeneralView();

private:
    Ui::GeneralView *ui;
};

#endif // GENERALVIEW_H
