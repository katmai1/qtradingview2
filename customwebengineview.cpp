#include "customwebengineview.h"
#include "QWebEngineProfile"
#include <QDir>
#include "QProcess"
#include <QWebEngineSettings>

#include "qurlquery.h"
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

    // asigna m_symbol del market cargado, el symbol completo
    connect(this, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        QUrlQuery query(url);
        QString sym = QUrl::fromPercentEncoding(query.queryItemValue("symbol").toUtf8());
        if (!sym.isEmpty() && sym != m_symbol)
            qDebug() << "Symbol desde URL:" << sym;
            m_symbol = sym;
    });

    this->setPage(page);

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

void CustomWebEngineView::javaScriptConsoleMessage(
    QWebEnginePage::JavaScriptConsoleMessageLevel level,
    const QString &message,
    int lineNumber,
    const QString &sourceID)
{
    qDebug() << "entra" << message;
    if (message.contains("symbol=")) {
        QRegularExpression re("symbol=([A-Z]+:[A-Z0-9.]+)");
        QRegularExpressionMatch match = re.match(message);
        if (match.hasMatch()) {
            QString sym = match.captured(1);
            if (sym != m_symbol) {
                m_symbol = sym;
                qDebug() << "Symbol:" << m_symbol;
            }
        }
    }
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
