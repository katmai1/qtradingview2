#include "dockwatchlist.h"
#include "ui_dockwatchlist.h"
#include "dbmanager.h"
#include "watchlistdelegate.h"

#include <QClipboard>
#include <QApplication>
#include <QMenu>

dockWatchList::dockWatchList(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::dockWatchList)
{
    ui->setupUi(this);

    // cargamos tags en el combo y conectamos
    ui->comboTag->addItem("Todos");
    for (const QString& tag : watchlistTagOrder()) { ui->comboTag->addItem(tag);    }
    connect(ui->comboTag, &QComboBox::currentTextChanged, this, &dockWatchList::onFilterTag);

    // configuramos lista, actualizamos y conectamos
    ui->watchList->setItemDelegate(new WatchListDelegate(this));
    ui->watchList->setAlternatingRowColors(true);
    updateList();
    connect(ui->watchList, &QListWidget::itemDoubleClicked, this, &dockWatchList::onItemDoubleClicked);

    // menu contextual
    ui->watchList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->watchList, &QListWidget::customContextMenuRequested, this, &dockWatchList::onContextMenu);
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
            item.description = data.description;
            item.isin = data.isin;
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
        qItem->setData(Qt::UserRole + 1, item.name);
        qItem->setData(Qt::UserRole + 2, item.description);
        qItem->setData(Qt::UserRole + 3, item.tag);
        qItem->setData(Qt::UserRole + 4, item.notes);
        qItem->setData(Qt::UserRole + 5, item.isin);
        ui->watchList->addItem(qItem);
    }

}

void dockWatchList::onItemDoubleClicked(QListWidgetItem* item) {
    QString ticker = item->data(Qt::UserRole).toString();
    qDebug() << ticker;
    emit loadSymbol(ticker);
}

void dockWatchList::onFilterTag(const QString& tag) {
    for (int i = 0; i < ui->watchList->count(); i++) {
        QListWidgetItem* item = ui->watchList->item(i);
        QString itemTag = item->data(Qt::UserRole + 3).toString();
        bool visible = (tag == "Todos") || (itemTag == tag);
        item->setHidden(!visible);
    }
}

// menu contextual
void dockWatchList::onContextMenu(const QPoint& pos)
{
    QListWidgetItem* item = ui->watchList->itemAt(pos);
    if (!item) {    return; }

    QString ticker = item->data(Qt::UserRole).toString();
    QString tag = item->data(Qt::UserRole + 3).toString();
    QString notes = item->data(Qt::UserRole + 4).toString();
    QString isin = item->data(Qt::UserRole + 5).toString();

    // ### Menu
    QMenu menu(this);
    // submenu tag
    QMenu* submenuTag = menu.addMenu("Cambiar etiqueta");
    for (const QString& tag : watchlistTagOrder()) {
        QAction* action = submenuTag->addAction(tag);
        action->setData(tag);
    }
    // QMap<QString, QColor> tags = watchlistTags();
    // for (auto it = tags.constBegin(); it != tags.constEnd(); ++it) {
    //     QAction* action = submenuTag->addAction(it.key());
    //     action->setData(it.key());  // guardamos el nombre del tag en la action
    // }
    //
    QAction* actionIsin    = menu.addAction("Copiar ISIN");
    QAction* actionElim   = menu.addAction("Eliminar");

    // ##########################

    // execute
    QAction* selected = menu.exec(ui->watchList->mapToGlobal(pos));

    // copia ISIN
    if (selected == actionIsin) {   QApplication::clipboard()->setText(isin);   }
    // elimina de watchlist
    else if (selected == actionElim) {
        if (DbManager::getInstance().deleteWLbyTicker(ticker)) {
            delete ui->watchList->takeItem(ui->watchList->row(item));
            qDebug() << "Eliminado: " << ticker;
        }
    }
    // tag
    else if (submenuTag->actions().contains(selected)) {
        QString nuevoTag = selected->data().toString();

        if (DbManager::getInstance().updateTag(ticker, nuevoTag)) {
            item->setData(Qt::UserRole + 3, nuevoTag);
            ui->watchList->update();  // repinta el item con el nuevo tag
        }
    }

    // notes
}
