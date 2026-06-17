#ifndef STOCKDOCKWIDGET_H
#define STOCKDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class StockDockWidget;
}

class StockDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit StockDockWidget(QWidget *parent = nullptr);
    ~StockDockWidget();

private:
    Ui::StockDockWidget *ui;
};

#endif // STOCKDOCKWIDGET_H
