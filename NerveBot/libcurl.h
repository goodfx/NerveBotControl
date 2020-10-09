#pragma once
#include <string>
#include <curl/curl.h>
typedef  CURL* (*fcurl_easy_init)(void);
typedef CURLcode(*fcurl_easy_setopt)(CURL *curl, CURLoption option, ...);
typedef CURLcode(*fcurl_easy_perform)(CURL *curl);
typedef void(*fcurl_easy_cleanup)(CURL *curl);
typedef struct curl_slist *(*fcurl_slist_append)(struct curl_slist *, const char *);
typedef  void(*fcurl_slist_free_all)(struct curl_slist *);
typedef CURLFORMcode(*fcurl_formadd)(struct curl_httppost **httppost, struct curl_httppost **last_post, ...);
typedef CURLcode(*fcurl_global_init)(long flags);
typedef void (*fcurl_global_cleanup)(void);
void IniLibCurl();
std::string PostUrl(std::string PostData, std::string GetData );