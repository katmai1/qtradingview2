#ifndef SEARCHMARKET_H
#define SEARCHMARKET_H

#include <QDialog>
#include <QSqlQueryModel>

enum class AssetType { Stocks, Crypto };
namespace Ui {  class searchMarket; }


class searchMarket : public QDialog
{
    Q_OBJECT

public:
    explicit searchMarket(QWidget *parent = nullptr);
    ~searchMarket();

private:
    Ui::searchMarket *ui;
    QSqlQueryModel* m_model;
    void setupStocks();
    void setupCryptos();
};

#endif // SEARCHMARKET_H
