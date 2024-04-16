#include "marketslist.h"
#include "qdebug.h"

#include <QFile>
#include <QListWidgetItem>
#include <QListWidget>
#include <QList>


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


void MarketsList::saveList()
{
    //creamos el fichero...
    QFile file(this->m_filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // guardamos cada item al archivo...
        for (int i = 0; i < this->m_list->count(); ++i) {
            QListWidgetItem* item = this->m_list->item(i);
            out << item->text().toUpper()+":"+item->toolTip().toUpper() << "\n";
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    qInfo() << "Lista de mercados guardada";
}

bool MarketsList::existMarket(QString market)
{
    QList<QListWidgetItem *> lista = this->m_list->findItems(market, Qt::MatchExactly);
    if (lista.count() > 0) {    return true;    }
    else {  return false;   }
}

void MarketsList::addMarket(QString market)
{
    QListWidgetItem *item = getItem(market);
    this->m_list->addItem(item);
}

void MarketsList::loadList() {
    this->m_list->clear();
    QFile file(this->m_filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString market = in.readLine();
            this->addMarket(market);
        }
        file.close();
    } else {
        qWarning() << "Error al cargar el fichero markets";
    }
}
