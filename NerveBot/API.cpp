#include "stdafx.h"
#include <string>
#include "libcurl.h"
#include "StringConvert.h"
#include "API.h"
#include "json.hpp"
#include "Language.h"
#include "Config.h"
#include <curl/curl.h>
#include "JsonParse.h"
#include "Logs.h"

extern fcurl_global_init		pcurl_global_init;
extern fcurl_global_cleanup	pcurl_global_cleanup;


using json = nlohmann::json;
//std::string PublicServer = "https://public.nerve.network/api";
std::string PublicServer = "http://beta.public.nerve.network/api";

//std::string PublicAPI = "https://api.nerve.network/jsonrpc/";
std::string PublicAPI = "http://beta.api.nerve.network/jsonrpc/";

//std::string APIURL = "https://nervedex.com/";
std::string APIURL = "http://beta.nervedex.com/";

//int ChainId = 9;
int ChainId = 5;

//std::string MainPrefix = "NERVE";
std::string MainPrefix = "TNVT";

std::string UserPassword = "";//用户密码，用于解密私钥
std::string LibcurlPost(std::string PostData, std::string GetData) {
	std::string Result = "";
	
	Result = PostUrl(PostData, GetData);

	return Result;
}
// 获取所有交易对列表
std::string GetTradings() {
	std::string GetData = "coin/trading/config";
	return LibcurlPost("", GetData);
}
//获取地址所有资产余额和nonce值
std::string GetLedger(std::string Address, std::string Asset) {
	std::string GetData = "api/ledger/" + Address + "/" + Asset;
	return LibcurlPost("", GetData);
}

//获取指定订单详情
std::string GetOrderInfo(std::string OrderId) {
	std::string GetData = "api/order/" + OrderId;
	return LibcurlPost("", GetData);
}
//查询所有委托单列表
//Type 委托单类型 1：买单，2：卖单
//State 订单状态 0:全部，1：挂单中，2:部分成交、3:已成交 、4已撤销、5，部分成交已撤单。
std::string GetOrderList(std::string Address, std::string symbol, int Type, int State) {
	std::string GetData = "api/order/list";
	std::string PostData = "{\"address\":\""+ Address +"\",";
	PostData += "\"pageNumber\": 1,";
	PostData += "\"pageSize\": 30,";
	PostData += "\"beginTime\": 1586663355909,";
	PostData += "\"endTime\": 99999999999999,";
	PostData += format("\"type\": %d,",Type);
	PostData += "\"symbol\": \""+ symbol +"\",";
	PostData += format("\"state\": %d}", State);
	return LibcurlPost(PostData, GetData);

}
//获取指定交易对盘口信息
std::string GetOrderBook(std::string Symbol, std::string ItemNum) {
	std::string GetData = "api/orderBook/" + Symbol + "/" + ItemNum;
	return LibcurlPost("", GetData);
}
//获取交易对的config信息
std::string GetTradingConfig() {
	std::string GetData = "coin/trading/config";
	return LibcurlPost("", GetData);
}
//获取指定交易对的Hash
std::string GetTradingHash(std::string TradingName)
{
	std::string Result = "";
	std::string JsonStr = GetTradingConfig();
	json J;
	try {
		J = json::parse(JsonStr);
		for (auto&i : J["data"]["result"].items()) {
			for (auto&s : i.value()["tradingDtoList"].items()) {
				std::string FieldValue = s.value()["tradingName"].get<std::string>();
				//LogA("%s", FieldValue.c_str());
				if (s.value()["tradingName"].get<std::string>() == TradingName) {
					Result = s.value()["hash"].get<std::string>();
					return Result;
				}

			}
		}
	}
	catch (...) {
		std::string Caption = GetItemName("获取指定交易对的Hash异常");
		std::string Title = GetItemName("提示");
		MessageBoxA(NULL, Caption.c_str(), Title.c_str(), MB_OK);
	}
	return Result;
}
//通过Hash获取交易对的详细信息
nlohmann::json GetTradingInfo(std::string Hash) {
	json Result = "";
	std::string GetData = "coin/trading/get/" + Hash;
	std::string JsonStr = LibcurlPost("", GetData);
	json J;
	try {
		J = json::parse(JsonStr);
		Result = J["data"]["result"];
	}
	catch (...) {
		std::string Logs = "获取交易对详细信息异常：";
		Logs += "---" + GetData;
		Logs += "---" + JsonStr;
		LogsOut(Logs);
	}
	return Result;
}

