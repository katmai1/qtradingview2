#ifndef KUCOIN_H
#define KUCOIN_H

#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "exchange_base.h"


class Kucoin : public ExchangeBase {


public:
    explicit Kucoin(QObject *parent = nullptr) : ExchangeBase(parent) {
        url = QUrl("https://api.kucoin.com/api/v1");
    }

    void test() override {
        qDebug() << "test kucoin";
    }

    QStringList getMarketsList() override {
        url = QUrl("https://api.kucoin.com/api/v2");
        setPathUrl("symbols");
        QJsonObject json = this->fetchQuery(url);
        QStringList markets;
        if (json["qt2_status"] == "ok") {
            QJsonArray symbols = json["data"].toArray();
            for (const QJsonValue &value : symbols) {
                QJsonObject obj = value.toObject();
                markets << obj["symbol"].toString().replace("-", "/");
            }
        }
        else {  qDebug() << json["qt2_error"];  }
        return markets;
    }

private:

    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + "-" + base.toUpper();
        setPathUrl("market/orderbook/level1");
        QUrlQuery query;
        query.addQueryItem("symbol", market);
        url.setQuery(query);
        QJsonObject json = this->fetchQuery(url);
        if (json["qt2_status"] == "ok") {   return json["data"].toObject()["price"].toString().toDouble();  }
        else {  qDebug() << json["qt2_error"];  }
        return 0.0;
    }

};

#endif // KUCOIN_H
