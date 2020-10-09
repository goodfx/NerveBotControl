#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
typedef struct {
	float Price;
	float Amount;
	std::string TxHex;
	CListCtrl *List;
	int Index;
	std::string OrderId;
	time_t CreateTime;
	std::string RoundHash;
	int Type;// 1：买单，2：卖单
	int Status;//0初始化 1表示离线组装完成 2表示broadcast完成 3表示查询完成

}OrderInfo;
typedef struct {
	CListCtrl *List;
	int Index;
	int Type;// 1：买单，2：卖单
	OrderInfo Order;
}MAKE_ORDER_INFO;

typedef struct {
	std::string TxHex;
	CListCtrl *Order;
	int Index;

}DO_ORDER_INFO;
std::vector<OrderInfo> ComputeOrderInfo(int OrderNum, float AssetAmount, float StartPrice, float PriceStep, float h);
std::vector<OrderInfo>  MakeTmpOrder(float h);
unsigned int __stdcall MakeOrderInfo(void* lp);
void DoOrder();
std::string GetOmitHash(std::string Hash);
std::string GetRoundOrderHash();
//初始发时，从服务器获取挂单信息
unsigned int __stdcall GetOrderListFromServer(void* lp);
std::string GetOrderState(int State);
