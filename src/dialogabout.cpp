#include "dialogabout.h"
#include "ui_dialogabout.h"


dialogAbout::dialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAbout)
{
    ui->setupUi(this);
    ui->labelVersion->setText(QCoreApplication::applicationVersion());
}

dialogAbout::~dialogAbout()
{
    delete ui;
}
