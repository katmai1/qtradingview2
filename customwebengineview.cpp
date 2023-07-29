#include "customwebengineview.h"
#include "QWebEngineProfile"
#include <QDir>


CustomWebEngineView::CustomWebEngineView(QWidget *parent) : QWebEngineView(parent)
{
    // get carpeta home
    QString home_path = getenv("HOME");
    QString path_completo = home_path + "/.qtradingview2";
    QDir directorio(path_completo);
    if (!directorio.exists()) { directorio.mkpath(path_completo); }

    // configura perfil
    QWebEngineProfile *profile = new QWebEngineProfile(QString::fromLatin1("QTradingview.%1").arg(qWebEngineChromiumVersion()));
    profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
    profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    profile->setCachePath(path_completo);
    profile->setPersistentStoragePath(path_completo);

    // crea pagin i assigna
    QWebEnginePage *page = new QWebEnginePage(profile);
    this->setPage(page);

    this->load(QUrl("https://es.tradingview.com/chart/"));
    this->showMaximized();
    this->show();
}
