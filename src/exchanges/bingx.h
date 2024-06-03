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
        url = QUrl("https://open-api.bingx.com/openApi/spot/v1");
    }

    void test() override {
        qDebug() << "test bingx";
    }

    QStringList getMarketsList() override {
        setPathUrl("common/symbols");
        QJsonObject json = this->fetchQuery(url);
        QStringList markets;
        if (json["qt2_status"] == "ok") {
            QJsonArray symbols = json["data"].toObject()["symbols"].toArray();
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
        setPathUrl("ticker/24hr");
        QUrlQuery query;
        // Obtener la fecha y hora actual
        QDateTime fechaHoraActual = QDateTime::currentDateTime();
        qint64 timestamp = fechaHoraActual.toSecsSinceEpoch();
        QString timestampStr = QString::number(timestamp);

        query.addQueryItem("symbol", market);
        query.addQueryItem("timestamp", timestampStr);
        url.setQuery(query);
        QJsonObject json = this->fetchQuery(url);
        if (json["qt2_status"] == "ok") {   return json["data"].toArray().at(0).toObject()["lastPrice"].toDouble();  }
        else {  qDebug() << json["qt2_error"];  }
        return 0.0;
    }

};

#endif // BINGX_H
