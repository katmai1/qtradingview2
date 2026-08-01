#include "customwebengineview.h"
#include "QWebEngineProfile"
#include <QDir>
#include "QProcess"
#include <QWebEngineSettings>


CustomWebEngineView::CustomWebEngineView(QWidget *parent) : QWebEngineView(parent)
{

    // crea pagin i assigna
    m_page = new CustomWebEnginePage(this);
    // connect(m_page, &QWebEnginePage::loadFinished, this, &CustomWebEngineView::adBlockJS);

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

void CustomWebEngineView::loadChart(QString ticker) {
    QString url = "https://es.tradingview.com/chart/?symbol=" + ticker;
    this->load(QUrl(url));
}
