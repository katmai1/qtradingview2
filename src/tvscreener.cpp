#include "tvscreener.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QNetworkCookieJar>


TvScreener::TvScreener(QWebEngineProfile* profile, QObject* parent)
    : QObject(parent),
    m_nam(new QNetworkAccessManager(this))
{
    m_cookieJar = new QNetworkCookieJar(this);
    m_nam->setCookieJar(m_cookieJar);

    // Copiar cookies del perfil WebEngine al cookieJar del screener
    profile->cookieStore()->loadAllCookies();
    connect(profile->cookieStore(), &QWebEngineCookieStore::cookieAdded,
            this, [this](const QNetworkCookie& cookie) {
                if (cookie.domain().contains("tradingview.com"))
                    m_cookieJar->insertCookie(cookie);
            });

}


void TvScreener::fetchMarket(const QString& market, int offset, int limit) {
    QUrl url(QString("https://scanner.tradingview.com/%1/scan").arg(market));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload {
        {"columns", QJsonArray{"name", "description", "isin", "close", "volume", "market_cap_basic", "sector"}},
        {"range",   QJsonArray{offset, limit}},
        {"sort",    QJsonObject{{"sortBy", "market_cap_basic"}, {"sortOrder", "desc"}}}
    };

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(payload).toJson());

    // Guardar el market en la reply para recuperarlo luego
    reply->setProperty("market", market);

    connect(reply, &QNetworkReply::finished, this, &TvScreener::onReplyFinished);
}

void TvScreener::fetchCrypto(const QString& exchange) {
    QJsonObject payload {
        {"columns", QJsonArray{"name", "description", "close", "volume", "market_cap_basic"}},
        {"range",   QJsonArray{0, 500}},
        {"sort",    QJsonObject{{"sortBy", "volume"}, {"sortOrder", "desc"}}},
        {"filter",  QJsonArray{
                       QJsonObject{
                           {"left",      "exchange"},
                           {"operation", "equal"},
                           {"right",     exchange}
                       }
                   }}
    };

    QUrl url("https://scanner.tradingview.com/crypto/scan");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_nam->post(request, QJsonDocument(payload).toJson());
    reply->setProperty("market", QString("crypto_%1").arg(exchange));
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
            .isin      = d[2].toString(),
            .close     = d[3].toDouble(),
            .volume    = d[4].toDouble(),
            .marketCap = d[5].toDouble(),
            .sector    = d[6].toString(),
        });
    }

    emit dataReady(market, stocks, totalCount);
}