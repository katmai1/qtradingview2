#ifndef MARKETSLIST_H
#define MARKETSLIST_H

#include <QMainWindow>
#include <QString>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QMenu>


// clase para gestionar el qlistwidget de markets
class MarketsList
{
public:
    MarketsList(const QString &fileName, QListWidget *listWidget) : m_filename(fileName), m_list(listWidget) {}
    void saveList();
    void loadList();
    bool existMarket(QString market);
    void addMarket(QString market);

private:
    QString m_filename;
    QListWidget *m_list;
};


// ******************************************
// menu contextual
class MenuContextual : public QObject
{
    Q_OBJECT
public:
    explicit MenuContextual(QListWidget *parent = nullptr);
    ~MenuContextual();

signals:
    void sigLoadMarket(QListWidgetItem *item);
    void sigSaveMarketsList();

private slots:
    void showMenu(const QPoint &pos);
    void loadChart();
    void deleteMarket();
    void getPrice();

private:
    QListWidget *listWidget;
    QMenu *menu;
};



#endif // MARKETSLIST_H
