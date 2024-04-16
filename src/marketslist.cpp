#include "marketslist.h"
#include "qdebug.h"

#include <QFile>


//MarketsList::MarketsList(const QString &fileName, QListWidget *list)
//{
//    int total = list->count();
//    qInfo() << total;
//}


void MarketsList::saveList()
{
    // extraemos todos los items a un lista
    int total = this->m_list->count();

    //creamos el fichero...
    QFile file(this->m_filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // guardamos cada item al archivo...
        for (int i = 0; i < total; ++i) {
            QListWidgetItem* item = this->m_list->item(i);
            out << item->text() << "\n";
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    qInfo() << "Lista de mercados guardada";
}
