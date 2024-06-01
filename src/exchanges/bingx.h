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
    explicit Bingx(QObject *parent = nullptr) : ExchangeBase(parent) {
        url = QUrl("https://open-api.bingx.com/openApi/swap/v1");
    }

    void test() override {
        qDebug() << "test bingx";
    }

private:

    double _getPrice(const QString &symbol, const QString &base) override {
        QString market = symbol.toUpper() + "-" + base.toUpper();
        setPathUrl("ticker/price");
        QUrlQuery query;
        query.addQueryItem("symbol", market);
        url.setQuery(query);
        QJsonObject json = this->fetchQuery(url);
        if (json["qt2_status"] == "ok") {   return json["data"].toObject()["price"].toString().toDouble();  }
        else {  qDebug() << json["qt2_error"];  }
        return 0.0;
    }

};

#endif // BINGX_H
