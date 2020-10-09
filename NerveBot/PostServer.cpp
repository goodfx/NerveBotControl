#include "stdafx.h"
#include <string>
#include <mutex>
#include "libcurl.h"
#include "json.hpp"
#include "PostServer.h"
#include "StringConvert.h"
#include "Config.h"
#include <curl/curl.h>

extern fcurl_global_init		pcurl_global_init;
extern fcurl_global_cleanup	pcurl_global_cleanup;
std::mutex mtx;
using json = nlohmann::json;
std::string PostServer(std::string Method,std::string Params) {
	
	std::string Result = "";
	std::string PostData = format("{\"method\":\"%s\",\"params\":[%s]}",Method.c_str(),Params.c_str());
	
	//mtx.lock();
	//pcurl_global_init(CURL_GLOBAL_ALL);
	Result = PostUrl(PostData, "https://json.nuls.gold:88/nerve_bot/api/");
	//pcurl_global_cleanup();
	//mtx.unlock();

	return Result;
}

std::string SaveAccountInfoToServer(std::string Field, std::string Value) {
	return PostServer("SaveAccountInfoToServer", format("\"%s\",\"%s\",\"%s\",\"%s\"", GetConfig("Address").c_str(), GetConfig("Trading").c_str(),Field.c_str(),Value.c_str()));
}

std::string SaveDateToServer(std::string Table, std::string Values) {
	return PostServer("SaveDateToServer", format("\"%s\",{%s}", Table.c_str(), Values.c_str()));
}
std::string UpdataToServer(std::string Table, std::string Values) {
	return PostServer("UpdataToServer", format("\"%s\",\"%s\",\"%s\",{%s}", Table.c_str(), GetConfig("Address").c_str(), GetConfig("Trading").c_str(), Values.c_str()));
}

std::string GetAccountInfo(std::string Field) {
	std::string Result = "";
	std::string Params = format("\"%s\",\"%s\"", GetConfig("Address").c_str(), GetConfig("Trading").c_str());
	std::string JsonStr = PostServer("GetAccountInfo", Params);
	json J;
	try {
		J = json::parse(JsonStr);
		if (J["success"].get<std::string>() == "true") {
			if (J["data"][Field].is_number()) {
				Result = format("%f", J["data"][Field].get<float>());
			}
			else {
				Result = J["data"][Field].get<std::string>().c_str();
			}
		}
	}
	catch (...) {

	}
	return Result;
}
std::string DeleteOrderList(std::string RoundHash) {
	std::string Result = "";
	std::string Params = format("\"%s\"", RoundHash.c_str());
	Result = PostServer("DeleteOrderList", Params);
	return Result;
}
//std::string GetDateFromServer(std::string RoundHash) {
//	return	PostServer("GetDateFromServer", format("\"%s\"", RoundHash.c_str()));
//}