#include "tvscreener.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>


TvScreener::TvScreener(QObject* parent) : QObject(parent), m_nam(new QNetworkAccessManager(this)) {}


void TvScreener::fetchMarket(const QString& market, int offset, int limit) {
    QUrl url(QString("https://scanner.tradingview.com/%1/scan").arg(market));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload {
        {"columns", QJsonArray{"name", "description", "close", "volume", "market_cap_basic", "sector"}},
        {"range",   QJsonArray{offset, limit}},
        {"sort",    QJsonObject{{"sortBy", "market_cap_basic"}, {"sortOrder", "desc"}}}
    };

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(payload).toJson());

    // Guardar el market en la reply para recuperarlo luego
    reply->setProperty("market", market);

    connect(reply, &QNetworkReply::finished, this, &TvScreener::onReplyFinished);
}

void TvScreener::onReplyFinished() {
    auto* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        return;
    }

    QString market = reply->property("market").toString();
    QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();

    int totalCount = root["totalCount"].toInt();
    QList<Stock> stocks;

    for (const QJsonValue& item : root["data"].toArray()) {
        QJsonObject obj = item.toObject();
        QJsonArray  d   = obj["d"].toArray();

        stocks.append({
            .ticker    = obj["s"].toString(),
            .name      = d[0].toString(),
            .description = d[1].toString(),
            .close     = d[2].toDouble(),
            .volume    = d[3].toDouble(),
            .marketCap = d[4].toDouble(),
            .sector    = d[5].toString()
        });
    }

    emit dataReady(market, stocks, totalCount);
}