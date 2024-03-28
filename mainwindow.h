#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qlistwidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    QString filepath_favorites;
    QString filepath_binance;
    QString filepath_markets;


    void sendStatus(QString message, int timeout);
    void sendDebug(QString message);
    void addToList(QString market);

private slots:
    void saveListMarkets();

    void on_actionPantalla_completa_triggered(bool checked);

    void on_bt_delete_clicked();

    void cargarFicheroLista(QListWidget *lista, QString path);

    void on_actionjavascript_triggered();

    void ShowContextMenuMarkets(const QPoint& pos);

    void on_ed_filtro_textChanged(const QString &arg1);

    void on_actionTest_triggered();

    void on_actionAbout_triggered();

    void on_list_markets_itemDoubleClicked(QListWidgetItem *item);

    void on_btAdd_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
