#ifndef TICKER_H
#define TICKER_H

#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qurlquery.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <functional>
#include <QDebug>

class HttpManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpManager(QObject *parent = nullptr) : QObject(parent) {}

    void getRequest(const QUrl &url, std::function<void(QByteArray)> onSuccess,
                    std::function<void(QString)> onError) {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);

        QNetworkReply *reply = manager->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                onSuccess(responseData);
            } else {
                onError(reply->errorString());
            }
            reply->deleteLater();
        });
    }

    void getPrice(QString symbol, QString base) {
        // URL de la solicitud HTTP
        QUrl url("https://min-api.cryptocompare.com/data/price");

        // Construir los parámetros de la consulta
        QUrlQuery query;
        query.addQueryItem("fsym", symbol.toUpper());
        query.addQueryItem("tsyms", base.toUpper());
        url.setQuery(query.query());

        // Definir la función de devolución de llamada para el éxito de la solicitud
        auto onSuccess = [base](QByteArray responseData) {
            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(responseData, &error);
            QJsonObject jsonObject = jsonDocument.object();

            qDebug() << "Response:" << jsonObject.value(base.toUpper()).toDouble();;
            // Aquí puedes procesar la respuesta, por ejemplo, actualizar la interfaz de usuario
        };

        // Definir la función de devolución de llamada para el error de la solicitud
        auto onError = [](QString errorString) {
            qDebug() << "Error:" << errorString;
        };

        // Realizar la solicitud HTTP
        this->getRequest(url, onSuccess, onError);
    }
};


#endif // TICKER_H
