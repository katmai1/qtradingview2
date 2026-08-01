#ifndef CUSTOMWEBENGINEVIEW_H
#define CUSTOMWEBENGINEVIEW_H

#include <QWebEngineView>
#include <QWebEnginePage>
#include "customwebenginepage.h"


class CustomWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit CustomWebEngineView(QWidget *parent = nullptr);

    void loadChart(QString ticker);
    QString currentSymbol() const { return m_symbol; }

public slots:
    void testJavascript();

private:
    QString m_symbol;
    CustomWebEnginePage *m_page = nullptr;
};

#endif // CUSTOMWEBENGINEVIEW_H
