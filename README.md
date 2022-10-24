# RSIgnal_bot
binance crypro RSI calculator (same as tradingview)

## Overview
This code will calculate RSI according to the TradinView and it will be using Binance API. To Compute RSI and have the near exact number as tradingview, the standard formula has been used :

```
RSI = 100 – 100/ (1 + RS)

RS = Average Gain of n days UP  / Average Loss of n days DOWN
```

RSI  provides a visual mean to monitor both the current, as well as historical, strength and weakness of a particular market. 

The strength or weakness is based on closing prices over the duration of a specified trading period creating a reliable metric of price and momentum changes.

The Partial difference between RSI in small timeframes such as 1minutes in calculated RSI using this code, oppose to tradingview RSI, is because of unmutual starting point for getting RMA (relative moving average).

```
NOTICE: It is important to maintain an stable internet, without filtering connection to the Binance. 
```

## Dependencies :
The following libraries must be installed before building the RSIgnal_bot code.
```
curl
Json
```

To install the required libraries execute the following command :

```
sudo apt install git -y

sudo apt install build-essential -y

sudo apt-get install libcurl4-openssl-dev -y
```

## Build :
To build the RSIgnal_bot execute the following commands.

```
git clone https://github.com/Mina-Jahan/RSIgnal_bot.git

cd RSIgnal_bot/RSIgnal-v1.1/

g++ -c apiclient.cpp  curl.cpp  main.cpp

g++ apiclient.o  curl.o  main.o -lcurl -lpthread -o RSIgnal
```

## Running the RSIgnal_bot :
 To run this code, tow entries are mandatory, COINPAIR and INTERVAL. For example :
 
 ```
 for 1 minute RSI:
 
./RSIgnal BTCUSDT 1m
```

OR
```
For 1 hour RSI:

./RSIgnal BTCUSDT 1h
```

OR
```
for 1 Day RSI:

./RSIgnal BTCUSDT 1D
```

### Authored By :
Mina Jahanbakhshi
