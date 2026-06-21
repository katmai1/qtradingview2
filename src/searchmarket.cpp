#include "searchmarket.h"
#include "ui_searchmarket.h"
#include <QStyledItemDelegate>
#include <QLocale>

// delegate para convertir valores a num comprensibles
class NumberDelegate : public QStyledItemDelegate {
public:
    enum Format { Volume, MarketCap, CloseCrypto };

    NumberDelegate(Format format, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), m_format(format) {}

    QString displayText(const QVariant& value, const QLocale&) const override {
        double v = value.toDouble();
        QLocale locale;

        if (m_format == MarketCap) {
            if (v >= 1e12) return QString("%1 T").arg(locale.toString(v / 1e12, 'f', 2));
            if (v >= 1e9)  return QString("%1 B").arg(locale.toString(v / 1e9,  'f', 2));
            if (v >= 1e6)  return QString("%1 M").arg(locale.toString(v / 1e6,  'f', 2));
            return locale.toString(v, 'f', 0);
        }

        if (m_format == CloseCrypto) {  return locale.toString(v, 'f', 8);  }

        // Volume
        if (v >= 1e6) return QString("%1 M").arg(locale.toString(v / 1e6, 'f', 2));
        if (v >= 1e3) return QString("%1 K").arg(locale.toString(v / 1e3, 'f', 2));
        return locale.toString(v, 'f', 0);
    }

private:
    Format m_format;
};



searchMarket::searchMarket(AssetType type, TvScreener* screener, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchMarket)
    , m_screener(screener)
{
    ui->setupUi(this);

    m_model = new QSqlQueryModel(this);
    m_type = type;

    if (type == AssetType::Stocks) {
        m_query = R"(
            SELECT market, ticker, name, description, isin, close, volume, market_cap, sector, updated_at
            FROM stocks
            %1
            ORDER BY market_cap DESC
        )";
        setupStocks();
    }
    else {
        m_query = R"(
            SELECT exchange, ticker, name, description, close, volume, updated_at
            FROM crypto
            %1
            ORDER BY volume DESC
        )";
        setupCryptos();
    }

    connect(ui->tableMarkets, &QTableView::doubleClicked, this, [this](const QModelIndex& index) {
        int row = index.row();
        QString ticker = m_model->data(m_model->index(row, 1)).toString();
        emit loadSymbol(ticker);
        qDebug() << "Doble click en:" << ticker;
    });

    connect(ui->editFilter, &QLineEdit::textChanged, this, [this](const QString& text) {
        QString filter = text.trimmed();
        QString where;
        if (!filter.isEmpty()) {
            if (m_type == AssetType::Stocks) {
                where = QString("WHERE ticker LIKE '%%1%' OR name LIKE '%%1%' OR isin LIKE '%%1%' OR description LIKE '%%1%'").arg(filter);
            }
            else {
                where = QString("WHERE ticker LIKE '%%1%' OR name LIKE '%%1%' OR description LIKE '%%1%'").arg(filter);
            }

        }
        m_model->setQuery(m_query.arg(where));
    });
    // conectar el signal para refrescar la tabla cuando lleguen datos
    if (m_type == AssetType::Stocks) {
        connect(m_screener, &TvScreener::stocksReady, this, [this]() {
            m_model->setQuery(m_query.arg(""));
        });
    } else {
        connect(m_screener, &TvScreener::cryptoReady, this, [this]() {
            m_model->setQuery(m_query.arg(""));
        });
    }
}

searchMarket::~searchMarket()
{
    delete ui;
}

// carga stocks
void searchMarket::setupStocks() {
    setWindowTitle("Search Stocks");
    auto* model = new QSqlQueryModel(this);
    model->setQuery(m_query.arg(""));

    model->setHeaderData(0,  Qt::Horizontal, "Mercado");
    //model->setHeaderData(1,  Qt::Horizontal, "Ticker");
    model->setHeaderData(2,  Qt::Horizontal, "Nombre");
    model->setHeaderData(3,  Qt::Horizontal, "Descripción");
    // model->setHeaderData(4,  Qt::Horizontal, "ISIN");
    model->setHeaderData(5,  Qt::Horizontal, "Cierre");
    model->setHeaderData(6,  Qt::Horizontal, "Volumen");
    model->setHeaderData(7,  Qt::Horizontal, "Market Cap");
    model->setHeaderData(8,  Qt::Horizontal, "Sector");
    // model->setHeaderData(9,  Qt::Horizontal, "Actualizado");

    m_model = model;
    ui->tableMarkets->setModel(m_model);
    // convierte volumen y market a valores comprensibles
    ui->tableMarkets->setItemDelegateForColumn(6, new NumberDelegate(NumberDelegate::Volume,    this));
    ui->tableMarkets->setItemDelegateForColumn(7, new NumberDelegate(NumberDelegate::MarketCap, this));
    ui->tableMarkets->resizeColumnsToContents();
    ui->tableMarkets->setColumnWidth(3, 200);
    // hide columns
    ui->tableMarkets->setColumnHidden(1, true);
    ui->tableMarkets->setColumnHidden(4, true);
    ui->tableMarkets->setColumnHidden(9, true);
}


void searchMarket::setupCryptos() {
    setWindowTitle("Crypto");
    auto* model = new QSqlQueryModel(this);
    model->setQuery(m_query.arg(""));

    model->setHeaderData(0, Qt::Horizontal, "Exchange");
    model->setHeaderData(1, Qt::Horizontal, "Ticker");
    model->setHeaderData(2, Qt::Horizontal, "Nombre");
    model->setHeaderData(3, Qt::Horizontal, "Descripción");
    model->setHeaderData(4, Qt::Horizontal, "Cierre");
    model->setHeaderData(5, Qt::Horizontal, "Volumen");
    model->setHeaderData(6, Qt::Horizontal, "Actualizado");

    m_model = model;
    ui->tableMarkets->setModel(m_model);
    ui->tableMarkets->setItemDelegateForColumn(4, new NumberDelegate(NumberDelegate::CloseCrypto,    this));
    ui->tableMarkets->setItemDelegateForColumn(5, new NumberDelegate(NumberDelegate::Volume,    this));
}
void searchMarket::on_butonUpdate_released()
{
    if (m_type == AssetType::Stocks) {
        m_screener->fetchMarket("germany");
        m_screener->fetchMarket("france");
        m_screener->fetchMarket("uk");
        // etc.
    } else {
        m_screener->fetchCrypto("BINANCE");
    }
}

