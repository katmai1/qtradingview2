#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>


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

    // Crear una instancia de QSystemTrayIcon
    QSystemTrayIcon trayIcon;

    // Cargar un ícono para el área de notificación
    QIcon icon(":/icons/logo"); // Asegúrate de que la ruta del archivo sea correcta
    trayIcon.setIcon(icon);

    // Crear un menú contextual para el ícono del área de notificación
    QMenu *menu = new QMenu();
    menu->addAction("Salir", &a, &QCoreApplication::quit); // Conecta la acción de salir con el cierre de la aplicación

    // Establecer el menú en el ícono del área de notificación
    trayIcon.setContextMenu(menu);

    // Mostrar el ícono en el área de notificación
    trayIcon.show();

    MainWindow w;

    w.showMaximized();
    return a.exec();
}
