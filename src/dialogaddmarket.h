#ifndef DIALOGADDMARKET_H
#define DIALOGADDMARKET_H

#include <QDialog>

class MainWindow;
namespace Ui {
class dialogAddMarket;
}

class dialogAddMarket : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAddMarket(MainWindow *parent = nullptr);
    ~dialogAddMarket();

private slots:
    void on_buttonBox_accepted();

//    void on_editEx_textChanged(const QString &arg1);

    void on_comboEx_currentTextChanged(const QString &arg1);

private:
    Ui::dialogAddMarket *ui;
    MainWindow *mwin;
};

#endif // DIALOGADDMARKET_H
