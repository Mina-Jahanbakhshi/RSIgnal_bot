#pragma once
#include <cstdio>
#include <iostream>
#include <string.h> 
#include <string> 
#include <unistd.h>
#include <curl/curl.h>

using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
string CURL_MAKE_REQUEST(string URL, string parameters, string headerstring, bool secure, string request_type);