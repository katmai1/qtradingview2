#include "mainwindow.h"
#include "src/dialogabout.h"
#include "src/dialogaddmarket.h"
#include "src/dialogoptions.h"

#include "QObject"
#include "QMessageBox"
#include <QtWidgets>
#include <QTextEdit>
#include <QDebug>
#include <QString>

#include "src/uimanager.h"
#include "src/marketslist.h"
#include "src/portfolio.h"
#include "src/dialogaddposition.h"
#include "src/exmanager.h"


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

    // activa widgets visibles segun ultima sesion
    this->ui->dockDebug->setVisible(settings->getValue("debug", false, "View").toBool());
    this->ui->dockMarkets->setVisible(settings->getValue("markets", false, "View").toBool());
    this->ui->statusbar->setVisible(settings->getValue("statusbar", false, "View").toBool());
    this->ui->actionStatusbar->setChecked(settings->getValue("statusbar", false, "View").toBool());

    // carga markets list
    this->loadListMarkets();

    // declara menus contextuales
    menuCtx = new MenuContextual(ui->listMarkets);
    connect(menuCtx, &MenuContextual::sigLoadMarket, this, &MainWindow::loadMarket);
    connect(menuCtx, &MenuContextual::sigAddPosition, this, &MainWindow::addPosition);
    connect(menuCtx, &MenuContextual::sigSaveMarketsList, this, &MainWindow::saveMarketsList);

    // redirige mensajes debug
    qInstallMessageHandler(customMessageHandler);
    qInfo() << "Iniciando version: " << QString("v%1").arg(VERSION_FULL);

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

// abre el dialogo addMarket
void MainWindow::on_btAdd_clicked()
{
    dialogAddMarket *addMarket;
    addMarket = new dialogAddMarket(this);
    addMarket->setModal(true);
    addMarket->show();
}

// abre dialogo de opciones
void MainWindow::on_actionOptions_triggered()
{
    dialogoptions *Options;
    Options = new dialogoptions(this);
    Options->show();
}

void MainWindow::on_actionPortfolio_triggered()
{
    Portfolio *portfolio;
    portfolio = new Portfolio(this);
    portfolio->show();
}

// ************************************************************************************************
// Slots del menu contextual

// load selected market
void MainWindow::loadMarket(QListWidgetItem *item) {
    this->on_listMarkets_itemDoubleClicked(item);
}

// open dialog to add position to portfolio
void MainWindow::addPosition(QListWidgetItem *item) {
    Portfolio *portfolio;
    portfolio = new Portfolio(this);
    portfolio->show();

    dialogAddPosition *addPosition;
    QString market = item->text();
    QString exchange = item->toolTip();

    ExchangeBase *ex = getExchangeClass(exchange.toLower());
    double lastPrice = ex->getPrice(market.toUpper());

    addPosition = new dialogAddPosition(portfolio, exchange, market, lastPrice);
    addPosition->setModal(true);
    connect(addPosition, &dialogAddPosition::signalAddPosition, portfolio, &Portfolio::slotAddPosition);
    addPosition->show();

}

// save markets list
void MainWindow::saveMarketsList() {
    MarketsList ml(this->ui->listMarkets);
    ml.saveList();
}


// ************************************************************************************************
// Save view options checked value in settings
void MainWindow::on_actionDebug_triggered(bool checked) {   settings->setValue("debug", checked, "View");    }

void MainWindow::on_actionMarkets_triggered(bool checked)   { settings->setValue("markets", checked, "View"); }

void MainWindow::on_actionStatusbar_triggered(bool checked) {   settings->setValue("statusbar", checked, "View");   }

void MainWindow::on_actionFullscreen_triggered(bool checked) {
    if (checked) {  this->showFullScreen();    }
    else {  this->showMaximized();  }
    settings->setValue("fullscreen", checked, "View");
}


// ************************************************************************************************

// ************************************************************************************************
// add market to list
void MainWindow::addToList(QString market) {
    MarketsList ml(this->ui->listMarkets);
    ml.addMarket(market);
    ml.saveList();
}

// load markets list
void MainWindow::loadListMarkets()
{
    MarketsList ml(this->ui->listMarkets);
    ml.loadList();
}

// execute a custom js code
void MainWindow::on_actionjavascript_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Ingresar Texto"),
        tr("Por favor, ingresa un texto:"), QLineEdit::Normal,
        "", &ok);
    qDebug() << text;
    this->ui->webview->testJavascript(text);
}

// test button
void MainWindow::on_actionTest_triggered()
{
    try {
        ExchangeBase *ex = getExchangeClass("bingx");
        qDebug() << ex->getPrice("KAS/USDT");
        delete ex;
    }
    catch (QException e) { qDebug() << e.what(); }

}


// double click event on market list
void MainWindow::on_listMarkets_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "cargando market...";
    QString pair = item->text();
    QString exchange = item->toolTip();
    this->ui->webview->loadChart(pair, exchange);
}

// filtrer markets
void MainWindow::on_edFilter_textChanged(const QString &arg1)
{
    int total = this->ui->listMarkets->count();
    for (int i = 0; i < total; ++i) {
        QListWidgetItem* item = this->ui->listMarkets->item(i);
        const bool matches = item->text().contains(arg1.toUpper(), Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

// save HTML code into web.html file
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
