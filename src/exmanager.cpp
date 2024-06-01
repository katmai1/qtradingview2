#include "src/exmanager.h"
#include "src/exchanges/binance.h"
#include "src/exchanges/kucoin.h"
#include "src/exchanges/bingx.h"

// return exchange class
ExchangeBase* getExchangeClass(QString exchange) {
    if (exchange == "binance") {    return new Binance;  }
    else if (exchange == "kucoin") {    return new Kucoin;  }
    else if (exchange == "bingx") {    return new Bingx;  }
    else {  return new ExchangeBase;    }
}

// calculate profit value
double calcProfit(double amount, double buyPrice, double lastPrice) {
    return amount * (lastPrice - buyPrice);
}

// calculate profit percentage
double calcProfitPercent(double amount, double buyPrice, double lastPrice) {
    double profit = amount * (lastPrice - buyPrice);
    return (profit / (amount * buyPrice)) * 100.0;
}
