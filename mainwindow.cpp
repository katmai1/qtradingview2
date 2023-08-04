#include "mainwindow.h"
#include "qdir.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <QDebug>
#include "QDir"
#include "QObject"



// --------------------------------------
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // init
    this->ui->dock_markets->setVisible(false);
    filepath_favorites = this->ui->webview->path_completo + "/favoritos.txt";
    filepath_binance = this->ui->webview->path_completo + "/binance.txt";

    // carga listas
    this->cargarFicheroLista(this->ui->list_favorites, filepath_favorites);
    this->cargarFicheroLista(this->ui->list_markets, filepath_binance);

    // declara menus contextuales
    this->ui->list_favorites->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->ui->list_favorites, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenuFavorites(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::ShowContextMenuFavorites(const QPoint& pos) // this is a slot
{
    QPoint globalPos = this->ui->list_favorites->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Abrir...", this, SLOT(on_list_favorites_itemDoubleClicked()));
    myMenu.addAction("Eliminar", this, SLOT(on_bt_delete_clicked()));

    myMenu.exec(globalPos);
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
    this->on_actionGuardar_lista_triggered();
}


// ---------------------------------------
// elimina seleccionados
void MainWindow::on_bt_delete_clicked()
{
    QList<QListWidgetItem *> lista = this->ui->list_favorites->selectedItems();
    for (QListWidgetItem *item : lista ) {  delete item;    }
    this->on_actionGuardar_lista_triggered();

}


// ------------------------------------------
// carga grafica alhacer dobleclick
void MainWindow::on_list_favorites_itemDoubleClicked()
{
    QListWidgetItem *item = this->ui->list_favorites->currentItem();
    QString symbol = item->text();
    this->ui->webview->loadChart(symbol);
}


void MainWindow::on_actionGuardar_lista_triggered()
{
    // extraemos todos los items a un lista
    int total = this->ui->list_favorites->count();
    //creamos el fichero...
    QFile file(filepath_favorites);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // guardamos cada item al archivo...
        for (int i = 0; i < total; ++i) {
            QListWidgetItem* item = this->ui->list_favorites->item(i);
            out << item->text() << "\n";
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    this->sendStatus("Lista de favoritos guardada");
}


void MainWindow::cargarFicheroLista(QListWidget *lista, QString path)
{
    lista->clear();
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            lista->addItem(line);
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    this->sendStatus("Lista de favoritos cargada");
}




void MainWindow::on_actionjavascript_triggered()
{
    this->ui->webview->testJavascript();
}


void MainWindow::on_bt_add_tofav_clicked()
{
    QList<QListWidgetItem *> lista = this->ui->list_markets->selectedItems();
    for (QListWidgetItem *item : lista ) {
        QString symbol = item->text().toUpper().trimmed();
        QList<QListWidgetItem *> existe = this->ui->list_favorites->findItems(symbol, Qt::MatchExactly);
        if (existe.count() > 0) {    this->sendStatus("Este par ya está en la lista...");    }
        else {  this->ui->list_favorites->addItem(symbol);  }
    }
    this->on_actionGuardar_lista_triggered();
}


void MainWindow::on_ed_filtro_textChanged(const QString &texto)
{
    int total = this->ui->list_markets->count();
    for (int i = 0; i < total; ++i) {
        QListWidgetItem* item = this->ui->list_markets->item(i);
        const bool matches = item->text().contains(texto.toUpper(), Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

