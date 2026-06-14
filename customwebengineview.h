#ifndef CUSTOMWEBENGINEVIEW_H
#define CUSTOMWEBENGINEVIEW_H

#include <QWebEngineView>
#include <QWebEnginePage>


class CustomWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit CustomWebEngineView(QWidget *parent = nullptr);

    void loadChart(QString pair, QString exchange);
    QString currentSymbol() const { return m_symbol; }

public slots:
    void testJavascript();
    void adBlockJS();

private:
    QString m_symbol;
};

#endif // CUSTOMWEBENGINEVIEW_H
