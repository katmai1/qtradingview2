#ifndef DOCKWATCHLIST_H
#define DOCKWATCHLIST_H

#include <QDockWidget>
#include <QSqlQuery>


namespace Ui {
class dockWatchList;
}

class dockWatchList : public QDockWidget
{
    Q_OBJECT

public:
    explicit dockWatchList(QWidget *parent = nullptr);
    ~dockWatchList();

private:
    Ui::dockWatchList *ui;

    void updateList();
};

#endif // DOCKWATCHLIST_H
