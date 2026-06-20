#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/tvscreener.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QTextEdit>

#include "src/systray.h"
#include "src/settings.h"
#include "src/stockdockwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
    void setSystemTrayWindow(SystemTrayIcon *SystemTrayIcon) {
        this->trayIcon = SystemTrayIcon;
    }

    void sendStatus(QString message, int timeout);


private slots:
    void on_actionjavascript_triggered();

    void on_actionTest_triggered();

    void on_actionAbout_triggered();

    void on_actionFullscreen_triggered(bool checked);

    void on_actionOptions_triggered();

    void on_actionDebug_triggered(bool checked);

    void on_actionStatusbar_triggered(bool checked);

    void on_actionSaveHTML_triggered();

protected:
    void closeEvent(QCloseEvent* event) override;

public slots:

private:
    Ui::MainWindow *ui;
    QTextEdit *tdebug;
    SystemTrayIcon *trayIcon;
    SettingsManager *settings;
    TvScreener* screener;
    StockDockWidget* dockStock;

    bool m_closing = false;
    inline void connectDock(QDockWidget* dock, QAction* action, const QString& key) {
        bool visible = settings->getValue(key, false, "View").toBool();
        dock->setVisible(visible);
        action->setChecked(visible);

        connect(action, &QAction::toggled, [this, dock, key](bool checked) {
            dock->setVisible(checked);
            settings->setValue(key, checked, "View");
        });
        connect(dock, &QDockWidget::visibilityChanged, [this, action, key](bool visible) {
            if (m_closing) return;
            action->setChecked(visible);
            settings->setValue(key, visible, "View");
        });
    }
};

#endif // MAINWINDOW_H
