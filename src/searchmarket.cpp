#include "searchmarket.h"
#include "ui_searchmarket.h"
#include <QStyledItemDelegate>
#include <QLocale>


// delegate para convertir valores a num comprensibles
class NumberDelegate : public QStyledItemDelegate {
public:
    enum Format { Volume, MarketCap };

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

        // Volume
        if (v >= 1e6) return QString("%1 M").arg(locale.toString(v / 1e6, 'f', 2));
        if (v >= 1e3) return QString("%1 K").arg(locale.toString(v / 1e3, 'f', 2));
        return locale.toString(v, 'f', 0);
    }

private:
    Format m_format;
};



searchMarket::searchMarket(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchMarket)
{
    ui->setupUi(this);

    m_model = new QSqlQueryModel(this);
    setupStocks();
    ui->tableStocks->setModel(m_model);

    connect(ui->tableStocks, &QTableView::doubleClicked, this, [this](const QModelIndex& index) {
        int row = index.row();
        QString ticker = m_model->data(m_model->index(row, 1)).toString();
        qDebug() << "Doble click en:" << ticker;
        // tu código aquí
    });
}

searchMarket::~searchMarket()
{
    delete ui;
}

void searchMarket::setupStocks() {
    setWindowTitle("Stocks");
    auto* model = new QSqlQueryModel(this);
    model->setQuery(R"(
        SELECT market, ticker, name, description, isin, close, volume, market_cap, sector, updated_at
        FROM stocks
        ORDER BY market_cap DESC
    )");

    model->setHeaderData(0,  Qt::Horizontal, "Mercado");
    //model->setHeaderData(1,  Qt::Horizontal, "Ticker");
    model->setHeaderData(2,  Qt::Horizontal, "Nombre");
    model->setHeaderData(3,  Qt::Horizontal, "Descripción");
    // model->setHeaderData(4,  Qt::Horizontal, "ISIN");
    model->setHeaderData(5,  Qt::Horizontal, "Cierre");
    model->setHeaderData(6,  Qt::Horizontal, "Volumen");
    model->setHeaderData(7,  Qt::Horizontal, "Market Cap");
    model->setHeaderData(8,  Qt::Horizontal, "Sector");
    model->setHeaderData(9,  Qt::Horizontal, "Actualizado");

    m_model = model;
    ui->tableStocks->setModel(m_model);
    // convierte volumen y market a valores comprensibles
    ui->tableStocks->setItemDelegateForColumn(6, new NumberDelegate(NumberDelegate::Volume,    this));
    ui->tableStocks->setItemDelegateForColumn(7, new NumberDelegate(NumberDelegate::MarketCap, this));
    ui->tableStocks->resizeColumnsToContents();
    ui->tableStocks->setColumnWidth(3, 200);
    // hide columns
    ui->tableStocks->setColumnHidden(1, true);
    ui->tableStocks->setColumnHidden(4, true);
    ui->tableStocks->setColumnHidden(9, true);
}


void searchMarket::setupCryptos() {
    setWindowTitle("Crypto");
    // m_model->setTable("crypto");
    // m_model->setSort(4, Qt::DescendingOrder); // volume desc
    // m_model->select();

    // m_model->setHeaderData(1, Qt::Horizontal, "Exchange");
    // m_model->setHeaderData(2, Qt::Horizontal, "Ticker");
    // m_model->setHeaderData(3, Qt::Horizontal, "Nombre");
    // m_model->setHeaderData(4, Qt::Horizontal, "Descripción");
    // m_model->setHeaderData(5, Qt::Horizontal, "Cierre");
    // m_model->setHeaderData(6, Qt::Horizontal, "Volumen");
    // m_model->setHeaderData(7, Qt::Horizontal, "Actualizado");
}