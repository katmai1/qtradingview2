#include "customwebenginepage.h"
#include <QRegularExpression>
#include <QTimer>

CustomWebEnginePage::CustomWebEnginePage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage{profile, parent}
{}


void CustomWebEnginePage::javaScriptConsoleMessage(
    JavaScriptConsoleMessageLevel level,
    const QString &message,
    int lineNumber,
    const QString &sourceID)
{
    if (message.contains("Check your sub/unsub")) {
        QTimer::singleShot(300, this, [this]() {
            runJavaScript("document.title", [this](const QVariant &result) {
                QRegularExpression re("([A-Z]+:[A-Z0-9.]+)");
                QRegularExpressionMatch match = re.match(result.toString());
                if (match.hasMatch()) { emit symbolChanged(match.captured(1));  }
            });
        });
    }
}