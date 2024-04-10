#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

namespace Ui {
class dialogAbout;
}

class dialogAbout : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAbout(QWidget *parent = nullptr);
    ~dialogAbout();

private:
    Ui::dialogAbout *ui;
};

#endif // DIALOGABOUT_H
