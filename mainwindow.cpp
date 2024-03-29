#include "mainwindow.h"
#include "dialogabout.h"
#include "dialogaddmarket.h"
#include "dialogoptions.h"
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
    filepath_markets = this->ui->webview->path_completo + "/markets.txt";

    // carga listas
    this->loadListMarkets(this->ui->listMarkets, filepath_markets);

    // declara menus contextuales
    this->ui->listMarkets->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->ui->listMarkets, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenuMarkets(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// menu contextual de la lista de markets
void MainWindow::ShowContextMenuMarkets(const QPoint& pos) // this is a slot
{
    QPoint globalPos = this->ui->listMarkets->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Abrir...", this, SLOT(on_contextLoadMarket()));
    myMenu.addAction("Eliminar", this, SLOT(on_bt_delete_clicked()));

    myMenu.exec(globalPos);
}

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
    QList<QListWidgetItem *> lista = this->ui->listMarkets->findItems(market, Qt::MatchExactly);
    if (lista.count() > 0) {
        QMessageBox::warning(nullptr, "Error", "Este mercado ya está en la lista...");
    }
    else {
        this->ui->listMarkets->addItem(market);
        this->saveListMarkets();
    }
}

// guarda lista de markets en fichero
void MainWindow::saveListMarkets()
{
    // extraemos todos los items a un lista
    int total = this->ui->listMarkets->count();
    //creamos el fichero...
    QFile file(filepath_markets);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        // guardamos cada item al archivo...
        for (int i = 0; i < total; ++i) {
            QListWidgetItem* item = this->ui->listMarkets->item(i);
            out << item->text() << "\n";
        }
        file.close();
    } else {
        // Manejar el error si no se pudo abrir el archivo.
    }
    this->sendDebug("Lista de mercados guardada");
}


// carga una lista, debemos pasarle un listwidget y una ruta al fichero
void MainWindow::loadListMarkets(QListWidget *list, QString path)
{
    list->clear();
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            list->addItem(line);
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

void MainWindow::on_contextLoadMarket()
{
    this->on_listMarkets_itemDoubleClicked(this->ui->listMarkets->currentItem());
}

// elimina seleccionados de favoritos
void MainWindow::on_bt_delete_clicked()
{
    QList<QListWidgetItem *> lista = this->ui->listMarkets->selectedItems();
    for (QListWidgetItem *item : lista ) {  delete item;    }
    this->saveListMarkets();

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

// abre market seleccionado con doble click
void MainWindow::on_listMarkets_itemDoubleClicked(QListWidgetItem *item)
{
    QString symbol = item->text();
    this->ui->webview->loadChart(symbol);
}


// abre el dialogo addMarket
void MainWindow::on_btAdd_clicked()
{
    dialogAddMarket *addMarket;
    addMarket = new dialogAddMarket(this);
    addMarket->setModal(true);
    addMarket->show();
}


// pantalla completa
void MainWindow::on_actionFullscreen_triggered(bool checked)
{
    if (checked) { this->showFullScreen(); }
    else { this->showMaximized(); }
}


void MainWindow::on_edFilter_textChanged(const QString &arg1)
{
    int total = this->ui->listMarkets->count();
    for (int i = 0; i < total; ++i) {
        QListWidgetItem* item = this->ui->listMarkets->item(i);
        const bool matches = item->text().contains(arg1.toUpper(), Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}



void MainWindow::on_actionOptions_triggered()
{
    dialogoptions *Options;
    Options = new dialogoptions(this);
    Options->show();
}

