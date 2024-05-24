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
    explicit Kucoin(QObject *parent = nullptr) : ExchangeBase(parent) {}

    void test() override {
        qDebug() << "test kucoin";
    }

    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + "-" + base.toUpper();
        QUrl url = "https://api.kucoin.com/api/v1/market/orderbook/level1?symbol=" + market;
        return this->fetchPrice(url);
    }


private:

    double parsePriceFromJson(const QByteArray &jsonData) override {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("data")) {
            return jsonObj["data"].toObject()["price"].toString().toDouble();
        } else {
            qDebug() << "Error: Invalid response from Kucoin API";
            return -1.0; // Indicador de error
        }
    }
};

#endif // KUCOIN_H
