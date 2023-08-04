#include "customwebengineview.h"
#include "QWebEngineProfile"
#include <QDir>
#include "QProcess"


CustomWebEngineView::CustomWebEngineView(QWidget *parent) : QWebEngineView(parent)
{
    // prepara carpeta config
    QString home_path = getenv("HOME");
    path_completo = home_path + "/.qtradingview2";
    directorio.setPath(path_completo);
    // primer inici
    if (!directorio.exists()) { this->init_folder(); }

    // configura perfil
    QWebEngineProfile *profile = new QWebEngineProfile(QString::fromLatin1("QTradingview.%1").arg(qWebEngineChromiumVersion()));
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    profile->setCachePath(path_completo);
    profile->setPersistentStoragePath(path_completo);

    // crea pagin i assigna
    QWebEnginePage *page = new QWebEnginePage(profile);
    connect(page, &QWebEnginePage::loadFinished, this, &CustomWebEngineView::testJavascript);

    this->setPage(page);

    this->load(QUrl("https://es.tradingview.com/chart/"));
    this->showMaximized();
    this->show();
}


void CustomWebEngineView::init_folder() {
    this->directorio.mkpath(path_completo);
    QString binance =  R"delim(curl -s 'https://api.binance.com/api/v3/exchangeInfo' | jq -r '.symbols[] | "\(.baseAsset)/\(.quoteAsset)"' > )delim";
    binance += path_completo + "/binance.txt";
    std::system(binance.toUtf8().constData());
}


void CustomWebEngineView::testJavascript() {
    qDebug("entraaa");
    QString code = R"delim(
        const hideAds = setInterval(() => {
            const adWrapper = document.querySelector('div[class^=toast-positioning-wrapper-]');
            if (adWrapper) {
                adWrapper.querySelector('button').click();
            }
        }, 10000);
    )delim";
    this->page()->runJavaScript(code);
}

void CustomWebEngineView::loadChart(QString symbol) {
    QStringList market = symbol.split("/");
    QString url = "https://es.tradingview.com/chart/?symbol=BINANCE:" + market[0] + market[1];
    this->load(QUrl(url));
}
