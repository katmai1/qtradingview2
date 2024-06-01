#include "customwebengineview.h"
#include "QWebEngineProfile"
//#include <QDir>
#include "QProcess"
#include <QWebEngineSettings>

#include "src/settings.h"


CustomWebEngineView::CustomWebEngineView(QWidget *parent) : QWebEngineView(parent)
{

    SettingsManager settings;

    // configura perfil
    QWebEngineProfile *profile = new QWebEngineProfile(QString::fromLatin1("QTradingview.%1").arg(qWebEngineChromiumVersion()));
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    profile->setCachePath(settings.pathDir());
    profile->setPersistentStoragePath(settings.pathDir());
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    profile->settings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, true);

    // crea pagin i assigna
    QWebEnginePage *page = new QWebEnginePage(profile);
    connect(page, &QWebEnginePage::loadFinished, this, &CustomWebEngineView::adBlockJS);

    this->setPage(page);

    this->load(QUrl("https://es.tradingview.com/chart/"));
    this->showMaximized();
    this->show();
}


void CustomWebEngineView::testJavascript(QString texto) {
    //QString code = R"delim(texto)delim";
    this->page()->runJavaScript(texto);
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
