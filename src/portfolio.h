#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <QDialog>

namespace Ui {
class Portfolio;
}

class Portfolio : public QDialog
{
    Q_OBJECT

public:
    explicit Portfolio(QWidget *parent = nullptr);
    ~Portfolio();

private:
    Ui::Portfolio *ui;
};

#endif // PORTFOLIO_H
