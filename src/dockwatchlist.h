#ifndef DOCKWATCHLIST_H
#define DOCKWATCHLIST_H

#include "qlistwidget.h"
#include <QDockWidget>
#include <QSqlQuery>

struct WatchItem {
    QString ticker;
    QString type;
    QString tag;
    QString notes;
    QString name;
    QString description;
    double price;
};


namespace Ui {
class dockWatchList;
}

class dockWatchList : public QDockWidget
{
    Q_OBJECT

public:
    explicit dockWatchList(QWidget *parent = nullptr);
    ~dockWatchList();
    void updateList();

private slots:
    void onItemDoubleClicked(QListWidgetItem* item);
    void onContextMenu(const QPoint& pos);

signals:
    void loadSymbol(const QString& ticker);

private:
    Ui::dockWatchList *ui;
    void refreshList(QList<WatchItem>& lista);

};

#endif // DOCKWATCHLIST_H
