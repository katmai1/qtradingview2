#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "version.h"


dialogAbout::dialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAbout)
{
    ui->setupUi(this);
    ui->labelVersion->setText(APP_VERSION);
}

dialogAbout::~dialogAbout()
{
    delete ui;
}
