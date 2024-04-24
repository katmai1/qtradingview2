#include "marketslist.h"
#include "qdebug.h"

#include <QFile>
#include <QListWidgetItem>
#include <QListWidget>
#include <QList>

#include "qglobal.h"
#include "src/settings.h"


// le pasamos como parametro el market en bruto
// nos devuelve un item con el icono y el tooltip del exchange y como texto el market
QListWidgetItem *getItem(const QString &market) {
    QStringList data = market.split(":");
    QString pair = data[0].toLower();
    QString exchange = data[1].toLower();
    QString tooltip = exchange;
    tooltip[0] = tooltip[0].toUpper();
    QIcon icon(":exchanges/icons/"+exchange);

    QListWidgetItem *item = new QListWidgetItem(pair.toUpper());
    item->setToolTip(tooltip);
    item->setIcon(icon);
    return item;
}


// guarda lista en el fichero settings
void MarketsList::saveList()
{

    SettingsManager settings;
    QStringList lista;
    for (int i = 0; i < this->m_list->count(); ++i) {
        QListWidgetItem* item = this->m_list->item(i);
        lista << item->text().toUpper()+":"+item->toolTip().toUpper();
    }
    settings.setValue("markets", lista);

    qInfo() << "Lista de mercados guardada";
}

// devuelve true si ese market ya existe
bool MarketsList::existMarket(QString market)
{
    // obtiene item del market y hace la busqueda
    QListWidgetItem *item = getItem(market);
    QList<QListWidgetItem *> lista = this->m_list->findItems(item->text(), Qt::MatchExactly);

    // si la busqueda tiene algun resultado y ademas el exchange coincide devuelve true
    if (lista.count() > 0) {
        for (QListWidgetItem *market : lista) {
            qInfo() << market->text();
            if (market->toolTip() == item->toolTip()) { return true;    }
        }
    }
    return false;
}

// añade market a la lista
void MarketsList::addMarket(QString market)
{
    QListWidgetItem *item = getItem(market);
    this->m_list->addItem(item);
}

//// carga lista desde fichero txt
//void MarketsList::loadList_old() {
//    this->m_list->clear();
//    QFile file(this->m_filename);

//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        while (!in.atEnd()) {
//            QString market = in.readLine();
//            this->addMarket(market);
//        }
//        file.close();
//    } else {
//        qWarning() << "Error al cargar el fichero markets";
//    }
//}


// carga lista desde fichero settings
void MarketsList::loadList() {
    SettingsManager settings;
    this->m_list->clear();
    QStringList lista = settings.getListMarkets();
    foreach (const QString &market, lista) {
        this->addMarket(market);
    }
}
