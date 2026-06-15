#ifndef CUSTOMWEBENGINEPAGE_H
#define CUSTOMWEBENGINEPAGE_H

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QWebEngineSettings>

#include "src/settings.h"


class CustomWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit CustomWebEnginePage(QObject *parent = nullptr) : QWebEnginePage(createProfile(parent), parent)
    {
        setupCookies();
    }

private:
    static QWebEngineProfile* createProfile(QObject *parent) {
        auto *profile = new QWebEngineProfile("qtradingview2", parent);
        SettingsManager settings;
        profile->setPersistentCookiesPolicy(QWebEngineProfile::AllowPersistentCookies);
        profile->setCachePath(settings.pathDir());
        profile->setPersistentStoragePath(settings.pathDir());
        profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
        profile->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
        profile->settings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, true);
        return profile;
    }

    void setupCookies() {
        auto *store = profile()->cookieStore();
        store->setCookieFilter([](const QWebEngineCookieStore::FilterRequest &) {
            return true; // permite todas las cookies
        });
    }

signals:
    void symbolChanged(const QString &symbol);

protected:
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                  const QString &message,
                                  int lineNumber,
                                  const QString &sourceID) override;

};

#endif // CUSTOMWEBENGINEPAGE_H
