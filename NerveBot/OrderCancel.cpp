#include "stdafx.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include "resource.h"
#include "Config.h"
#include "OrderControl.h"
#include "json.hpp"
#include "StringConvert.h"
#include "OrderWnd.h"
#include "Language.h"
#include "OpenSSLAES.h"
#include "sha256.h"
#include "API.h"
#include "PostServer.h"
#include "IniWndStatus.h"
#include "OrderCancel.h"
#include "Logs.h"
using json = nlohmann::json;
extern json GlobalJson;
extern OrderWnd *pOrderWnd;
extern std::string UserPassword;
std::vector<CANCEL_ORDER_INFO> CancelOrderList;
std::string Singture(std::string PrivateKey, std::string Hash);
std::string hex2string(const std::string &strHex);
void GetCancelOrders(CListCtrl *List) {
	CANCEL_ORDER_INFO Tmp;
	for (int i = 0; i < List->GetItemCount(); i++) {
		Tmp.OrderId = ws2s(List->GetItemText(i, 0).GetBuffer());
		Tmp.List = List;
		Tmp.Index = i;
		Tmp.Status = 1;
		if (GetItemNameW(L"已成交") == List->GetItemText(i, 1) || GetItemNameW(L"已撤销") == List->GetItemText(i, 1))
			Tmp.Status = 0;
		CancelOrderList.push_back(Tmp);
	}

}

unsigned int __stdcall CancelOrders(void*lp) {
	pOrderWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(0);
	CancelOrderList.swap(std::vector<CANCEL_ORDER_INFO>());
	//获取要删除的订单
	GetCancelOrders(&pOrderWnd->BuyList);
	GetCancelOrders(&pOrderWnd->SellList);

	std::string Address = GetConfig("Address");
	std::string Trading = replace_str(GetConfig("Trading"), "-", "");
	std::string PriKey = AesDecrypt(GetConfig("EncPriKey"), UserPassword);

	
	while (1)
	{
		bool bExit = true;
		for (int i = 0; i < CancelOrderList.size(); i++) {
			CANCEL_ORDER_INFO CancelOrder = CancelOrderList[i];
			std::string OrderId = CancelOrder.OrderId;
			if (CancelOrder.Status == 0) {
				bExit = false;
				CancelOrder.List->SetItemText(CancelOrder.Index,1,GetItemNameW(L"已撤销"));
				CancelOrderList[i].Status = 2;
			}
			else if (CancelOrder.Status == 1) {
				bExit = false;
				std::string JsonStr = GetCancelOrderHash(OrderId);
				json J;
				try
				{
					J = json::parse(JsonStr);
					std::string Msg = J["msg"].get<std::string>();
					if (J["success"].get<bool>())
					{
						std::string TxHex = J["data"].get<std::string>();
						TxHex = TxHex.substr(0, TxHex.size() - 2);
						SHA256 sha256;
						std::string Hash = sha256(hex2string(sha256(hex2string(TxHex))));
						std::string SingHash = Singture(PriKey, Hash);
						SingHash = TxHex + SingHash;
						std::string JsonStr = Broadcast(SingHash);
						J = json::parse(JsonStr);
						if (J["success"].get<bool>())
						{
							CancelOrder.List->SetItemText(CancelOrder.Index, 1, GetItemNameW(L"已撤销"));
							CancelOrderList[i].Status = 2;
						}
						else if(J["msg"].get<std::string>().find("because status is")!=std::string::npos){
							CancelOrderList[i].Status = 2;
						}
					}
					else if (Msg.find("because status is") != std::string::npos || Msg.find("orderId can't be null") != std::string::npos) {
						CancelOrderList[i].Status = 2;
					}

				}
				catch (...)
				{
					std::string Logs = "撤单异常：" + JsonStr;
					LogsOut(Logs);
				}
			}
		}
		if (bExit)
		{
			pOrderWnd->BuyList.DeleteAllItems();
			pOrderWnd->SellList.DeleteAllItems();
			pOrderWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(1);
			DeleteOrderList(GetAccountInfo("round_hash"));//从服务器上删除订单信息
			return 0;
		}
		Sleep(3000);
	}
	pOrderWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(1);
	return 0;
}