#include "dialogaddmarket.h"
#include "mainwindow.h"
#include "qpushbutton.h"
#include "ui_dialogaddmarket.h"

#include <QMessageBox>

#include "src/settings.h"


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
    QString market = this->ui->editEx->text().toUpper() + ":" + this->ui->comboEx->currentText().toUpper();
    SettingsManager settings;
    if (settings.existMarket(market)) {
        QMessageBox::warning(nullptr, "Error", "Este mercado ya está en la lista...");
            this->show();
    }
    else {  mwin->addToList(market);    }
}

void dialogAddMarket::on_editEx_textChanged(const QString &arg1)
{
    bool activar = false;
    if (!arg1.isEmpty() && arg1.contains("/")) {
        activar = true;
    }
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(activar);
}

