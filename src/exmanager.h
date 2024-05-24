#ifndef EXMANAGER_H
#define EXMANAGER_H

#include "src/exchanges/exchange_base.h"
#include "src/exchanges/binance.h"
#include "src/exchanges/kucoin.h"
#include "src/exchanges/bingx.h"


class ExManager : public QObject {
    Q_OBJECT
public:
    explicit ExManager(QObject *parent = nullptr) : QObject(parent) {}

    ExchangeBase* setExchange(QString exchange) {
        if (exchange == "binance") {    return new Binance;  }
        else if (exchange == "kucoin") {    return new Kucoin;  }
        else if (exchange == "bingx") {    return new Bingx;  }
        else {  return new ExchangeBase;    }
    }
};

//// calcula los beneficios de una entrada
//double calcProfit(double amount, double buyPrice, double lastPrice) {
//    return amount * (lastPrice - buyPrice);
//}

//// calcula los beneficios de una entrada
//double calcProfitPercent(double amount, double buyPrice, double lastPrice) {
//    double profit = amount * (lastPrice - buyPrice);
//    return (profit / (amount * buyPrice)) * 100.0;
//}

#endif // EXMANAGER_H
