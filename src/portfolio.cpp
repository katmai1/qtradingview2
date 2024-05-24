#include "portfolio.h"
#include "qmessagebox.h"
#include "ui_portfolio.h"

#include "src/settings.h"
#include "src/dialogaddposition.h"
#include "src/exmanager.h"


Portfolio::Portfolio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Portfolio),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    // carga la lista sin actualizar online
    this->on_btnRefresh_clicked(true);

    // timer para actualizar
    timer->setInterval(30000);
    connect(timer, &QTimer::timeout, this, [this]() {   this->on_btnRefresh_clicked(false); });
    timer->start();

    // timer para la primera actualizacion en 5 segundos
    QTimer::singleShot(2000, this, [this]() {
        this->on_btnRefresh_clicked(false);
    });

}

Portfolio::~Portfolio()
{
    delete ui;
}

void Portfolio::closeEvent(QCloseEvent *event) {
    qInfo() << "Cerrando el portfolio...";
    if (timer->isActive()) {    timer->stop();  }
    QDialog::closeEvent(event);
}

void Portfolio::on_btnRefresh_clicked(const bool loadOnly)
{
    try {
        qInfo() << "Actualizando portfolio...";
        SettingsManager settings;
        QList<Positions> PositionsList = settings.getPositions();
        int row = 0;
        this->ui->tableWidget->setRowCount(PositionsList.count());
        for (const auto& trade : PositionsList) {
            if (loadOnly) {
                this->insertTrade(row, trade, 0, 0, 0);
            }
            else {
                ExchangeBase* ex = getExchangeClass(trade.exchange.toLower());
                double lastPrice = ex->getPrice(trade.market);

                double profit = calcProfit(trade.amount, trade.buyPrice, lastPrice);
                double profitPercent = calcProfitPercent(trade.amount, trade.buyPrice, lastPrice);

                this->insertTrade(row, trade, lastPrice, profit, profitPercent);
            }
            row++;
        }

    } catch (const std::exception& e) { std::cout << e.what();   }

}


// insert trade a la tabla
void Portfolio::insertTrade(int row, Positions trade, double lastPrice, double profit, double profitPercent) {
    // exchange
    QTableWidgetItem *itemExchange = new class QTableWidgetItem(trade.exchange.toUpper());
    itemExchange->setTextAlignment(Qt::AlignCenter);
    this->ui->tableWidget->setItem(row, 0, itemExchange);
    // market
    QTableWidgetItem *itemMarket = new class QTableWidgetItem(trade.market);
    itemMarket->setTextAlignment(Qt::AlignCenter);
    this->ui->tableWidget->setItem(row, 1, itemMarket);
    // profit
    QTableWidgetItem *itemProfit = new class QTableWidgetItem(
        QString("%1 (%2%)").arg(profit).arg(QString::number(profitPercent, 'f', 2)));
    itemProfit->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    if (profit > 0) {   itemProfit->setForeground(Qt::darkGreen); }
    else if (profit < 0) {  itemProfit->setForeground(Qt::darkRed);  }
    this->ui->tableWidget->setItem(row, 2, itemProfit);
    // amount
    QTableWidgetItem *itemAmount = new class QTableWidgetItem(QString("%1").arg(trade.amount));
    itemAmount->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->ui->tableWidget->setItem(row, 3, itemAmount);
    // buy price
    QTableWidgetItem *itemBuyPrice = new class QTableWidgetItem(QString("%1").arg(trade.buyPrice));
    itemBuyPrice->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->ui->tableWidget->setItem(row, 4, itemBuyPrice);
    // last price
    QTableWidgetItem *itemLastPrice = new class QTableWidgetItem(QString("%1").arg(lastPrice));
    itemLastPrice->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->ui->tableWidget->setItem(row, 5, itemLastPrice);
}

void Portfolio::on_btnDelete_clicked()
{
    int row = this->ui->tableWidget->currentRow();
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(QString("Estas seguro de quieres eliminar la posicion numero %1?").arg(row+1));
    msgBox.setInformativeText("Esta acción no se puede deshacer.");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    // Manejar la respuesta del usuario
    if (ret == QMessageBox::Yes) {
        this->ui->tableWidget->removeRow(row);
        this->savePositions();
    }
}

void Portfolio::savePositions() {
    QList<Positions> PositionsList;
    for (int row = 0; row < this->ui->tableWidget->rowCount(); ++row) {
        QString exchange = this->ui->tableWidget->item(row, 0)->text();
        QString market = this->ui->tableWidget->item(row, 1)->text();
        double amount = this->ui->tableWidget->item(row, 3)->text().toDouble();
        double buyPrice = this->ui->tableWidget->item(row, 4)->text().toDouble();
        PositionsList.append({exchange, market, amount, buyPrice});
    }
    QJsonArray jsonArray;
    for (const auto& trade : PositionsList) {
        jsonArray.append(trade.toJson());
    }
    QJsonDocument doc(jsonArray);
    QString jsonString = doc.toJson(QJsonDocument::Compact);
    SettingsManager settings;
    settings.setValue("positions", jsonString, "Portfolio");
}

void Portfolio::on_btnAdd_clicked()
{
    dialogAddPosition *addPosition;
    addPosition = new dialogAddPosition(this);
    addPosition->setModal(true);
    connect(addPosition, &dialogAddPosition::signalAddPosition, this, &Portfolio::slotAddPosition);
    addPosition->show();
}

void Portfolio::slotAddPosition(QString exchange, QString market, double amount, double buyPrice) {
    this->ui->tableWidget->insertRow(0);
    Positions trade;
    trade.exchange = exchange;
    trade.market = market;
    trade.amount = amount;
    trade.buyPrice = buyPrice;
    this->insertTrade(0, trade, 0, 0, 0);
    this->savePositions();
}
