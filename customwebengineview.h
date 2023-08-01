#ifndef CUSTOMWEBENGINEVIEW_H
#define CUSTOMWEBENGINEVIEW_H

#include <QWebEngineView>

class CustomWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit CustomWebEngineView(QWidget *parent = nullptr);

    QString path_completo;

    void loadChart(QString symbol);

private:
    // Puedes agregar aquí cualquier variable adicional que desees.

};

#endif // CUSTOMWEBENGINEVIEW_H
