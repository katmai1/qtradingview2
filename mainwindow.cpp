#include "mainwindow.h"
#include "src/dialogabout.h"
#include "src/dialogoptions.h"

#include "QObject"
#include "QMessageBox"
#include <QtWidgets>
#include <QTextEdit>
#include <QDebug>
#include <QString>

#include "src/uimanager.h"
#include "src/tvscreener.h"
#include "dbmanager.h"


// ************************************************************************************************
// Función personalizada para manejar los mensajes de depuración
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


// ************************************************************************************************
// Main Window

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(new SettingsManager())
{
    ui->setupUi(this);
    UIManager::getInstance()->setTextEdit(ui->txtDebug);

    // load dock stocks
    dockStock = new StockDockWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, dockStock);
    ui->actionStocks->setChecked(settings->getValue("stock", false, "View").toBool());
    connect(ui->actionStocks, &QAction::toggled, dockStock, &QDockWidget::setVisible);
    connect(dockStock, &QDockWidget::visibilityChanged, ui->actionStocks, &QAction::setChecked);

    // load docks
    this->ui->dockDebug->setVisible(settings->getValue("debug", false, "View").toBool());
    // this->ui->dockMarkets->setVisible(settings->getValue("markets", false, "View").toBool());
    this->ui->statusbar->setVisible(settings->getValue("statusbar", false, "View").toBool());
    this->ui->actionStatusbar->setChecked(settings->getValue("statusbar", false, "View").toBool());

    // init db
    DbManager::getInstance().init("qtradingview2.db");

    // redirige mensajes debug
    qInstallMessageHandler(customMessageHandler);
    qInfo() << "Iniciando version: " << QString("v%1").arg(VERSION_FULL);

    // screener...
    screener = new TvScreener(this);

    connect(screener, &TvScreener::dataReady, [](const QString& market, const QList<Stock>& stocks, int total) {
        qDebug() << "Loaded %1" << total;
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
    QList stocks = DbManager::getInstance().loadCrypto("BINANCE");
    for (const auto& s : stocks) {
        qDebug() << s.description;
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
    settings->setValue("stock", dockStock->isVisible(), "View");
    event->accept();
}