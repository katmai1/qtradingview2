#include "dockwatchlist.h"
#include "ui_dockwatchlist.h"

dockWatchList::dockWatchList(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::dockWatchList)
{
    ui->setupUi(this);
    updateList();
}

dockWatchList::~dockWatchList()
{
    delete ui;
}

void dockWatchList::updateList() {
    auto* q = new QSqlQuery();
    q->exec("SELECT * FROM watch");
    while (q->next()) {
            qInfo() << q->value(1).toString();
    }

}