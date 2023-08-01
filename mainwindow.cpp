#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"


// aun no funciona
void saveListWidgetItems(QListWidget *lista_raw, QString path) {
    QString filepath = path + "/lista.txt";
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        printf("entraa");
        QTextStream out(&file);
        for (int i = 0; i < lista_raw->count(); ++i) {
             QListWidgetItem* item = lista_raw->item(i);
             out << item->text() << "\n";
       }
    file.close();
    }
    else { printf("error"); }
}

// --------------------------------------
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);   
}

MainWindow::~MainWindow()
{
    delete ui;
}


// ------------------------------------
// Pantalla completa

void MainWindow::on_actionPantalla_completa_triggered(bool checked)
{
    if (checked) { this->showFullScreen(); }
    else { this->showMaximized(); }
}

void MainWindow::sendStatus(QString message, int timeout=10000) {
    this->ui->statusbar->showMessage(message, timeout);
}


// -----------------------------------------
// boton añadir

void MainWindow::on_bt_add_clicked()
{
    QString symbol = this->ui->ed_symbol->text().toUpper().trimmed();
    QList<QListWidgetItem *> lista = this->ui->list_favorites->findItems(symbol, Qt::MatchExactly);
    if (lista.count() > 0) {    this->sendStatus("Este par ya está en la lista...");    }
    else {  this->ui->list_favorites->addItem(symbol);  }
}


// ---------------------------------------
// elimina seleccionados
void MainWindow::on_bt_delete_clicked()
{
    QList<QListWidgetItem *> lista = this->ui->list_favorites->selectedItems();
    for (QListWidgetItem *item : lista ) {  delete item;    }
}


// ------------------------------------------
// carga grafica alhacer dobleclick
void MainWindow::on_list_favorites_itemDoubleClicked(QListWidgetItem *item)
{
    QString symbol = item->text();
    this->ui->webview->loadChart(symbol);
}

