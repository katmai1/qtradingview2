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
    virtual ~ExchangeBase() {};

    virtual double getPrice(const QString &symbol, const QString &base) {
        QString market = symbol.toUpper() + "/" + base.toUpper();
        qInfo() << "No se puede obtener precio del market "<< market
                    << " sin haber definido antes un exchange valido";
        return -1.0;
    };

    virtual void test() {
        qDebug() << "funcion de test de la clase ExchangeBase";
    }

    double fetchPrice(QUrl url) {
        manager = new QNetworkAccessManager(this);
        QNetworkRequest request;
        request.setUrl(url);

        QNetworkReply *reply = manager->get(request);

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            double price = parsePriceFromJson(data);
            reply->deleteLater();
            return price;
        } else {
            qDebug() << "Error:" << reply->errorString();
            reply->deleteLater();
            return -1.0; // Indicador de error
        }
    }

    virtual double parsePriceFromJson(const QByteArray &jsonData) {
        Q_UNUSED(jsonData);
        return -1.0;
    };

private:
    QNetworkAccessManager *manager;

};


#endif // EXCHANGE_BASE_H
