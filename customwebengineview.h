#ifndef CUSTOMWEBENGINEVIEW_H
#define CUSTOMWEBENGINEVIEW_H

#include "qdir.h"
#include <QWebEngineView>

class CustomWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit CustomWebEngineView(QWidget *parent = nullptr);

    QString path_completo;
    QDir directorio;

    void loadChart(QString symbol);

public slots:
    void testJavascript();
    void adBlockJS();

private:
    void init_folder();
    // Puedes agregar aquí cualquier variable adicional que desees.

};

#endif // CUSTOMWEBENGINEVIEW_H
