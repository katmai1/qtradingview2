#ifndef EXCHANGE_BASE_H
#define EXCHANGE_BASE_H

#include <QDebug>


class ExchangeBase{
public:
    virtual double getPrice(const QString &symbol) {
        qInfo() << "No se puede obtener precio del market "<< symbol.toUpper()
                    << " sin haber definido antes un exchange valido";
        return -1.0;
    };

    virtual void test() {
        qDebug() << "funcion de test de la clase ExchangeBase";
    }
};


#endif // EXCHANGE_BASE_H
