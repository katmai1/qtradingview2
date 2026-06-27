#include "dockwatchlist.h"
#include "ui_dockwatchlist.h"
#include "dbmanager.h"


dockWatchList::dockWatchList(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::dockWatchList)
{
    ui->setupUi(this);
    updateList();

    connect(ui->watchList, &QListWidget::itemDoubleClicked, this, &dockWatchList::onItemDoubleClicked);
}

dockWatchList::~dockWatchList()
{
    delete ui;
}

void dockWatchList::updateList() {
    // get all wl
    auto* q = new QSqlQuery();
    q->exec("SELECT * FROM watch");

    // crea lista
    QList<WatchItem> itemList;
    while (q->next()) {
        WatchItem item;
        item.ticker = q->value(1).toString();
        item.type = q->value(2).toString();
        item.tag = q->value(3).toString();
        item.notes = q->value(4).toString();

        Stock data = DbManager::getInstance().getStockByTicker(item.ticker);
        if (!data.name.isEmpty()) {
            item.name = data.name;
        }
        itemList.append(item);

        qDebug() << QString("%1 (%2)").arg(item.name, item.ticker);
    }
    refreshList(itemList);
}

void dockWatchList::refreshList(QList<WatchItem>& lista) {
    ui->watchList->clear();
    for (const WatchItem& item: lista) {
        QListWidgetItem* qItem = new QListWidgetItem(item.name);
        qItem->setData(Qt::UserRole, item.ticker);
        ui->watchList->addItem(qItem);
    }

}

void dockWatchList::onItemDoubleClicked(QListWidgetItem* item) {
    QString ticker = item->data(Qt::UserRole).toString();
    qDebug() << ticker;
    emit loadSymbol(ticker);
}