//离线组装委托单交易 获取交易txHex
//Type 1:买单 2:卖单
std::string GetMameOrderHash(std::string Address, float Quantity, float Price, std::string Trading, int Type){
	std::string GetData = "api/order";
	std::string PostData = "{\"address\":\"" + Address + "\",";
	PostData += format("\"quantity\": %f,", Quantity);
	PostData += format("\"price\": %f,", Price);
	PostData += format("\"symbol\":\"%s\",", Trading.c_str());
	PostData += format("\"type\": %d}", Type);
	return LibcurlPost(PostData, GetData);
}
//离线组装删除订单交易
std::string GetCancelOrderHash(std::string Hash) {
	std::string GetData = "api/cancelOrder";
	std::string PostData = format("{\"orderId\":\"%s\"}", Hash.c_str());
	return LibcurlPost(PostData, GetData);
}
//广播交易
std::string Broadcast(std::string Hash) {
	std::string GetData = "api/broadcast";
	std::string PostData = format("{\"txHex\":\"%s\"}",Hash.c_str());
	return LibcurlPost(PostData, GetData);
}
//转账交易
std::string Transfer() {
	std::string  PostData = "";
	PostData += "{\"jsonrpc\" : \"2.0\",\"method\" : \"createTransferTxOffline\",";
	PostData += "\"params\" : [ [ {";
	PostData += format("\"address\" : \"TNVTdTSPGQc9eSCkUbnwVyJuBCaM4Zj2PVpvN\",\"assetChainId\" : 5,\"assetId\" : 1,\"amount\" : \"1000000000\",\"nonce\" : \"0000000000000000\"");
	PostData += "} ], [ {";
	PostData += format("\"address\" : \"TNVTdTSPGQc9eSCkUbnwVyJuBCaM4Zj2PVpvN\",\"assetChainId\" : 5,\"assetId\" : 1,\"amount\" : \"1000000000\",\"lockTime\" : 0");
	PostData += " } ], \"222\" ],\"id\" : 2530}";
	std::string JsonStr = LibcurlPost(PostData, PublicAPI);
	return JsonStr;
}
//获取资产相关信息,AssetsChainId,AssetsId,decimals
json getAssetInfo(std::string Asset) {
	json Return = "";
	std::string  PostData = "";
	PostData += "{\"jsonrpc\" : \"2.0\",\"method\" : \"getAssetList\",";
	PostData += format("\"params\" : [%d,0],\"id\" : 2530}",ChainId);
	std::string JsonStr = LibcurlPost(PostData, PublicServer);
	if (Asset == "") {
		try
		{
			Return = json::parse(JsonStr);
		}
		catch (...)
		{

		}
	}
	else {
		Return = GetJsonByField(JsonStr, "result", "symbol", Asset);
	}
	
	return Return;
}
//获取账户资产信息,available,freeze,nonce
json GetAccountLedgerInfo(std::string Asset) {
	json Return = "";
	std::string JsonStr = GetLedger(GetConfig("Address"), Asset);
	try
	{
		json J = json::parse(JsonStr);
		Return = J["data"];
	}
	catch (...)
	{

	}
	return Return;
}
//获取离线组装交易的hash
json CreateTransferTxOffline(std::string TransferStr) {
	json Return = "";
	std::string JsonStr = LibcurlPost(TransferStr, PublicAPI);
	try
	{
		Return = json::parse(JsonStr);

	}
	catch (...)
	{

	}
	return Return;
}
//JsonRPC
json JsonRPC(std::string Method,json Params) {
	json Return = "";
	json JsonPost;
	JsonPost["jsonrpc"] = "2.0";
	JsonPost["method"] = Method;
	JsonPost["params"] = Params;
	JsonPost["id"] = "2530";

	std::string JsonStr = LibcurlPost(JsonPost.dump(), PublicAPI);
	try
	{
		Return = json::parse(JsonStr);

	}
	catch (...)
	{

	}
	return Return;
}
//通过ChainId和assetId获取余额和nonce

