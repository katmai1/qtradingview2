#ifndef DIALOGADDPOSITION_H
#define DIALOGADDPOSITION_H

#include <QDialog>

namespace Ui {
class dialogAddPosition;
}

class dialogAddPosition : public QDialog
{
    Q_OBJECT

public:
    explicit dialogAddPosition(QWidget *parent = nullptr, const QString &exchange="", const QString &market="", const double &buyPrice=-1);
    ~dialogAddPosition();

signals:
    void signalAddPosition(QString exchange, QString market, double amount, double buyPrice);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::dialogAddPosition *ui;
    QString m_exchange;
    QString m_market;
    double m_buyPrice;
};

#endif // DIALOGADDPOSITION_H
