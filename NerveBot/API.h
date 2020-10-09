#pragma once
#include "json.hpp"
using json = nlohmann::json;
// 获取所有交易对列表
std::string GetTradings();
//获取地址所有资产余额和nonce值
std::string GetLedger(std::string Address, std::string Asset);
//获取指定订单详情
std::string GetOrderInfo(std::string OrderId);
//查询所有委托单列表
//Type 委托单类型 1：买单，2：卖单
//State 订单状态 0:全部，1：挂单中，2:部分成交、3:已成交 、4已撤销、5，部分成交已撤单。
std::string GetOrderList(std::string Address, std::string symbol, int Type, int State);
//获取指定交易对盘口信息
std::string GetOrderBook(std::string Symbol, std::string ItemNum);
//获取指定交易对的Hash
std::string GetTradingHash(std::string TradingName);
//通过Hash获取交易对的详细信息
nlohmann::json GetTradingInfo(std::string Hash);
//离线组装委托单交易 获取交易txHex
//Type 1:买单 2:卖单
std::string GetMameOrderHash(std::string Address, float Quantity, float Price, std::string Trading, int Type);
//广播交易
std::string Broadcast(std::string Hash);
//测试转账交易
std::string Transfer();
//离线组装删除订单交易
std::string GetCancelOrderHash(std::string Hash);
//获取资产相关信息
json getAssetInfo(std::string Asset);
//获取账户资产信息,available,freeze,nonce
json GetAccountLedgerInfo(std::string Asset);
//获取离线组装交易的hash
json CreateTransferTxOffline(std::string TransferStr);
//JsonRPC
json JsonRPC(std::string Method, json Params);