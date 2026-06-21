#include "dockwatchlist.h"
#include "ui_dockwatchlist.h"

dockWatchList::dockWatchList(QWidget *parent)
    : QDockWidget(parent)
    , ui(new Ui::dockWatchList)
{
    ui->setupUi(this);
}

dockWatchList::~dockWatchList()
{
    delete ui;
}
