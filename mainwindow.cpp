#include "mainwindow.h"
#include "dialogabout.h"
#include "dialogaddmarket.h"
#include "qdir.h"
#include "ui_mainwindow.h"
#include "QFile"
#include <QDebug>
#include "QDir"
#include "QObject"
#include "QMessageBox"


// --------------------------------------
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // init
    this->ui->dockDebug->setVisible(false);
    filepath_favorites = this->ui->webview->path_completo + "/favoritos.txt";
    filepath_binance = this->ui->webview->path_completo + "/binance.txt";
    filepath_markets = this->ui->webview->path_completo + "/markets.txt";

    // carga listas
    this->cargarFicheroLista(this->ui->list_markets, filepath_markets);

    // declara menus contextuales
    this->ui->list_markets->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->ui->list_markets, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenuMarkets(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// menu contextual de la lista de markets
void MainWindow::ShowContextMenuMarkets(const QPoint& pos) // this is a slot
{
    QPoint globalPos = this->ui->list_markets->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Abrir...", this, SLOT(on_list_favorites_itemDoubleClicked()));
    myMenu.addAction("Eliminar", this, SLOT(on_bt_delete_clicked()));

    myMenu.exec(globalPos);
}

// -Custom Functions-------------------
// SatusBar
void MainWindow::sendStatus(QString message, int timeout=10000) {
    this->ui->statusbar->showMessage(message, timeout);
}

// SendDebug
void MainWindow::sendDebug(QString message) {
    this->ui->txtDebug->insertPlainText(message + "\n");
}

// añade market a la lista
void MainWindow::addToList(QString market) {
    QList<QListWidgetItem *> lista = this->ui->list_markets->findItems(market, Qt::MatchExactly);
    if (lista.count() > 0) {
        QMessageBox::warning(nullptr, "Error", "Este market ya está en la lista...");
    }
    else {
        this->ui->list_markets->addItem(market);
        this->saveListMarkets();
    }
}

// guarda lista de markets en fichero
void MainWindow::saveListMarkets()
{
    // extraemos todos los items a un lista
    int total = this->ui->list_markets->count();
    //creamos el fichero...
    QFile file(filepath_markets);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // guardamos cada item al archivo...
        for (int i = 0; i < total; ++i) {
            QListWidgetItem* item = this->ui->list_markets->item(i);
            out << item->text() << "\n";
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    this->sendStatus("Lista de markets guardada");
}

// Pantalla completa
void MainWindow::on_actionPantalla_completa_triggered(bool checked)
{
    if (checked) { this->showFullScreen(); }
    else { this->showMaximized(); }
}

// carga una lista, debemos pasarle un listwidget y una ruta al fichero
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
    this->sendStatus("Lista cargada");
}

// ejecuta javascript
void MainWindow::on_actionjavascript_triggered()
{
    this->ui->webview->testJavascript();
}


// elimina seleccionados de favoritos
void MainWindow::on_bt_delete_clicked()
{
    QList<QListWidgetItem *> lista = this->ui->list_markets->selectedItems();
    for (QListWidgetItem *item : lista ) {  delete item;    }
    this->saveListMarkets();

}

// filtra lista
void MainWindow::on_ed_filtro_textChanged(const QString &texto)
{
    int total = this->ui->list_markets->count();
    for (int i = 0; i < total; ++i) {
        QListWidgetItem* item = this->ui->list_markets->item(i);
        const bool matches = item->text().contains(texto.toUpper(), Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

// boton de test
void MainWindow::on_actionTest_triggered()
{
    this->sendDebug("Probando...");
}

// abre dialog about
void MainWindow::on_actionAbout_triggered()
{
    dialogAbout *about;
    about = new dialogAbout(this);
    about->setModal(true);
    about->show();
}

// abre market con doble click
void MainWindow::on_list_markets_itemDoubleClicked(QListWidgetItem *item)
{
    QString symbol = item->text();
    this->ui->webview->loadChart(symbol);
}


void MainWindow::on_btAdd_clicked()
{
    dialogAddMarket *addMarket;
    addMarket = new dialogAddMarket(this);
    addMarket->setModal(true);
    addMarket->show();
}

