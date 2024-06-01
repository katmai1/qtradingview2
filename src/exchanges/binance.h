#ifndef BINANCE_H
#define BINANCE_H

#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "exchange_base.h"


class Binance : public ExchangeBase {

public:
    explicit Binance(QObject *parent = nullptr) : ExchangeBase(parent) {
        url = QUrl("https://api.binance.com/api/v3");
    }

    void test() override {
        setPathUrl("ticker/price");
        QUrlQuery query;
        query.addQueryItem("symbol", "BTCUTxxx");
        url.setQuery(query);
        QJsonObject json = this->fetchQuery(url);
        if (json["qt2_status"] == "ok") {
            qDebug() << json;
        }
        else {
            qDebug() << json["qt2_error"];
        }

    }


private:
    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + base.toUpper();
        setPathUrl("ticker/price");
        QUrlQuery query;
        query.addQueryItem("symbol", market);
        url.setQuery(query);
        QJsonObject json = this->fetchQuery(url);
        if (json["qt2_status"] == "ok") {   return json["price"].toString().toDouble(); }
        else {  qDebug() << json["qt2_error"];  }
        return 0.0;
    }

};

#endif // BINANCE_H
