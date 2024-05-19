#include "dialogaddposition.h"
#include "ui_dialogaddposition.h"

dialogAddPosition::dialogAddPosition(QWidget *parent, const QString &exchange, const QString &market, const double &buyPrice) :
    QDialog(parent),
    ui(new Ui::dialogAddPosition),
    m_exchange(exchange),
    m_market(market),
    m_buyPrice(buyPrice)
{
    ui->setupUi(this);
    if (m_buyPrice > -1) {  ui->spinBuyPrice->setValue(buyPrice);   }
    if (!m_market.isEmpty()) {  ui->edMarket->setText(market.toUpper());    }
    if (!m_exchange.isEmpty()) {
        int index = ui->cmbExchange->findText(exchange, Qt::MatchFixedString);
        ui->cmbExchange->setCurrentIndex(index);
    }

}

dialogAddPosition::~dialogAddPosition()
{
    delete ui;
}

void dialogAddPosition::on_buttonBox_accepted()
{
    try {
        QString exchange = this->ui->cmbExchange->currentText().toLower();
        QString market = this->ui->edMarket->text().toUpper();
        double amount = this->ui->spinAmount->value();
        double buyPrice = this->ui->spinBuyPrice->value();
        emit signalAddPosition(exchange, market, amount, buyPrice);

    } catch (const std::exception& e) { qWarning() << "Error: " << e.what();    }

}

