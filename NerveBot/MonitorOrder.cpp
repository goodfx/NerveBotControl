#include "stdafx.h"
#include <string>
#include <vector>
#include "resource.h"
#include "IniWndStatus.h"
#include "Language.h"
#include "Config.h"
#include "OrderWnd.h"
#include "NerveBotDlg.h"
#include "API.h"
#include "JsonParse.h"
#include "StringConvert.h"
#include "PostServer.h"
#include "json.hpp"
#include "OrderControl.h"
#include "MonitorOrder.h"
#include "OrderCancel.h"

using json = nlohmann::json;
extern json GlobalJson;
extern CNerveBotDlg *MainWnd;
extern OrderWnd *pOrderWnd;
//是否进行自动挂单的判断 
bool IsAutoMakeOrder = false;
//监控指定订单
unsigned int __stdcall MonitorAssignOrder(void *lp) {
	MONITOR_ORDER_INFO *Order = (MONITOR_ORDER_INFO*)lp;
	CString OrderId = Order->List->GetItemText(Order->Index, 0);
	std::string JsonStr = GetOrderInfo(ws2s(OrderId.GetBuffer()));
	json J;
	while (1) {
		try
		{
			J = json::parse(JsonStr);
			if (J["success"].get<bool>())
			{
				int State = J["data"]["status"].get<int>();
				Order->List->SetItemText(Order->Index, 1, s2ws(GetOrderState(State)).c_str());
				break;
			}
		}
		catch (...)
		{

		}
	}
	delete[] lp;
	return 0;
}
//监控订单状态
unsigned int __stdcall MonitorOrderState(void* lp) {
	CListCtrl *List = (CListCtrl *)lp;
	CString OrderStateWaitCheck = GetItemNameW(L"待检测");
	CString OrderStateChecking = GetItemNameW(L"检测中");
	CString OrderStateOrdering = GetItemNameW(L"挂单中");
	CString OrderStatePartDeal = GetItemNameW(L"部分成交");
	while (1) {
		//pOrderWnd->UpdateData();
		int WorkingTimes = 0;
		for (int i = 0; i < List->GetItemCount(); i++) {
			CString Status = List->GetItemText(i, 1);
			if (Status == L"")
				break;
			CString OrderId = List->GetItemText(i, 0);
			if (OrderId == L"...")
				continue;
			if (Status == OrderStateWaitCheck) {
				MONITOR_ORDER_INFO *Order = new MONITOR_ORDER_INFO;
				Order->List = List;
				Order->Index = i;
				_beginthreadex(NULL, 0, MonitorAssignOrder, Order, 0, 0);
				List->SetItemText(i, 1, OrderStateChecking);
			}
			else if (Status == OrderStateOrdering || Status == OrderStatePartDeal) {
				if (WorkingTimes >= 2)
					continue;
				std::string Result = GetOrderInfo(ws2s(OrderId.GetBuffer()));
				json J;
				try
				{
					J = json::parse(Result);
					if (J["success"].get<bool>())
					{
						int State = J["data"]["status"].get<int>();
						List->SetItemText(i, 1, s2ws(GetOrderState(State)).c_str());
					}
					WorkingTimes++;
				}
				catch (...)
				{

				}

			}
		}
		Sleep(4000);
	}
	return 0;
}
//初始化订单状态
unsigned int __stdcall IniOrderStatus(void* lp) {
	OrderInfo* Order = (OrderInfo*)lp;
	std::string OrderId = Order->OrderId;	
	json J;
	std::string Address = GetConfig("Address");
	std::string TradingSymbol = GetConfig("Trading");
	while (1) {
		std::string Result = GetOrderInfo(OrderId);
		try
		{
			J = json::parse(Result);
			if (J["success"].get<bool>())
			{
				std::string Hash = J["data"]["id"].get<std::string>();
				LONGLONG CreateTime = J["data"]["createTime"].get<LONGLONG>();
				int State = J["data"]["status"].get<int>();
				Order->List->SetItemText(Order->Index, 1, s2ws(GetOrderState(State)).c_str());
				Order->List->SetItemText(Order->Index, 0, s2ws(Hash).c_str());
				Order->List->SetItemText(Order->Index, 4, s2ws(TimeToString(CreateTime / 1000, "%d-%02d-%02d %02d:%02d:%02d")).c_str());
				std::string Params = format("\"address\":\"%s\",\"trading_symbol\":\"%s\",\"hash\":\"%s\",\"round_hash\":\"%s\",\"type\":%d,\"price\":%f,\"quantity\":%f,\"create_time\":%lld", Address.c_str(), TradingSymbol.c_str(), Hash.c_str(), Order->RoundHash.c_str(), Order->Type, Order->Price, Order->Amount, CreateTime);
				SaveDateToServer("order_list", Params);
				break;
			}
			else {
				//std::string Msg = J["msg"].get<std::string>();
				//Order->List->SetItemText(Order->Index, 1, s2ws(U2G(Msg.c_str())).c_str());
			}

		}
		catch (...)
		{

		}
		Sleep(2000);
	}
	delete[] lp;
	return 0;
}
//根据振幅变动，自动处理挂单逻辑
unsigned int __stdcall AutoMakeOrder(void* lp) {
	float MakeOrderAmplitude = atof(GetConfig("Amplitude").c_str());
	while (true)
	{
		if (IsAutoMakeOrder) {
			try
			{
				float Amplitude = GlobalJson["Amplitude"].get<float>();
				if(Amplitude>MakeOrderAmplitude){
					HANDLE ThreadId = (HANDLE)_beginthreadex(NULL, 0, CancelOrders, NULL, 0, 0);
					WaitForSingleObject(ThreadId, INFINITE);
					ThreadId = (HANDLE)_beginthreadex(NULL, 0, MakeOrderInfo, NULL, 0, 0);
					WaitForSingleObject(ThreadId, INFINITE);
				}
			}
			catch (...)
			{

			}
		}
		Sleep(1000);
	}
	return 0;
}