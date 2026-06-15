#pragma once
#include <QSqlDatabase>
#include "src/tvscreener.h"

class DbManager {
public:
    static DbManager& getInstance();
    bool init(const QString& path = "qtradingview2.db");
    void saveStocks(const QString& market, const QList<Stock>& stocks);
    QList<Stock> loadStocks(const QString& market);

private:
    DbManager() = default;
    QSqlDatabase m_db;
    void createTables();
};