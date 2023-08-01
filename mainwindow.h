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

    void sendStatus(QString message, int timeout);

private slots:
    void on_actionPantalla_completa_triggered(bool checked);

    void on_bt_add_clicked();

    void on_bt_delete_clicked();

    void on_list_favorites_itemDoubleClicked(QListWidgetItem *item);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
