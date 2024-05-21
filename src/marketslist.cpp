#include "marketslist.h"
#include <QList>
#include "src/settings.h"



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
    QAction *actionAddPosition = new QAction("Add to portfolio...", this);

    connect(actionLoadChart, &QAction::triggered, this, &MenuContextual::loadChart);
    connect(actionDeleteMarket, &QAction::triggered, this, &MenuContextual::deleteMarket);
    connect(actionAddPosition, &QAction::triggered, this, &MenuContextual::addPosition);

    menu->addAction(actionLoadChart);
    menu->addAction(actionDeleteMarket);
    menu->addSeparator();
    menu->addAction(actionAddPosition);

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

void MenuContextual::addPosition() {
    QListWidgetItem *item = listWidget->currentItem();
    emit sigAddPosition(item);
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
