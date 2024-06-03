#ifndef EXCHANGE_BASE_H
#define EXCHANGE_BASE_H

#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>



class ExchangeBase : public QObject{
    Q_OBJECT

public:
    explicit ExchangeBase(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~ExchangeBase() {}

    double getPrice(const QString &market) {
        QStringList marketParts = market.split("/");
        return _getPrice(marketParts.value(0).toUpper(), marketParts.value(1).toUpper());
    }

    virtual QStringList getMarketsList() {
        QStringList marketsList;
        qDebug() << "funcion getMarketsList de la clase ExchangeBase";
        return marketsList;
    }

    virtual void test() {
        qDebug() << "funcion de test de la clase ExchangeBase";
    }

private:
    QNetworkAccessManager *manager;

    virtual double _getPrice(const QString &symbol, const QString &base) {
        QString market = symbol.toUpper() + "/" + base.toUpper();
        qInfo() << "No se puede obtener precio del market "<< market
                << " sin haber definido antes un exchange valido";
        return -1.0;
    }

protected:
    QUrl url;

    void setPathUrl(const QString path) {   url.setPath(url.path() + "/" + path);   }

    // fetch query to api and return raw data
    QJsonObject fetchQuery(const QUrl url) {
        // crea el query y lo envia
        manager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        request.setUrl(url);
        QNetworkReply *reply = manager->get(request);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        // comprueba si da error o no y devuelve un json
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject json = jsonDoc.object();
            json["qt2_status"] = "ok";
            return json;
        } else {
            QJsonObject json;
            json["qt2_url"] = url.toString();
            json["qt2_status"] = "error";
            json["qt2_error"] = reply->errorString();
            reply->deleteLater();
            return json;
        }
    }
};
#endif // EXCHANGE_BASE_H
