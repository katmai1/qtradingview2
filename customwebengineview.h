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
    void testJavascript(QString texto);
public slots:

    void adBlockJS(bool ok);

};

#endif // CUSTOMWEBENGINEVIEW_H
