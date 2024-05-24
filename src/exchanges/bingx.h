#ifndef BINGX_H
#define BINGX_H

#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "exchange_base.h"


class Bingx : public ExchangeBase {


public:
    explicit Bingx(QObject *parent = nullptr) : ExchangeBase(parent) {}

    void test() override {
        qDebug() << "test bingx";
    }

    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + "-" + base.toUpper();
        QUrl url = "https://open-api.bingx.com/openApi/swap/v1/ticker/price?symbol=" + market;
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

#endif // BINGX_H
