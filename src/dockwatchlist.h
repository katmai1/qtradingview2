#ifndef DOCKWATCHLIST_H
#define DOCKWATCHLIST_H

#include <QDockWidget>

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
};

#endif // DOCKWATCHLIST_H
