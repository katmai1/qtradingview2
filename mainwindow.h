#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTextEdit>
#include "src/marketslist.h"
#include "src/systray.h"


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

    // context menu
    void ShowContextMenuMarkets(const QPoint& pos);
    void on_contextLoadMarket();
    void on_contextDeleteMarket();

    void on_actionTest_triggered();

    void on_actionAbout_triggered();

    void on_listMarkets_itemDoubleClicked(QListWidgetItem *item);

    void on_btAdd_clicked();

    void on_actionFullscreen_triggered(bool checked);

    void on_edFilter_textChanged(const QString &arg1);

    void on_actionOptions_triggered();

private:
    Ui::MainWindow *ui;
    QTextEdit *tdebug;
    SystemTrayIcon *trayIcon;
};
#endif // MAINWINDOW_H
