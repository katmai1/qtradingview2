#include "marketslist.h"
#include "qdebug.h"

#include <QList>

#include "qglobal.h"
#include "src/settings.h"
#include "src/exmanager.h"



// le pasamos como parametro el market en bruto
// nos devuelve un item con el icono y el tooltip del exchange y como texto el market
QListWidgetItem *getItem(const QString &market) {
    QStringList data = market.split(":");
    QString pair = data[0].toLower();
    QString exchange = data[1].toLower();
    QString tooltip = exchange;
    tooltip[0] = tooltip[0].toUpper();
    QIcon icon(":exchanges/icons/"+exchange);

    QListWidgetItem *item = new QListWidgetItem(pair.toUpper());
    item->setToolTip(tooltip);
    item->setIcon(icon);
    return item;
}


// guarda lista en el fichero settings
void MarketsList::saveList()
{
    SettingsManager settings;
    QStringList lista;
    for (int i = 0; i < this->m_list->count(); ++i) {
        QListWidgetItem* item = this->m_list->item(i);
        lista << item->text().toUpper()+":"+item->toolTip().toUpper();
    }
    settings.setValue("markets", lista);

    qInfo() << "Lista de mercados guardada";
}

// devuelve true si ese market ya existe
bool MarketsList::existMarket(QString market)
{
    // obtiene item del market y hace la busqueda
    QListWidgetItem *item = getItem(market);
    QList<QListWidgetItem *> lista = this->m_list->findItems(item->text(), Qt::MatchExactly);

    // si la busqueda tiene algun resultado y ademas el exchange coincide devuelve true
    if (lista.count() > 0) {
        for (QListWidgetItem *market : lista) {
            qInfo() << market->text();
            if (market->toolTip() == item->toolTip()) { return true;    }
        }
    }
    return false;
}

// añade market a la lista
void MarketsList::addMarket(QString market)
{
    QListWidgetItem *item = getItem(market);
    this->m_list->addItem(item);
}


// carga lista desde fichero settings
void MarketsList::loadList() {
    SettingsManager settings;
    this->m_list->clear();
    QStringList lista = settings.getListMarkets();
    foreach (const QString &market, lista) {
        this->addMarket(market);
    }
}

// ***************************************************
// Menu conextual

MenuContextual::MenuContextual(QListWidget *parent) : QObject(parent)
{
    listWidget = parent;

    menu = new QMenu(parent);
    QAction *actionLoadChart = new QAction("Load chart...", this);
    QAction *actionDeleteMarket = new QAction("Delete...", this);
    QAction *actionGetPrice = new QAction("Get price", this);

    connect(actionLoadChart, &QAction::triggered, this, &MenuContextual::loadChart);
    connect(actionDeleteMarket, &QAction::triggered, this, &MenuContextual::deleteMarket);
    connect(actionGetPrice, &QAction::triggered, this, &MenuContextual::getPrice);

    menu->addAction(actionLoadChart);
    menu->addAction(actionDeleteMarket);
    menu->addSeparator();
    menu->addAction(actionGetPrice);

    parent->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(parent, &QListWidget::customContextMenuRequested, this, &MenuContextual::showMenu);
}

MenuContextual::~MenuContextual()
{
    delete menu;
}

void MenuContextual::showMenu(const QPoint &pos)
{
    QPoint globalPos = listWidget->mapToGlobal(pos);
    menu->exec(globalPos);
}

void MenuContextual::loadChart()
{
    QListWidgetItem *item = listWidget->currentItem();
    emit sigLoadMarket(item);
}

void MenuContextual::deleteMarket()
{
    QListWidgetItem *item = listWidget->currentItem();
    delete item;
    emit sigSaveMarketsList();
}

void MenuContextual::getPrice() {
    QListWidgetItem *item = listWidget->currentItem();
    QString pair = item->text();
    QString exchange = item->toolTip();
    QStringList market = pair.split("/");
    ExManager exman;
    ExchangeBase* ex = exman.setExchange(exchange.toLower());
    qDebug() << ex->getPrice(market[0], market[1]);
    delete ex;
}
