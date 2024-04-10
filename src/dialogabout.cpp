#include "dialogabout.h"
#include "ui_dialogabout.h"

dialogAbout::dialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogAbout)
{
    ui->setupUi(this);
    qWarning() << "joaan";
}

dialogAbout::~dialogAbout()
{
    delete ui;
}
