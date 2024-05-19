#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QDialog>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>


struct Positions {
    QString exchange;
    QString market;
    double amount;
    double buyPrice;

    // Función para serializar la entrada a un objeto JSON
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["exchange"] = exchange;
        obj["market"] = market;
        obj["amount"] = amount;
        obj["buyPrice"] = buyPrice;
        return obj;
    }

    // Función para deserializar un objeto JSON a una entrada
    static Positions fromJson(const QJsonObject& obj) {
        Positions trade;
        trade.exchange = obj["exchange"].toString();
        trade.market = obj["market"].toString();
        trade.amount = obj["amount"].toDouble();
        trade.buyPrice = obj["buyPrice"].toDouble();
        return trade;
    }
};


namespace Ui {
class Portfolio;
}

class Portfolio : public QDialog
{
    Q_OBJECT

public:
    explicit Portfolio(QWidget *parent = nullptr);
    ~Portfolio();

    void insertTrade(int row, Positions trade, double lastPrice, double profit, double profitPercent);

    void savePositions();

private slots:
    void on_btnRefresh_clicked(const bool loadOnly=false);

    void on_btnDelete_clicked();

    void on_btnAdd_clicked();

public slots:
    void slotAddPosition(QString exchange, QString market, double amount, double buyPrice);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::Portfolio *ui;
    QTimer *timer;
};

#endif // PORTFOLIO_H
