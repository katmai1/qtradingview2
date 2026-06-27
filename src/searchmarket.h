#ifndef SEARCHMARKET_H
#define SEARCHMARKET_H

#include <QDialog>
#include <QSqlQueryModel>
#include "tvscreener.h"

enum class AssetType { Stocks, Crypto };
namespace Ui {  class searchMarket; }


class searchMarket : public QDialog
{
    Q_OBJECT

public:
    explicit searchMarket(AssetType type, TvScreener* screener, QWidget *parent = nullptr);
    ~searchMarket();

private slots:
    void on_butonUpdate_released();

    void on_butAdd2WL_released();

signals:
    void loadSymbol(const QString& ticker);
    void refreshWatchList();

private:
    Ui::searchMarket *ui;
    QSqlQueryModel* m_model;
    QString m_query;
    QString m_where;
    void setupStocks();
    void setupCryptos();
    AssetType m_type;
    TvScreener* m_screener;
    void add2WL(QString ticker, QString type);
};

#endif // SEARCHMARKET_H
