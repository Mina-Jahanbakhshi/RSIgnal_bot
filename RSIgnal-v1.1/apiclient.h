#pragma once
#include <cstdio>
#include <iostream>
#include <string.h> 
#include <string> 
#include <unistd.h>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
#include "json.hpp"
#include "curl.h"

using namespace std;
using json = nlohmann::json;



class apiclient
{
public:
	//variables:
	string get_single_coin_price_url;
	string get_price_url;
	double current_price = 0;
	bool Get_Price_successful = false;



	//functions
	time_t get_timestamp();
	const string currentDateTime();
	
	void Get_Prices();
	
	
	void get_klines(string symbol, string interval, double ALL_klines_CLOSE[], int size, int& kline_num);
	double sma_calculator(double* a1, int len);
	void change_calc(double array[], double array_up[], double array_down[], int size);
	void rma_calc(double change_array[], double rma_array[], int change_array_size, int period);
	void rsi_calc(double rma_up[], double rma_down[], double rsi_array[], int rma_size, int period);
	void initiate_rsi(string COINPAIR, string interval);
	void update_rsi(string COINPAIR, string interval, double rma_up_prev, double rma_down_prev, int period);
	void update_price();
	time_t last_candle_close(string interval);
	void get_rsi(string COINPAIR, string interval);


};

