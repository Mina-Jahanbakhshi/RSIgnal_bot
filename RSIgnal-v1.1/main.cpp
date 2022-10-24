#include <cstdio>
#include <iostream>
#include <string.h> 
#include <string> 
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <string.h> 
#include <string> 
#include <unistd.h>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>

#include "apiclient.h"

using namespace std;


int main(int argc, char *argv[])
{

	if (argc < 3)
	{
		//print usage
		cout << "Usage:\n ./RSIgnal_bot COINPAIR interval" << endl;
		cout << "Example:\n ./RSIgnal_bot BTCUSDT 1m" << endl;
		exit(0);
	}
	
	string COINPAIR = argv[1];
	string interval = argv[2];

	apiclient binance_api;
	string url = "https://api1.binance.com/api/v3/ticker/price?symbol=";
	url += COINPAIR;
	binance_api.get_single_coin_price_url = url;
	binance_api.get_price_url = "https://api1.binance.com/api/v3/ticker/price";

	binance_api.update_price();
	binance_api.get_rsi(COINPAIR, interval);

	
	while (true)
	{
		sleep(1);
	}
}