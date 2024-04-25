#ifndef EXMANAGER_H
#define EXMANAGER_H

#include "src/exchanges/exchange_base.h"
#include "src/exchanges/binance.h"
#include "src/exchanges/kucoin.h"


class ExManager {
public:
    ExchangeBase* setExchange(QString exchange) {
        if (exchange == "binance") {    return new BinanceAPI;  }
        else if (exchange == "kucoin") {    return new KucoinAPI;  }
        else {  return new ExchangeBase;    }
    }
};

#endif // EXMANAGER_H
