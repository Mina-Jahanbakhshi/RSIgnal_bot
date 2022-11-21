#include "apiclient.h"


void apiclient::Get_Prices()
{
	while (true)
	{
		try
		{
			string Binance_get_price = CURL_MAKE_REQUEST(get_single_coin_price_url, "", "", true, "GET");
			json j_complete = json::parse(Binance_get_price);
			string temp = j_complete.at("/price"_json_pointer);
			current_price = stod(temp);
			Get_Price_successful = true;

		}
		catch (json::exception& e)
		{
				std::cout << "Get_Prices Error:" << e.what() << '\n';
				Get_Price_successful = false;

		}
	}
}//Get_Prices


void apiclient::update_price()
{
	thread t1(&apiclient::Get_Prices, this);
	t1.detach();
}//update_price

void apiclient::get_rsi(string COINPAIR, string interval)
{
	thread t1(&apiclient::initiate_rsi, this, COINPAIR, interval);
	t1.detach();
}//get_rsi

void apiclient::change_calc(double array[], double array_up[], double array_down[], int size)
{
	double temp;
	for (int i = 1; i < size; i++)
	{
		temp = array[i] - array[i - 1];

		if (temp < 0)
		{
			temp *= -1;
			array_down[i - 1] = temp;
			array_up[i - 1] = 0;
			
		}//if
		else
		{
			array_up[i - 1] = temp;
			array_down[i - 1] = 0;
			
		}//else
	}//for
}//change_calc


void apiclient::initiate_rsi(string COINPAIR, string interval)
{
	double ALL_klines_CLOSE[1000];
	int kline_num = 0;
	int gap = 10;
	while (true)
	{
		get_klines(COINPAIR, interval, ALL_klines_CLOSE, 1000, kline_num);
		if (kline_num <= 0)
		{
			cout << "Can Not Retrive Data.Please Check Your Internet Connection." << endl;
		}//if
		else
		{
			break;
		}//else

		cout << "will try agian in " << gap << "seconds" << endl;
		sleep(gap);

		if (gap > 40)
		{
			gap = 40;
		}//if
		else
		{
			gap = gap * 2;
		}//else

	}//while;what if we can't fetch klines?

	cout << "!!!!!!!!!!!!!GOT klines successfully" << endl;

	double change_closes_up[kline_num-1];
	double change_closes_down[kline_num-1];

	cout << "Getting change_calc" << endl;
	change_calc(ALL_klines_CLOSE, change_closes_up, change_closes_down, kline_num);
	cout << "change_calc done" << endl;

	double rma_up[kline_num-15];
	double rma_down[kline_num-15];

	
	cout << "Getting rma_calc" << endl;
	rma_calc(change_closes_up, rma_up, kline_num - 1, 14);
	rma_calc(change_closes_down, rma_down, kline_num - 1, 14);
	cout << "rma_calc done" << endl;

	double RSI[kline_num-15];

	rsi_calc(rma_up, rma_down, RSI, kline_num - 15, 14);

	for (int i = 0; i < kline_num - 15; i++)
	{
		cout << COINPAIR << " RSI" << i << "=" << RSI[i] << endl;
	}//for
	cout << "executing update_rsi" << endl;
	update_rsi(COINPAIR, interval, rma_up[kline_num-16], rma_down[kline_num-16], 14);

}//initiate_rsi

void apiclient::get_klines(string symbol, string interval, double ALL_klines_CLOSE[], int size, int& kline_num)
{
	string url = "https://api1.binance.com/api/v3/klines";

	stringstream ss;
	ss << url << "?symbol=" << symbol << "&interval=" << interval << "&limit=" << size;
	string response = CURL_MAKE_REQUEST(ss.str(), "", "", true, "GET");
	//cout << response << endl;

	try {

		json j_complete = json::parse(response);

		int i = 0;

		for (auto it = j_complete.begin(); it != j_complete.end(); ++it)
		{
			string CLOSE = (*it)[4].get<std::string>();
			ALL_klines_CLOSE[i] = stod(CLOSE);

			i++;

			if (i == size)
			{
				break;
			}


		}//for

		
		kline_num = i;

	}//try
	catch (json::exception& e)
	{
		cout << "#######################get_klines ERROR:" << endl;
		std::cout << e.what() << '\n';
	}//catch

}//get_klines

void apiclient::rsi_calc(double rma_up[], double rma_down[], double rsi_array[], int rma_size, int period)
{
	for (int i = 0; i < rma_size; i++)
	{
		double rsi = 100 - (100 / (1 + rma_up[i] / rma_down[i]));
		rsi_array[i] = rsi;
	}//for
}//rsi_calc

void apiclient::rma_calc(double change_array[], double rma_array[], int change_array_size, int period)
{
	//sma
	double sma = 0;

	for (int i = 0; i < period; i++)
	{
		sma += change_array[i];

	}//for

	sma = sma / period;
	sma = ceil(sma * 100.0) / 100.0;

	double alpha = (double)1 / period;
	int j = 1;
	rma_array[0] = sma;

	for (int i = period; i < change_array_size; i++)
	{
		double rma = alpha * change_array[i] + (1 - alpha) * rma_array[j - 1];
		rma_array[j] = rma;
		j++;
	}//for

}//rma_calc


