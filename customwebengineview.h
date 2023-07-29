#ifndef CUSTOMWEBENGINEVIEW_H
#define CUSTOMWEBENGINEVIEW_H

#include <QWebEngineView>

class CustomWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit CustomWebEngineView(QWidget *parent = nullptr);
    // Puedes agregar aquí cualquier función adicional que desees.

private:
    // Puedes agregar aquí cualquier variable adicional que desees.
};

#endif // CUSTOMWEBENGINEVIEW_H
