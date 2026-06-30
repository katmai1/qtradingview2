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
    QString isin;
    double price;
};

inline QMap<QString, QColor> watchlistTags()
{
    return {
        { "En cartera",            QColor(0x26, 0xa6, 0x9a) }, // verde
        { "Esperando corrección",  QColor(0xff, 0xa7, 0x26) }, // naranja
        { "Pendiente de análisis", QColor(0x42, 0xa5, 0xf5) }, // azul
        { "Descartado",            QColor(0xef, 0x53, 0x50) }, // rojo
    };
}

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
