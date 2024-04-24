#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

#include "src/systray.h"
#include <src/settings.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // declaramos settings y asignamos el valor created a true para que cree la carpeta de configuracion si no existía
    SettingsManager settings;
    settings.setValue("created", "true");

    // configura idioma
    QTranslator translator;
    if (translator.load(":/i18n/" + settings.getTranslatorFile())) {
        qDebug() << "idioma cargado";
        a.installTranslator(&translator);
    }
    else {
        qDebug() << "idioma no cargado?";
    }

    MainWindow w;

    // configura systray
    SystemTrayIcon systemTrayIcon(&w);
    systemTrayIcon.show();

    w.showMaximized();
    return a.exec();
}
