#include "dialogaddmarket.h"
#include "mainwindow.h"
#include "qpushbutton.h"
#include "ui_dialogaddmarket.h"

#include <QMessageBox>

#include "src/settings.h"
#include "src/exmanager.h"


dialogAddMarket::dialogAddMarket(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::dialogAddMarket),
    mwin(parent)
{
    ui->setupUi(this);

//    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->on_comboEx_currentTextChanged(this->ui->comboEx->currentText());
}

dialogAddMarket::~dialogAddMarket()
{
    delete ui;
}

void dialogAddMarket::on_buttonBox_accepted()
{
    QString symbol = this->ui->listMarkets->currentIndex().data(Qt::DisplayRole).toString();
    QString market = symbol + ":" + this->ui->comboEx->currentText().toUpper();
    SettingsManager settings;
    if (settings.existMarket(market)) {
        QMessageBox::warning(nullptr, "Error", "Este mercado ya está en la lista...");
        this->show();
    }
    else {  mwin->addToList(market);    }
}

//void dialogAddMarket::on_editEx_textChanged(const QString &arg1)
//{
//    bool activar = false;
//    if (!arg1.isEmpty() && arg1.contains("/")) {
//        activar = true;
//    }
//    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(activar);
//}


void dialogAddMarket::on_comboEx_currentTextChanged(const QString &arg1)
{
    ExchangeBase *ex = getExchangeClass(arg1.toLower());
    // Crear un QStringListModel y asignarle el QStringList
    QStringListModel *model = new QStringListModel();
    model->setStringList(ex->getMarketsList());
    // Crear un QSortFilterProxyModel para el filtrado
    QSortFilterProxyModel *filterModel = new QSortFilterProxyModel();
    filterModel->setSourceModel(model);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(ui->editEx, &QLineEdit::textChanged, filterModel, &QSortFilterProxyModel::setFilterFixedString);
    this->ui->listMarkets->setModel(filterModel);
}

