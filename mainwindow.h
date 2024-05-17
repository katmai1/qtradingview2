#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTextEdit>

#include "src/marketslist.h"
#include "src/systray.h"
#include "src/settings.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
    void setSystemTrayWindow(SystemTrayIcon *SystemTrayIcon) {
        this->trayIcon = SystemTrayIcon;
    }

    QString filepath_markets;

    void sendStatus(QString message, int timeout);
    void addToList(QString market);


private slots:
    void loadListMarkets();

    void on_actionjavascript_triggered();

    // doble click item in marketslist
    void on_listMarkets_itemDoubleClicked(QListWidgetItem *item);

    // otros
    void on_actionTest_triggered();

    void on_actionAbout_triggered();

    void on_btAdd_clicked();

    void on_actionFullscreen_triggered(bool checked);

    void on_edFilter_textChanged(const QString &arg1);

    void on_actionOptions_triggered();

    void on_actionDebug_triggered(bool checked);

    void on_actionMarkets_triggered(bool checked);

public slots:
    void loadMarket(QListWidgetItem *item);
    void saveMarketsList();

private:
    Ui::MainWindow *ui;
    QTextEdit *tdebug;
    SystemTrayIcon *trayIcon;
    MenuContextual *menuCtx;
    SettingsManager *settings;
};

#endif // MAINWINDOW_H
