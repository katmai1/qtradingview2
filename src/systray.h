#ifndef SYSTRAY_H
#define SYSTRAY_H
#include <QApplication>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>

class SystemTrayIcon : public QObject {
    Q_OBJECT

public:
    SystemTrayIcon(QWidget *mainWindow, QObject *parent = nullptr) : QObject(parent), mainWindow(mainWindow) {
        trayIcon = new QSystemTrayIcon(this);

        QIcon icon(":/icons/logo"); // Asegúrate de que la ruta del archivo sea correcta
        trayIcon->setIcon(icon);

        QMenu *menu = new QMenu();
        menu->addAction("Mostrar/ocultar", this, &SystemTrayIcon::toggleVisibility);
        menu->addSeparator();
        menu->addAction("Salir", qApp, &QCoreApplication::quit);

        trayIcon->setContextMenu(menu);

        connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTrayIcon::toggleVisibility);
    }

    void show() {
        trayIcon->show();
    }

    void hide() {
        trayIcon->hide();
    }

private slots:
    void toggleVisibility() {
        if (mainWindow->isVisible()) {
            mainWindow->hide();
        } else {
            mainWindow->show();
        }
    }

private:
    QSystemTrayIcon *trayIcon;
    QWidget *mainWindow;

};

#endif // SYSTRAY_H
