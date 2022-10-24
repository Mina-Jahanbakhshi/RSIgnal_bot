#include "curl.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string CURL_MAKE_REQUEST(string URL, string parameters, string headerstring, bool secure, string request_type)
{
	
	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {

		curl_global_init(CURL_GLOBAL_ALL);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.38.0");

		if (request_type == "POST")
		{
			curl_easy_setopt(curl, CURLOPT_POST, 1);
		}
		else if (request_type == "GET")
		{
			curl_easy_setopt(curl, CURLOPT_POST, 0);
		}

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //debug

		if (!secure)
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);  // for --insecure option
		}

		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

		if (request_type == "POST")
		{
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, parameters.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, parameters.length());
		}

		if (!headerstring.empty())
		{
			struct curl_slist* headers = NULL;
			headers = curl_slist_append(headers, "Content-Type: application/json");
			headers = curl_slist_append(headers, headerstring.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		}

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return readBuffer;

}//CURL_MAKE_REQUEST

