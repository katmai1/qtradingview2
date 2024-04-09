#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
#include <QDebug>
#include "uimanager.h"
//#include <iostream>

////// Función personalizada para manejar los mensajes de depuración
//void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    QTextEdit *txt = UIManager::getInstance()->getTextEdit();
//    QString formattedMessage;
//    switch (type) {
//    case QtInfoMsg:
//        formattedMessage = QString("Info: %1").arg(msg);
//        break;
//    case QtWarningMsg:
//        formattedMessage = QString("<font color=\"orange\">Warning: </font> %1").arg(msg);
//        break;
//    case QtCriticalMsg:
//        formattedMessage = QString("Critical: %1").arg(msg);
//        break;
//    case QtFatalMsg:
//        formattedMessage = QString("Fatal: %1").arg(msg);
//        break;
//    default:
//        formattedMessage = msg;
//    }
//    txt->append(formattedMessage);
//    // Puedes implementar tu propia lógica aquí
//}


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

//    qInstallMessageHandler(customMessageHandler);

//    qDebug() << "Este es un mensaje de depuración.";
//    qInfo() << "Este es un mensaje de información.";
//    qWarning() << "Este es un mensaje de advertencia.";
//    qCritical() << "Este es un mensaje crítico.";

    w.showMaximized();
    return a.exec();
}
