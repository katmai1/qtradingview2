#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include "src/systray.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // prepara carpeta config
    QString home_path = getenv("HOME");
    QString file_config = home_path + "/.qtradingview2/config.ini";
    QSettings settings(file_config, QSettings::NativeFormat);

    QString language = settings.value("language", "system").toString();

    QTranslator translator;

    if (language == "system") {
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale : uiLanguages) {
            const QString baseName = "QTradingview2_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
            }
        }
    }
    else {
        const QString baseName = "QTradingview2_" + language;
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
        }
    }


    MainWindow w;

    SystemTrayIcon systemTrayIcon(&w);
    systemTrayIcon.show();

    w.showMaximized();
    return a.exec();
}
