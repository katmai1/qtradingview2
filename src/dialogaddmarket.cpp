#include "dialogaddmarket.h"
#include "mainwindow.h"
#include "qpushbutton.h"
#include "ui_dialogaddmarket.h"

dialogAddMarket::dialogAddMarket(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::dialogAddMarket),
    mwin(parent)
{
    ui->setupUi(this);
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

dialogAddMarket::~dialogAddMarket()
{
    delete ui;
}

void dialogAddMarket::on_buttonBox_accepted()
{
    QString texto = this->ui->editEx->text().toUpper() + ":" + this->ui->comboEx->currentText().toUpper();
    mwin->addToList(texto);
}


void dialogAddMarket::on_editEx_textChanged(const QString &arg1)
{
    bool activar = false;
    if (!arg1.isEmpty() && arg1.contains("/")) {
        activar = true;
    }
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(activar);
}

