#ifndef MARKETSLIST_H
#define MARKETSLIST_H

#include <QString>
#include <QListWidget>
#include <QListWidgetItem>


class MarketsList
{
public:
    MarketsList(const QString &filenName, QListWidget *listWidget) : m_filename(filenName), m_list(listWidget) {}
    void saveList();
//    void loadList_old();
    void loadList();
    bool existMarket(QString market);
    void addMarket(QString market);

private:
    QString m_filename;
    QListWidget *m_list;
};

#endif // MARKETSLIST_H
