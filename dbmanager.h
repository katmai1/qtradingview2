#pragma once
#include <QSqlDatabase>
#include "src/tvscreener.h"


class DbManager {
public:
    static DbManager& getInstance();
    bool init();

    void saveStocks(const QString& market, const QList<Stock>& stocks);
    void saveCrypto(const QString& exchange, const QList<Crypto>& stocks);
    void migrateStocks();

    QList<Stock> loadStocks(const QString& market);
    QList<Crypto> loadCrypto(const QString& exchange);

    Stock getStockByTicker(const QString& ticker);
    // Crypto getCryptoByTicker(const QString& ticker);

private:
    DbManager() = default;
    QSqlDatabase m_db;
    void createTables();
};