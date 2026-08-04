#include "dockwatchlist.h"
#include "ui_dockwatchlist.h"
#include "dbmanager.h"
#include "watchlistdelegate.h"
#include "settings.h"

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
    ui->comboTag->setCurrentIndex(SettingsManager::getInstance().getValue("lastTag", 1, "watchlist").toInt());

    // configuramos lista, actualizamos y conectamos
    ui->watchList->setItemDelegate(new WatchListDelegate(this));
    ui->watchList->setAlternatingRowColors(true);
    updateList();
    onFilterTag(ui->comboTag->currentText());

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
    // una sola query (JOIN watch + stocks) en vez de 1 query por ticker
    QList<WatchItem> itemList = DbManager::getInstance().loadWatchlist();
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
    QAction* actionIsin    = menu.addAction("Copiar ISIN");
    QAction* actionElim   = menu.addAction("Eliminar");

    // ##########################

    // execute
    QAction* selected = menu.exec(ui->watchList->mapToGlobal(pos));

    // copia ISIN
    if (selected == actionIsin) {
        QApplication::clipboard()->setText(isin);
        qInfo() << ticker + ": Copiado ISIN (" + isin + ")";
    }
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
