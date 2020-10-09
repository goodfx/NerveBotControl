#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <curl/curl.h>
#include "libcurl.h"
#include "config.h"
#include "StringConvert.h"


#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
fcurl_easy_init			pcurl_easy_init;
fcurl_easy_setopt		pcurl_easy_setopt;
fcurl_easy_perform		pcurl_easy_perform;
fcurl_easy_cleanup		pcurl_easy_cleanup;
fcurl_slist_append		pcurl_slist_append;
fcurl_slist_free_all	pcurl_slist_free_all;
fcurl_formadd			pcurl_formadd;
fcurl_global_init		pcurl_global_init;
fcurl_global_cleanup	pcurl_global_cleanup;
HMODULE hLibcurl = NULL;
bool is_save_httpdata = false;
char* HttpBuffer = new char[MAX_PATH * 1024];//存放http数据的内存
extern std::string APIURL;


CRITICAL_SECTION cs;
void IniLibCurl()
{
	if (hLibcurl == NULL)
	{
		//InitializeCriticalSection(&cs);		
		HMODULE hLibcurl = LoadLibrary(L"libcurl.dll");
		pcurl_easy_init = (fcurl_easy_init)GetProcAddress(hLibcurl, "curl_easy_init");
		pcurl_easy_setopt = (fcurl_easy_setopt)GetProcAddress(hLibcurl, "curl_easy_setopt");
		pcurl_easy_perform = (fcurl_easy_perform)GetProcAddress(hLibcurl, "curl_easy_perform");
		pcurl_easy_cleanup = (fcurl_easy_cleanup)GetProcAddress(hLibcurl, "curl_easy_cleanup");
		pcurl_slist_append = (fcurl_slist_append)GetProcAddress(hLibcurl, "curl_slist_append");
		pcurl_slist_free_all = (fcurl_slist_free_all)GetProcAddress(hLibcurl, "curl_slist_free_all");
		pcurl_formadd = (fcurl_formadd)GetProcAddress(hLibcurl, "curl_formadd");
		pcurl_global_init = (fcurl_global_init)GetProcAddress(hLibcurl, "curl_global_init");
		pcurl_global_cleanup = (fcurl_global_cleanup)GetProcAddress(hLibcurl, "curl_global_cleanup");
		pcurl_global_init(CURL_GLOBAL_ALL);
	}
}


size_t GetHttpData(void *ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t realsize = size * nmemb;

	//FILE* stream = (FILE*)userdata;
	//if (!stream) {
	//	printf("!!! No stream\n");
	//	return 0;
	//}
	//if (is_save_httpdata)
	//{
	//	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	//}
	//else
	//{
	//	//ZeroMemory(HttpBuffer,MAX_PATH * 1024);
	//	strcat((char*)userdata, (const char*)ptr);


	//}


	int len = strlen((const char*)userdata);
	char* start = (char*)userdata + len;
	memcpy_s(start, realsize, ptr, realsize);

	//const char* httpend = strstr((const char*)userdata, "\r\n0\r\n\r\n");
	//if (httpend)
	//{
	//	ZeroMemory((void*)httpend, 7);
	//}
	//strcat_s((char*)userdata, realsize, (const char*)ptr);
	//strcat((char*)userdata, (const char*)ptr);
	//fclose(stream);
	return realsize;
}

std::string  PostUrl(std::string PostData, std::string GetData)
{
	
	
	char buf[MAX_PATH*100] = { 0 };
	std::string Result = "";
	std::string PostUrl = "";
	if (GetData.find("http") == std::string::npos) {
		PostUrl = APIURL + GetData;
	}
	else {
		PostUrl = GetData;
	}
	
	CURL *curl;
	curl = pcurl_easy_init();

	CURLcode res_ret;
	struct curl_slist *headers = NULL;
	headers = pcurl_slist_append(headers, "Connection: keep-alive");
	headers = pcurl_slist_append(headers, "Accept: */*");
	headers = pcurl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
	headers = pcurl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");

	
	//pcurl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:8888");
	pcurl_easy_setopt(curl, CURLOPT_URL, PostUrl.c_str());
	pcurl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	//pcurl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	if (PostData != "")
		pcurl_easy_setopt(curl, CURLOPT_POSTFIELDS, PostData.c_str());
	//pcurl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
	//pcurl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	pcurl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
	pcurl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetHttpData);
	pcurl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	pcurl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	res_ret = pcurl_easy_perform(curl);
	pcurl_easy_cleanup(curl);
	pcurl_slist_free_all(headers);
	//EnterCriticalSection(&cs);
	Result = buf;
	//OutputDebugStringA(format("Message:%s---%s", GetData.c_str(), Result.c_str()).c_str());
	//LeaveCriticalSection(&cs);
	return Result;
}