#include "customwebengineview.h"
#include "QWebEngineProfile"
#include <QDir>
#include "QProcess"
#include <QWebEngineSettings>


CustomWebEngineView::CustomWebEngineView(QWidget *parent) : QWebEngineView(parent)
{

    // crea pagin i assigna
    m_page = new CustomWebEnginePage(this);
    connect(m_page, &QWebEnginePage::loadFinished, this, &CustomWebEngineView::adBlockJS);

    this->setPage(m_page);

    this->load(QUrl("https://es.tradingview.com/chart/"));
    this->showMaximized();
    this->show();
}


void CustomWebEngineView::testJavascript() {
    // const adWrapper = document.querySelector('div[class^=toast-positioning-wrapper-]');
    QString code = R"delim(
        const hideAds = setInterval(() => {
            const adWrapper = document.querySelector("div[class^='wrap-']");
            if (adWrapper) {
                adWrapper.querySelector('button').click();
            }
        }, 5000);
    )delim";
    this->page()->runJavaScript(code);
}


void CustomWebEngineView::adBlockJS() {
    QString code = R"delim(
        const adBlock = setInterval(() => {
            const adBlock = document.querySelector("article[class^='toast-']");
            if (adBlock) {
                adBlock.querySelector('button').click();
            }
        }, 5000);
    )delim";
    this->page()->runJavaScript(code);
}

void CustomWebEngineView::loadChart(QString pair, QString exchange) {
    QStringList market = pair.split("/");
    QString url = "https://es.tradingview.com/chart/?symbol=" + exchange.toUpper() + ":" + market[0] + market[1];
    this->load(QUrl(url));
}
