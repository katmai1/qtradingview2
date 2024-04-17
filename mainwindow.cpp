#include "mainwindow.h"
#include "src/dialogabout.h"
#include "src/dialogaddmarket.h"
#include "src/dialogoptions.h"
#include "qdir.h"
//#include "ui_mainwindow.h"
#include "QFile"
#include "QDir"
#include "QObject"
#include "QMessageBox"
#include <QtWidgets>
#include <QTextEdit>
#include <QDebug>
#include "src/uimanager.h"
#include "version.h"
#include "src/marketslist.h"



//// Función personalizada para manejar los mensajes de depuración
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    QTextEdit *txt = UIManager::getInstance()->getTextEdit();
    QString formattedMessage;
    switch (type) {
    case QtInfoMsg:
        formattedMessage = QString("<font color=\"blue\">Info: </font> %1").arg(msg);
        break;
    case QtWarningMsg:
        formattedMessage = QString("<font color=\"orange\">Warning: </font> %1").arg(msg);
        break;
    case QtCriticalMsg:
        formattedMessage = QString("<font color=\"red\">Critical: </font> %1").arg(msg);
        break;
    case QtFatalMsg:
        formattedMessage = QString("<font color=\"red\">Fatal: </font> %1").arg(msg);
        break;
    default:
        formattedMessage = msg;
    }
    txt->append(formattedMessage);
}

// --------------------------------------
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UIManager::getInstance()->setTextEdit(ui->txtDebug);
    // init
    this->ui->dockDebug->setVisible(false);
    filepath_markets = this->ui->webview->path_completo + "/markets.txt";
    this->loadListMarkets();

    // declara menus contextuales
    this->ui->listMarkets->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->ui->listMarkets, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(ShowContextMenuMarkets(QPoint)));

    // redirige mensajes debug
    qInstallMessageHandler(customMessageHandler);
    qInfo() << "Iniciando version: " << APP_VERSION;

}


MainWindow::~MainWindow()
{
    delete ui;
}


// menu contextual de la lista de markets
void MainWindow::ShowContextMenuMarkets(const QPoint& pos) // this is a slot
{
    QPoint globalPos = this->ui->listMarkets->mapToGlobal(pos);
    QMenu menu;
    menu.addAction("Abrir...", this, SLOT(on_contextLoadMarket()));
    menu.addAction("Eliminar", this, SLOT(on_contextDeleteMarket()));

    menu.exec(globalPos);
}

// SatusBar
void MainWindow::sendStatus(QString message, int timeout=5000) {
    this->ui->statusbar->showMessage(message, timeout);
}

// añade market a la lista
void MainWindow::addToList(QString market) {
    MarketsList ml(filepath_markets, this->ui->listMarkets);
    if (ml.existMarket(market)) {
        QMessageBox::warning(nullptr, "Error", "Este mercado ya está en la lista...");
    }
    else {
        ml.addMarket(market);
        ml.saveList();
    }
}

// carga una lista, debemos pasarle un listwidget y una ruta al fichero
void MainWindow::loadListMarkets()
{
    MarketsList ml(filepath_markets, this->ui->listMarkets);
    ml.loadList();
}

// ejecuta javascript de test
void MainWindow::on_actionjavascript_triggered()
{
    this->ui->webview->testJavascript();
}

// envia el item seleccionado a la funcion que carga la grafica
void MainWindow::on_contextLoadMarket()
{
    this->on_listMarkets_itemDoubleClicked(this->ui->listMarkets->currentItem());
}

// elimina market seleccionado
void MainWindow::on_contextDeleteMarket()
{
    MarketsList ml(filepath_markets, this->ui->listMarkets);
    QList<QListWidgetItem *> lista = this->ui->listMarkets->selectedItems();
    for (QListWidgetItem *item : lista ) {  delete item;    }
    ml.saveList();
}

// boton de test
void MainWindow::on_actionTest_triggered()
{
    qDebug() << "Pruebaaa";
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
    QString pair = item->text();
    QString exchange = item->toolTip();
    this->ui->webview->loadChart(pair, exchange);
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

// filtra los markets
void MainWindow::on_edFilter_textChanged(const QString &arg1)
{
    int total = this->ui->listMarkets->count();
    for (int i = 0; i < total; ++i) {
        QListWidgetItem* item = this->ui->listMarkets->item(i);
        const bool matches = item->text().contains(arg1.toUpper(), Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}


// abre dialogo de opciones
void MainWindow::on_actionOptions_triggered()
{
    dialogoptions *Options;
    Options = new dialogoptions(this);
    Options->show();
}

