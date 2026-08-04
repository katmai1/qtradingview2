#pragma once
#include <QSqlDatabase>
#include "tvscreener.h"

// item de la watchlist, ya con los datos de "stocks" incorporados via JOIN
struct WatchItem {
    QString ticker;
    QString type;
    QString tag;
    QString notes;
    QString name;
    QString description;
    QString isin;
};

class DbManager {
public:
    static DbManager& getInstance();

    // singleton: sin copia ni asignación
    DbManager(const DbManager&) = delete;
    DbManager& operator=(const DbManager&) = delete;

    bool init();

    void saveStocks(const QString& market, const QList<Stock>& stocks);
    void saveCrypto(const QString& exchange, const QList<Crypto>& stocks);
    void migrateStocks();

    QList<Stock> loadStocks(const QString& market);
    QList<Crypto> loadCrypto(const QString& exchange = "BINANCE");

    Stock getStockByTicker(const QString& ticker);
    QList<WatchItem> loadWatchlist();
    bool deleteWLbyTicker(const QString& ticker);
    bool updateTag(const QString& ticker, const QString& tag);
    bool updateNotes(const QString& ticker, const QString& notes);

private:
    DbManager() = default;
    QSqlDatabase m_db;
    void createTables();
};