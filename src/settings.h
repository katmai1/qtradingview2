#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QLocale>
#include <QFileInfo>

#include "src/marketslist.h"
#include "src/portfolio.h"


struct Languages {
    QString name;
    QString locale;
};


class SettingsManager {
public:
    SettingsManager() {
        // Configura QSettings con la organización y el nombre de la aplicación
        m_settings = new QSettings("QTradingview2", "QTradingview2");
    }

    ~SettingsManager() {
        delete m_settings;
    }

    // Método para obtener un valor de configuración
    QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant(), const QString &group = "Options") const {
        m_settings->beginGroup(group);
        QVariant res = m_settings->value(key, defaultValue);
        m_settings->endGroup();
        return res;
    }

    // Método para establecer un valor de configuración
    void setValue(const QString &key, const QVariant &value, const QString &group = "Options") {
        m_settings->beginGroup(group);
        m_settings->setValue(key, value);
        m_settings->endGroup();
    }

    // devuelve el fichero para el translator de qt
    QString getTranslatorFile() {
        QString language = this->language();
        QString baseName;

        // si es el idioma del sistema
        if (language == "system") {
            QString locale = QLocale::system().name();
            // comprueba que sea de los disponibles por la app
            if (this->existLanguage(locale)) {  baseName = "QTradingview2_" + locale;   }
            // si no lo és, devuelve el inglés
            else {  baseName = "QTradingview2_en_EN";   }
            return baseName;
        }
        // si hay uno seleccionado, devuelve ese
        baseName = "QTradingview2_" + language;
        return baseName;
    }

    // devuelve el lenguage registrado
    QString language() {
        return this->getValue("language", "system").toString();
    }

    // devuelve la carpeta de la config de la aplicación
    QString pathDir() {
        QString filePath = m_settings->fileName();
        QFileInfo fileInfo(filePath);
        return fileInfo.absolutePath();
    }

    // devuelve lista de markets, si no existe devuelve BTC/USDT de Binance
    QStringList getListMarkets() {
        return this->getValue("markets", "BTC/USDT:BINANCE").toStringList();
    }

    // devuelve lista de idiomas disponibles
    QList<Languages> availableLanguages() {
        QList<Languages> lista;
        lista << Languages{"Inglés", "en_EN"}
              << Languages{"Español", "es_ES"}
              << Languages{"Catalan", "ca_ES"}
              << Languages{"Italiano", "it_IT"}
              << Languages{"Francés", "fr_FR"};
        return lista;
    }

    // le pasamos el locale de un idioma y devuelve true si es uno de los disponibles
    bool existLanguage(QString locale) {
        QList<Languages> languages = this->availableLanguages();
        foreach (const Languages &lang, languages) {
            if (lang.locale == locale) {    return true;   }
        }
        return false;
    }

    // devuelve true si ese market ya existe
    bool existMarket(QString market)
    {
        QStringList lista = this->getListMarkets();
        foreach (QString item, lista) {
            if (item == market) {   return true;  }
        }
        return false;
    }

    QList<Positions> getPositions() {
        // Leer el JSON desde QSettings y deserializarlo a una lista de entradas
        QString jsonStringLoaded = this->getValue("positions", "{}", "Portfolio").toString();
        QJsonDocument docLoaded = QJsonDocument::fromJson(jsonStringLoaded.toUtf8());
        QJsonArray jsonArrayLoaded = docLoaded.array();
        QList<Positions> PositionsList;
        for (const auto& item : jsonArrayLoaded) {
            if (item.isObject()) {
                PositionsList.append(Positions::fromJson(item.toObject()));
            }
        }
        return PositionsList;
    }

private:
    QSettings *m_settings;
};


#endif // SETTINGS_H
