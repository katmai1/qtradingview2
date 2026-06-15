#ifndef TVSCREENER_H
#define TVSCREENER_H

#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonArray>


struct Stock {
    QString ticker;
    QString name;
    QString description;
    double  close;
    double  volume;
    double  marketCap;
    QString sector;
};


class TvScreener : public QObject
{
    Q_OBJECT

public:
    explicit TvScreener(QObject* parent = nullptr);
    void fetchMarket(const QString& market, int offset = 0, int limit = 500);

signals:
    void dataReady(const QString& market, const QList<Stock>& stocks, int totalCount);
    void errorOccurred(const QString& message);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager* m_nam;
};

#endif // TVSCREENER_H
