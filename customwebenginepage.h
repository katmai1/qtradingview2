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

        // run adblock
        connect(this, &QWebEnginePage::loadFinished, this, [this](bool ok) {
            if (!ok) return;
            qDebug() << "AdBlock running";
            runJavaScript(R"delim(
            const adBlock = setInterval(() => {
                const adBlock = document.querySelector("article[class^='toast-']");
                if (adBlock) {
                    adBlock.querySelector('button').click();
                }
            }, 5000);
        )delim");
        });
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
        profile->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
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

};

#endif // CUSTOMWEBENGINEPAGE_H