double apiclient::sma_calculator(double* a1, int len)
{
	double sma = 0;

	for (int i = 0; i < len; i++)
	{
		sma += a1[i];
	}//for
	sma = sma / len;
	return sma;

}//sma_calculator


void apiclient::update_rsi(string COINPAIR, string interval, double rma_up_prev, double rma_down_prev, int period)
{
	sleep(2); // this is for letting price get update

	double ALL_klines_CLOSE[2];
	int kline_num = 0;
	int gap = 10;
	while (true)
	{
		get_klines(COINPAIR, interval, ALL_klines_CLOSE, 2, kline_num);
		if (kline_num <= 0)
		{
			cout << "update_rsi:Can Not Retrive Data.Please Check Your Internet Connection." << endl;
		}//if
		else
		{
			break;
		}//else

		cout << "update_rsi:will try agian in " << gap << "seconds" << endl;
		sleep(gap);

		if (gap > 40)
		{
			gap = 3600;
		}//if
		else
		{
			gap = gap * 2;
		}//else

	}//while

	cout << "update_rsi:get_klines done" << endl;
	double close = ALL_klines_CLOSE[1];
	double close_prev = ALL_klines_CLOSE[0];

	time_t last_candle_close_time = last_candle_close(interval);

	if (last_candle_close_time == 0)
	{
		cout << "rsi_calc_new:INVALID INTERVAL" << endl;
		exit(0);
	}//if
	

	while (true)
	{
		if (Get_Price_successful)
		{
			close = current_price;

			double change_down;
			double change_up;
			double change = close - close_prev;
			if (change < 0)
			{
				change *= -1;
				change_down = change;
				change_up = 0;
			}//if
			else
			{
				change_up = change;
				change_down = 0;
			}//else

			double alpha = (double)1 / period;
			double rma_up = alpha * change_up + (1 - alpha) * rma_up_prev;
			double rma_down = alpha * change_down + (1 - alpha) * rma_down_prev;
			double rsi = 100 - (100 / (1 + rma_up / rma_down));
			cout << "####" << COINPAIR << " RSI=" << rsi << endl;

			if (get_timestamp() >= last_candle_close_time)
			{
				cout << "Candle Closed" << endl;
				last_candle_close_time = last_candle_close(interval);

				kline_num = 0;

				while (true)
				{
					get_klines(COINPAIR, interval, ALL_klines_CLOSE, 2, kline_num);

					if (kline_num <= 0)
					{
						cout << "Could Not Retrive Data" << endl;
						if (get_timestamp() <= (last_candle_close_time - 10000))
						{
							sleep(10);
						}//if
						else
						{
							get_rsi(COINPAIR, interval);
							return;
						}//else
					}//if
					else
					{
						break;
					}//else
				}//while

				double close_prev_prev = close_prev;
				double rma_up_prev_prev = rma_up_prev;
				double rma_down_prev_prev = rma_down_prev;

				close = ALL_klines_CLOSE[1];
				close_prev = ALL_klines_CLOSE[0];

				change = close_prev - close_prev_prev;
				if (change < 0)
				{
					change *= -1;
					change_down = change;
					change_up = 0;
				}//if
				else
				{
					change_up = change;
					change_down = 0;
				}//else

				rma_up_prev = alpha * change_up + (1 - alpha) * rma_up_prev_prev;
				rma_down_prev = alpha * change_down + (1 - alpha) * rma_down_prev_prev;
			}//if
		}
		else
		{
			if (get_timestamp() <= (last_candle_close_time - 10000))
			{
				cout << "update_rsi:ERROR:Get Recent Price Was Not Successful" << endl;
			}
			else
			{
				cout << "update_rsi:ERROR:calling get_rsi" << endl;
				get_rsi(COINPAIR, interval);
				return;
			}
		}
		sleep(1);
	}//while

}//update_rsi


time_t apiclient::last_candle_close(string interval)
{
	time_t now_time = get_timestamp();
	time_t candlestick_close_time;
	int seconds_in_interval = 0;

	if (interval == "1m")
	{
		seconds_in_interval = 60;
	}//if
	else if (interval == "1h")
	{
		seconds_in_interval = 60 * 60;
	}//else if
	else if (interval == "1D")
	{
		seconds_in_interval = (60 * 60) * 24;
	}//else if
	else
	{
		return 0;
	}//else

	int milli_seconds = seconds_in_interval * 1000;
	candlestick_close_time = (now_time - (now_time % (milli_seconds)));
	return(candlestick_close_time + milli_seconds);

}//last_candle_close


time_t apiclient::get_timestamp()
{
	struct timeval time_now {};
	gettimeofday(&time_now, nullptr);
	time_t TM = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

	return TM;

}//get_timestamp

const string apiclient::currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}//currentDateTime
