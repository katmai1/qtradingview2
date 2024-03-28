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
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    QString filepath_favorites;
    QString filepath_binance;


    void sendStatus(QString message, int timeout);
    void sendDebug(QString message);

private slots:
    void on_actionPantalla_completa_triggered(bool checked);

    void on_bt_add_clicked();

    void on_bt_delete_clicked();

    void on_list_favorites_itemDoubleClicked();

    void cargarFicheroLista(QListWidget *lista, QString path);

    void on_actionGuardar_lista_triggered();

    void on_actionjavascript_triggered();

    void ShowContextMenuFavorites(const QPoint& pos);

    void on_bt_add_tofav_clicked();

    void on_ed_filtro_textChanged(const QString &arg1);

    void on_actionTest_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
