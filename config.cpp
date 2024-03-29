#include "config.h"

QString home_path = getenv("HOME");
QString file_config = home_path + "/.qtradingview2/config.ini";

config::config()
    : QSettings(file_config, QSettings::NativeFormat)
{

}

QString config::getData(const QString &clave, const QString &valorPorDefecto) const {
    return value(clave, valorPorDefecto).toString();
}

void config::setData(const QString &clave, const QString &valor) {
    setValue(clave, valor);
}
