#include "portfolio.h"
#include "ui_portfolio.h"

Portfolio::Portfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Portfolio)
{
    ui->setupUi(this);
}

Portfolio::~Portfolio()
{
    delete ui;
}
