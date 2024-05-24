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
    explicit Binance(QObject *parent = nullptr) : ExchangeBase(parent) {}

    void test() override {
        qDebug() << "test binace";
    }

    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + base.toUpper();
        QUrl url = "https://api.binance.com/api/v3/ticker/price?symbol=" + market;
        return this->fetchPrice(url);
    }

private:

    double parsePriceFromJson(const QByteArray &jsonData) override {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("price")) {
            return jsonObj["price"].toString().toDouble();
        } else {
            qDebug() << "Error: Invalid response from Binance API";
            return -1.0; // Indicador de error
        }
    }
};

#endif // BINANCE_H
