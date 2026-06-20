#include "mainwindow.h"
#include "src/dialogabout.h"
#include "src/dialogoptions.h"

#include "QObject"
#include "QMessageBox"
#include <QtWidgets>
#include <QTextEdit>
#include <QDebug>
#include <QString>
#include <QCloseEvent>

#include "src/uimanager.h"
#include "src/tvscreener.h"
#include "dbmanager.h"

#include <unistd.h>


// ************************************************************************************************
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(new SettingsManager())
{
    // load ui
    ui->setupUi(this);

    // redirect debug messages to dockdebug
    UIManager::getInstance()->setTextEdit(ui->txtDebug);
    qInstallMessageHandler(customMessageHandler);

    qInfo() << "Iniciando version: " << QString("v%1").arg(VERSION_FULL);

    // init db
    DbManager::getInstance().init("qtradingview2.db");

    // load dock stocks
    dockStock = new StockDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, dockStock);
    connectDock(dockStock, ui->actionStocks, "stock");

    // dockStock->setVisible(settings->getValue("stock", false, "View").toBool());
    // ui->actionStocks->setChecked(settings->getValue("stock", false, "View").toBool());

    // connect(ui->actionStocks, &QAction::toggled, dockStock, &QDockWidget::setVisible);
    // connect(dockStock, &QDockWidget::visibilityChanged, ui->actionStocks, &QAction::setChecked);

    // load views
    this->ui->dockDebug->setVisible(settings->getValue("debug", false, "View").toBool());
    this->ui->statusbar->setVisible(settings->getValue("statusbar", false, "View").toBool());
    this->ui->actionStatusbar->setChecked(settings->getValue("statusbar", false, "View").toBool());

    // screener...
    auto* profile = qobject_cast<CustomWebEnginePage*>(ui->webview->page())->profile();
    screener = new TvScreener(profile, this);

    connect(screener, &TvScreener::dataReady, [](const QString& market, const QList<Stock>& stocks, int total) {
        qDebug() << "Loaded " << total;
        // check if is stock or crypto
        if (market.startsWith("crypto_")) {
            QString exchange = market.mid(7); // quita "crypto_"
            DbManager::getInstance().saveCrypto(exchange, stocks);
        } else {
            DbManager::getInstance().saveStocks(market, stocks);
        }
    });

    connect(screener, &TvScreener::errorOccurred, [](const QString& err) { qDebug() << "Error:" << err; });
}


MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
}

// SatusBar
void MainWindow::sendStatus(QString message, int timeout=5000) {
    this->ui->statusbar->showMessage(message, timeout);
}


// ************************************************************************************************
// Abre dialogs

// abre dialog about
void MainWindow::on_actionAbout_triggered()
{
    dialogAbout *about;
    about = new dialogAbout(this);
    about->setModal(true);
    about->show();
}

// abre dialogo de opciones
void MainWindow::on_actionOptions_triggered()
{
    dialogoptions *Options;
    Options = new dialogoptions(this);
    Options->show();
}


// ************************************************************************************************
// Save view options checked value in settings
void MainWindow::on_actionDebug_triggered(bool checked) {   settings->setValue("debug", checked, "View");    }

void MainWindow::on_actionStatusbar_triggered(bool checked) {   settings->setValue("statusbar", checked, "View");   }

void MainWindow::on_actionFullscreen_triggered(bool checked) {
    if (checked) {  this->showFullScreen();    }
    else {  this->showMaximized();  }
    settings->setValue("fullscreen", checked, "View");
}

// ejecuta javascript de test
void MainWindow::on_actionjavascript_triggered()
{
    this->ui->webview->testJavascript();
}

// boton de test
void MainWindow::on_actionTest_triggered()
{
    screener->fetchMarket("france");
    QList stk = DbManager::getInstance().loadStocks("france");
    for (const auto& stock : stk) {
        qDebug() << stock.ticker << stock.isin;
    }
}

void MainWindow::on_actionSaveHTML_triggered()
{
    this->ui->webview->page()->toHtml([this](const QString &html) {
        QFile file(settings->pathDir() + "/web.html");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << html;
            file.close();
        }
    });
}

// Al cerrar
void MainWindow::closeEvent(QCloseEvent* event) {
    qInfo() << "cerrando";
    m_closing = true;
    event->accept();
}