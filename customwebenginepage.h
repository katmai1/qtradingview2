#ifndef CUSTOMWEBENGINEPAGE_H
#define CUSTOMWEBENGINEPAGE_H

#include <QWebEnginePage>

class CustomWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit CustomWebEnginePage(QObject *parent = nullptr);

signals:
    void symbolChanged(const QString &symbol);

protected:
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level,
                                  const QString &message,
                                  int lineNumber,
                                  const QString &sourceID) override;

};

#endif // CUSTOMWEBENGINEPAGE_H
