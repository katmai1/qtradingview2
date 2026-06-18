#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DbManager& DbManager::getInstance() {
    static DbManager instance;
    return instance;
}

bool DbManager::init(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qWarning() << "Error abriendo BD:" << m_db.lastError().text();
        return false;
    }

    createTables();
    return true;
}

// create tables if not exists
void DbManager::createTables() {
    QSqlQuery q;

    // tabla stocks
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS stocks (
            id         INTEGER PRIMARY KEY AUTOINCREMENT,
            market     TEXT NOT NULL,
            ticker     TEXT NOT NULL,
            name       TEXT,
            description TEXT,
            isin        TEXT,
            close      REAL,
            volume     REAL,
            market_cap REAL,
            sector     TEXT,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(market, ticker)
        )
    )");

    // tabla de crypto
    q.exec(R"(
        CREATE TABLE IF NOT EXISTS crypto (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            exchange    TEXT NOT NULL,
            ticker      TEXT NOT NULL,
            name        TEXT,
            description TEXT,
            close       REAL,
            volume      REAL,
            updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(exchange, ticker)
        )
    )");

    migrateStocks();
}

void DbManager::migrateStocks() {
    QSqlQuery q;

    // Obtener columnas actuales
    q.exec("PRAGMA table_info(stocks)");
    QStringList existing;
    while (q.next())
        existing << q.value(1).toString(); // nombre de columna

    // Añadir solo las que faltan
    if (!existing.contains("isin")) {
        q.exec("ALTER TABLE stocks ADD COLUMN isin TEXT");
        qInfo() << "ISIN column added";
    }
}


void DbManager::saveStocks(const QString& market, const QList<Stock>& stocks) {
    m_db.transaction();

    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO stocks (market, ticker, name, description, isin, close, volume, market_cap, sector)
        VALUES (:market, :ticker, :name, :description, :isin, :close, :volume, :market_cap, :sector)
        ON CONFLICT(market, ticker) DO UPDATE SET
            name       = excluded.name,
            description = excluded.description,
            isin       = excluded.isin,
            close      = excluded.close,
            volume     = excluded.volume,
            market_cap = excluded.market_cap,
            sector     = excluded.sector,
            updated_at = CURRENT_TIMESTAMP
    )");

    for (const auto& s : stocks) {
        q.bindValue(":market",     market);
        q.bindValue(":ticker",     s.ticker);
        q.bindValue(":name",       s.name);
        q.bindValue(":description", s.description);
        q.bindValue(":isin",        s.isin);
        q.bindValue(":close",      s.close);
        q.bindValue(":volume",     s.volume);
        q.bindValue(":market_cap", s.marketCap);
        q.bindValue(":sector",     s.sector);
        q.exec();
    }

    m_db.commit();
    qInfo() << "Guardados" << stocks.size() << "stocks de" << market;
}

void DbManager::saveCrypto(const QString& exchange, const QList<Stock>& stocks) {
    m_db.transaction();
    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO crypto (exchange, ticker, name, description, close, volume)
        VALUES (:exchange, :ticker, :name, :description, :close, :volume)
        ON CONFLICT(exchange, ticker) DO UPDATE SET
            name        = excluded.name,
            description = excluded.description,
            close       = excluded.close,
            volume      = excluded.volume,
            updated_at  = CURRENT_TIMESTAMP
    )");

    for (const auto& s : stocks) {
        q.bindValue(":exchange",    exchange);
        q.bindValue(":ticker",      s.ticker);
        q.bindValue(":name",        s.name);
        q.bindValue(":description", s.description);
        q.bindValue(":close",       s.close);
        q.bindValue(":volume",      s.volume);
        q.exec();
    }

    m_db.commit();
    qInfo() << "Crypto guardados:" << stocks.size() << "en" << exchange;
}

QList<Stock> DbManager::loadStocks(const QString& market) {
    QList<Stock> result;
    QSqlQuery q;
    q.prepare("SELECT ticker, name, description, isin, close, volume, market_cap, sector FROM stocks WHERE market = :market");
    q.bindValue(":market", market);
    q.exec();

    while (q.next()) {
        result.append({
            .ticker      = q.value(0).toString(),
            .name        = q.value(1).toString(),
            .description = q.value(2).toString(),
            .isin        = q.value(3).toString(),
            .close       = q.value(4).toDouble(),
            .volume      = q.value(5).toDouble(),
            .marketCap   = q.value(6).toDouble(),
            .sector      = q.value(7).toString()
        });
    }
    return result;
}

QList<Stock> DbManager::loadCrypto(const QString& exchange = "BINANCE") {
    QList<Stock> result;
    QSqlQuery q;
    q.prepare("SELECT ticker, name, description, close, volume FROM crypto WHERE exchange = :exchange");
    q.bindValue(":exchange", exchange);
    q.exec();

    while (q.next()) {
        result.append({
            .ticker      = q.value(0).toString(),
            .name        = q.value(1).toString(),
            .description = q.value(2).toString(),
            .close       = q.value(3).toDouble(),
            .volume      = q.value(4).toDouble(),
        });
    }
    return result;
}