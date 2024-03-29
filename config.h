#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class config : public QSettings
{
public:
    config();

    QString getData(const QString &clave, const QString &valorPorDefecto = QString()) const;
    void setData(const QString &clave, const QString &valor);
};

#endif // CONFIG_H
