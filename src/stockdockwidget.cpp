#include "stockdockwidget.h"
#include "ui_stockdockwidget.h"

StockDockWidget::StockDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::StockDockWidget)
{
    ui->setupUi(this);
}

StockDockWidget::~StockDockWidget()
{
    delete ui;
}
