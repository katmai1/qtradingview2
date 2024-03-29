#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>
#include "config.h"

class MainWindow;
namespace Ui {
class dialogoptions;
}

class dialogoptions : public QDialog
{
    Q_OBJECT

public:
    explicit dialogoptions(MainWindow *parent = nullptr);
    ~dialogoptions();

private slots:
    void on_buttonBox_accepted();
    void addLanguages();

private:
    Ui::dialogoptions *ui;
    MainWindow *mwin;
    config settings;
};

#endif // DIALOGOPTIONS_H
