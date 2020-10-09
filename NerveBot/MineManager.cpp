#include "stdafx.h"
#include <string>
#include <vector>
#include "Language.h"
#include "StringConvert.h"
#include "json.hpp"
#include "Config.h"
#include "PostServer.h"
#include "MineManagerWnd.h"
#include "resource.h"
#include "MineManager.h"
#include "API.h"
#include "OpenSSLAES.h"
#include "JsonParse.h"
#include "NerveBotDlg.h"
#include "BigNumber.h"
using json = nlohmann::json;

extern int LoginState;//0为未登录，1为登录成功，3为退出登录
extern bool bMineManagerAccount;//识别当前登录的账户是否为矿池管理地址
extern MineManagerWnd *pMineManagerWnd;
extern std::string UserPassword ;//用户密码，用于解密私钥
extern int ChainId;
extern json GlobalJson;
extern CNerveBotDlg *MainWnd;
/*
GlobalJson["ID"]
GlobalJson["PrePrice"]
GlobalJson["NewPrice"]
GlobalJson["asset"]["asset01"]
GlobalJson["asset"]["asset02"]
*/
std::string Singture(std::string PrivateKey, std::string Hash);
void SetTabItemText(CString TabItemName) {
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	//tcItem.cchTextMax = TabItemName.GetLength();
	tcItem.pszText = TabItemName.GetBuffer();
	MainWnd->MainTable.SetItem(1, &tcItem);
	MainWnd->MyOrderWnd.ShowWindow(false);
	MainWnd->MyOrderWnd.ShowWindow(true);
}
unsigned int __stdcall IsMineManagerAccount(void* lp) {
	
	std::string Params = format("\"%s\",\"%s\"", GetConfig("Address").c_str(), GetConfig("Trading").c_str());
	std::string JsonStr = PostServer("GetMineManagerAccount", Params);
	json J;
	try
	{
		J = json::parse(JsonStr);
		if (J["success"].get<bool>() == true) {
			bMineManagerAccount = true;
		}
	}
	catch (...)
	{

	}
	return 0;
}
unsigned int __stdcall IniMineManager(void* lp) {
	while (1)
	{
		if (LoginState == 1 ) {
			break;
		}
		Sleep(50);
	}
	IsMineManagerAccount(NULL);
	if (bMineManagerAccount) {
		std::string Params = format("\"%s\",\"%s\"", GetConfig("Address").c_str(), GetConfig("Trading").c_str());
		std::string JsonStr = PostServer("GetMineManagerAccount", Params);
		json J;
		try
		{
			J = json::parse(JsonStr);
			if (J["success"].get<bool>() == true) {
				std::string LastRewardTime = "";
				if (J["data"]["last_reward_time"].is_null()) {
					LastRewardTime = GetItemName("尚未发放过奖励");
				}
				else {
					LastRewardTime = J["data"]["last_reward_time"].get<std::string>();
				}
				LastRewardTime = GetItemName("上次奖励发放时间：") + LastRewardTime;
				SetDlgItemTextA(pMineManagerWnd->m_hWnd, IDC_STATIC, LastRewardTime.c_str());
			}
		}
		catch (std::exception& e) {

			MessageBoxA(NULL, e.what(), e.what(), MB_OK);
		}
	}


	DWORD dwStyle = pMineManagerWnd->RewardList.GetExtendedStyle();//获取当前风格
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_DOUBLEBUFFER;
	dwStyle |= LVS_EX_CHECKBOXES;
	pMineManagerWnd->RewardList.SetExtendedStyle(dwStyle);
	pMineManagerWnd->RewardList.InsertColumn(0, GetItemNameW(L"地址"), LVCFMT_LEFT, 300);


	if (bMineManagerAccount) {
		//MainWnd->MainTable.SetDlgItemTextW(1, GetItemNameW(L"矿池管理"));
		SetTabItemText(GetItemNameW(L"矿池管理"));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_STATIC01, GetItemNameW(L"奖励资产："));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_STATIC2, GetItemNameW(L"奖励数量："));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_BUTTON2, GetItemNameW(L"发放奖励"));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_BUTTON1, GetItemNameW(L"获取奖励地址"));
		pMineManagerWnd->RewardList.InsertColumn(1, GetItemNameW(L"流动量"), LVCFMT_LEFT, 120);
		pMineManagerWnd->RewardList.InsertColumn(2, GetItemNameW(L"流动量"), LVCFMT_LEFT, 120);
		pMineManagerWnd->RewardList.InsertColumn(3, GetItemNameW(L"百分比"), LVCFMT_LEFT, 70);
		pMineManagerWnd->RewardList.InsertColumn(5, GetItemNameW(L"状态"), LVCFMT_LEFT, 100);
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_EDIT1, s2ws(GetConfig("RewardAmount")).c_str());
	}
	else {
		pMineManagerWnd->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		SetTabItemText(GetItemNameW(L"批量转账"));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_STATIC01, GetItemNameW(L"转账资产："));
		//SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_STATIC2, GetItemNameW(L"转账数量："));
		pMineManagerWnd->GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		pMineManagerWnd->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_BUTTON2, GetItemNameW(L"批量转账"));
		SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_BUTTON1, GetItemNameW(L"导入地址"));
		pMineManagerWnd->RewardList.InsertColumn(1, L"", LVCFMT_LEFT, 0);
		pMineManagerWnd->RewardList.InsertColumn(2, L"", LVCFMT_LEFT, 0);
		pMineManagerWnd->RewardList.InsertColumn(3, L"", LVCFMT_LEFT, 0);
		pMineManagerWnd->RewardList.InsertColumn(5, GetItemNameW(L"备注"), LVCFMT_LEFT, 180);
		pMineManagerWnd->RewardList.InsertColumn(6, GetItemNameW(L"状态"), LVCFMT_LEFT, 100);
	}
	pMineManagerWnd->RewardList.InsertColumn(4, GetItemNameW(L"奖励数量"), LVCFMT_LEFT, 100);
	
	
	
	SetDlgItemText(pMineManagerWnd->m_hWnd, IDC_BUTTON3, GetItemNameW(L"查看记录"));
	

	std::string RewardAmount = GetConfig("RewardAmount");
	std::string SlectedAsset = GetConfig("RewardAsset");
	std::string RewardAssetList = GetConfig("RewardAssetList");


	json AssetList = getAssetInfo("");
	if (AssetList != "") {
		try
		{
			int Index = 0;
			for (auto&i : AssetList["result"].items()) {
				pMineManagerWnd->AssetList.InsertString(Index, s2ws(i.value()["symbol"]).c_str());
				if (SlectedAsset == i.value()["symbol"])
					pMineManagerWnd->AssetList.SetCurSel(Index);
				Index++;
			}
		}
		catch (...)
		{

		}
	}
	else {
		pMineManagerWnd->AssetList.InsertString(0, GetItemNameW(L"获取资产列表异常"));
	}




	

	pMineManagerWnd->UpdateData();
	
	return 0;
}
void SaveToFile(std::string Hash,std::string Symbol,std::vector<int> Lists) {
	CListCtrl * List = &pMineManagerWnd->RewardList;
	CString AddressInfo = L"";
	for (int i = 0; i < Lists.size(); i++) {
		AddressInfo = AddressInfo + List->GetItemText(Lists[i], 0) + L"	";
		AddressInfo = AddressInfo + List->GetItemText(Lists[i], 4) + L"	";
		AddressInfo = AddressInfo + s2ws(Symbol).c_str() + L"	";
		AddressInfo = AddressInfo + s2ws(Hash).c_str() + +L"\r";
	}
	std::string TimeStr = TimeToString(time(NULL), "%d-%02d-%02d %02d-00-00");
	std::string SavePath = "record/" + TimeStr + ".txt";
	FILE* fp = NULL;
	std::string FileContent = ws2s(AddressInfo.GetBuffer());
	fopen_s(&fp, SavePath.c_str(), "a");
	fwrite(FileContent.c_str(), 1, FileContent.size(), fp);
	if(fp!=NULL)
		fclose(fp);
}
void SetListStatus(CString Status,std::vector<int> Lists) {
	CListCtrl * List = &pMineManagerWnd->RewardList;
	for (int i = 0; i < Lists.size(); i++) {
		if (bMineManagerAccount) {
			List->SetItemText(Lists[i], 5, Status);
		}
		else {
			List->SetItemText(Lists[i], 6, Status);
		}
	}
}
ASSET_ADDED GetTotalAssetAdded(std::string JsonStr) {
	ASSET_ADDED Return;
	Return.base_asset_added = 0;
	Return.quote_asset_added = 0;
	Return.TotalAssetAdded = 0;
	Return.quote_base_asset_added = 0;
	json J;
	try
	{
		J = json::parse(JsonStr);
		if (J["success"].get<bool>() == true) {
			for (auto&i : J["data"].items()) {
				Return.base_asset_added += atof(i.value()["base_asset_added"].get<std::string>().c_str());
				//Return.quote_asset_added += atof(i.value()["quote_asset_added"].get<std::string>().c_str());
				Return.quote_base_asset_added += atof(i.value()["quote_base_asset_added"].get<std::string>().c_str());
				
			}
		}
	}
	catch (...)
	{

	}
	Return.TotalAssetAdded += Return.base_asset_added + Return.quote_base_asset_added;
	return Return;
}
unsigned int __stdcall GetRewardingList(void* lp) {	
	if (!bMineManagerAccount)
		return 0;
	GlobalJson["ID"].clear();
	pMineManagerWnd->RewardList.DeleteAllItems();
	pMineManagerWnd->UpdateData();
	pMineManagerWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	float TotalRewardAmount = atof(ws2s(pMineManagerWnd->RewardAmount.GetBuffer()).c_str());
	std::string Params = format("\"%s\"",GetConfig("Trading").c_str());
	std::string JsonStr = PostServer("GetRewardingAccountList", Params);
	OutputDebugStringA(JsonStr.c_str());
	ASSET_ADDED TotalAsset = GetTotalAssetAdded(JsonStr);
	pMineManagerWnd->GetDlgItem(IDC_STATIC1)->SetWindowTextW(s2ws(format("%s%f",GetItemName("总流动量：").c_str(), TotalAsset.TotalAssetAdded)).c_str());
	if (TotalAsset.quote_asset_added > 0 || TotalAsset.base_asset_added > 0) {
		json J;
		pMineManagerWnd->RewardList.DeleteAllItems();
		try
		{
			J = json::parse(JsonStr);
			if (J["success"].get<bool>() == true) {
				int Index = 0;
				for (auto&i : J["data"].items()) {
					std::vector<std::string> Symbol;
					StringSplitA(Symbol, i.value()["trading_symbol"].get<std::string>(), "-");
					std::string base_asset_added = i.value()["base_asset_added"].get<std::string>();
					//std::string BassePercent = format("%.2f%%", TotalAsset.base_asset_added / atof(base_asset_added.c_str()));

					

					std::string quote_asset_added = i.value()["quote_asset_added"].get<std::string>();//通过价格计算，得到的quote资产数量 
					std::string quote_base_asset_added = i.value()["quote_base_asset_added"].get<std::string>();//加入的base资产数量 
					//std::string QuotePercent = format("%02f%%", TotalAsset.quote_asset_added / atof(quote_asset_added.c_str()));

					std::string TotalPercent = format("%.2f%%", (atof(base_asset_added.c_str()) + atof(quote_base_asset_added.c_str()))/TotalAsset.TotalAssetAdded *100);
					std::string RewardAmount = format("%.2f", TotalRewardAmount*atof(TotalPercent.c_str())/100);
					base_asset_added += "(" + Symbol[0] + ")";
					quote_asset_added += +"(" + Symbol[1] + ")";


					GlobalJson["ID"].push_back(format("%s,%s", i.value()["ID"].get<std::string>().c_str(), RewardAmount.c_str()));

					int nRow = pMineManagerWnd->RewardList.InsertItem(0, L"");
					pMineManagerWnd->RewardList.SetItemText(nRow, 0, s2ws(i.value()["address"].get<std::string>()).c_str());
					pMineManagerWnd->RewardList.SetItemText(nRow, 1, s2ws(base_asset_added).c_str());
					pMineManagerWnd->RewardList.SetItemText(nRow, 2, s2ws(quote_asset_added).c_str());
					pMineManagerWnd->RewardList.SetItemText(nRow, 3, s2ws(TotalPercent).c_str());
					pMineManagerWnd->RewardList.SetItemText(nRow, 4, s2ws(RewardAmount).c_str());
					pMineManagerWnd->RewardList.SetCheck(nRow);
					Index++;
				}
			}
		}
		catch (...)
		{

		}
	}

	pMineManagerWnd->GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	return 0;
}
std::vector<int> GetTransferAccounts() {
	CListCtrl *List = &pMineManagerWnd->RewardList;
	std::vector<int> Lists;
	int PerNum = 10;
	int Num = 0;
	for (int i = 0; i < List->GetItemCount();i++) {
		if ( Num>PerNum-1)
			break;
		if (!List->GetCheck(i))
			continue;
		std::string  Status;
		if (bMineManagerAccount) {
			Status = ws2s(List->GetItemText(i, 5).GetBuffer());
		}
		else {
			Status = ws2s(List->GetItemText(i, 6).GetBuffer());
		}
		if (Status != GetItemName("交易完成")) {
			Lists.push_back(i);
			Num++;
		}	
	}
	return Lists;
}
unsigned int __stdcall TransfeThread(void* lp) {
	if (bMineManagerAccount)
		GlobalJson["PostID"].clear();//保存要发送到服务器的信息,先清空
	BigNumber BigInt;	
	CString AssetSymbol;
	pMineManagerWnd->AssetList.GetWindowTextW(AssetSymbol);
	CListCtrl *List = &pMineManagerWnd->RewardList;

	while (1) {
		std::vector<int> Lists = GetTransferAccounts();
		if (Lists.size() == 0)
			break;		
		pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
		SetListStatus(GetItemNameW(L"开始转账"),Lists);

		//获取AssetChainId,AssetId;

		json AssetInfo = getAssetInfo(ws2s(AssetSymbol.GetBuffer()));

		if (AssetInfo == "") {
			SetListStatus(GetItemNameW(L"获取资产信息异常"), Lists);
			AfxMessageBox(GetItemNameW(L"获取资产信息异常"));
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		json Params = { ChainId, ChainId, 1, GetConfig("Address") };
		json NonceNVT = JsonRPC("getAccountBalance", Params);
		json NonceAsset;
		if (AssetSymbol != L"NVT") {
			Params = { ChainId,AssetInfo["assetChainId"].get<int>(), AssetInfo["assetId"].get<int>(), GetConfig("Address") };
			NonceAsset = JsonRPC("getAccountBalance", Params);
		}
		else {
			NonceAsset = NonceNVT;
		}
		if (NonceNVT == "" || NonceAsset == "") {
			SetListStatus(GetItemNameW(L"获取Nonce异常"), Lists);
			AfxMessageBox(GetItemNameW(L"获取Nonce异常"));
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		if (NonceNVT["result"].is_null() || NonceAsset["result"].is_null()) {
			SetListStatus(GetItemNameW(L"获取Nonce异常"), Lists);
			AfxMessageBox(GetItemNameW(L"获取Nonce异常"));
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}

		SetListStatus(GetItemNameW(L"组装交易"), Lists);
		json FullJson;
		FullJson["jsonrpc"] = "2.0";
		FullJson["method"] = "createTransferTxOffline";
		if (bMineManagerAccount) {
			FullJson["params"] = { {},{},"Liquidity Reward" };
		}
		else {
			std::string Demo = ws2s(List->GetItemText(0, 5).GetBuffer()).c_str();
			FullJson["params"] = { {},{},G2U(Demo.c_str()) };
		}

		FullJson["id"] = "2530";

		int AssetChainId = AssetInfo["assetChainId"].get<int>();
		int AssetId = AssetInfo["assetId"].get<int>();
		int Decimal = AssetInfo["decimals"].get<int>();
		json Input, InputFees;
		///////////////////////////////////////////////////////////////////////////////////////组装交易
		ULONGLONG Fees = 0;
		std::string RealAmount = "";
		std::string TotalRealAmount = "0";



		for (int i = 0; i < Lists.size(); i++) {
			if (bMineManagerAccount)
				GlobalJson["PostID"].push_back(GlobalJson["ID"][Lists[i]]);//保存要发送到服务器的信息
			
			json Output;
			std::string Amount = ws2s(List->GetItemText(Lists[i], 4).GetBuffer());
			//RealAmount = pow(10, Decimal)* atof(Amount.c_str());
			RealAmount = BigInt.Pow(Amount, Decimal);
			TotalRealAmount = BigInt.Add(RealAmount, TotalRealAmount);
			Output["address"] = ws2s(List->GetItemText(Lists[i], 0).GetBuffer());
			Output["assetChainId"] = AssetChainId;
			Output["assetId"] = AssetId;
			Output["lockTime"] = 0;
			Output["amount"] = RealAmount;
			Fees += 100000;
			FullJson["params"][1].push_back(Output);
		}
		Input["address"] = GetConfig("Address");
		Input["assetChainId"] = AssetChainId;
		Input["assetId"] = AssetId;

		//如果要转账的资产不是NVT，则需要另外组装一个input用来计算转账需要的NVT手续费，
		if (AssetSymbol != L"NVT") {
			Input["amount"] = TotalRealAmount;
			Input["nonce"] = NonceAsset["result"]["nonce"];

			InputFees["address"] = GetConfig("Address");
			InputFees["assetChainId"] = ChainId;
			InputFees["assetId"] = 1;
			InputFees["nonce"] = NonceNVT["result"]["nonce"];
			InputFees["amount"] = format("%llu", Fees);;


		}
		else {
			Input["nonce"] = NonceNVT["result"]["nonce"];
			TotalRealAmount = BigInt.Add(TotalRealAmount, format("%d", Fees));
			Input["amount"] = TotalRealAmount;
		}
		FullJson["params"][0].push_back(Input);
		if (AssetSymbol != L"NVT")
			FullJson["params"][0].push_back(InputFees);
		///////////////////////////////////////////////////////////////////////////////////////返回离线组装交易hash
		std::string PostData = FullJson.dump();


		//pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);

		json TxOffline = CreateTransferTxOffline(PostData);
		if (TxOffline == "") {
			SetListStatus(GetItemNameW(L"组装交易异常"),Lists);
			AfxMessageBox(GetItemNameW(L"组装交易异常"));
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		if (!TxOffline["error"].is_null())
		{
			SetListStatus(GetItemNameW(L"组装交易异常"), Lists);
			std::string Msg = TxOffline["error"].dump();
			MessageBoxA(NULL, Msg.c_str(), GetItemName("提示").c_str(), MB_OK);
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		SetListStatus(GetItemNameW(L"广播交易"), Lists);
		///////////////////////////////////////////////////////////////////////////////////////广播交易
		std::string Hash = TxOffline["result"]["hash"];
		std::string TxHex = TxOffline["result"]["txHex"];
		TxHex = TxHex.substr(0, TxHex.size() - 2);
		std::string PriKey = AesDecrypt(GetConfig("EncPriKey"), UserPassword);
		std::string SingHash = Singture(PriKey, Hash);
		SingHash = TxHex + SingHash;
		//json Result = ParseJsonStr(Broadcast(SingHash));
		Params = { ChainId,SingHash.c_str() };
		json Result = JsonRPC("broadcastTx", Params);
		///////////////////////////////////////////////////////////////////////////////////////查询交易

		if (Result == "") {
			SetListStatus(GetItemNameW(L"广播交易失败"), Lists);
			AfxMessageBox(GetItemNameW(L"广播交易失败"));
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		if (Result["error"].is_null()) {
			Hash = Result["result"]["hash"].get<std::string>();
		}
		else
		{
			SetListStatus(GetItemNameW(L"广播交易失败"), Lists);
			std::string Msg = Result["error"].dump();
			if (Msg.find("CoinTo has duplicate account assets") != std::string::npos) {
				MessageBoxA(NULL, GetItemName("有重复账号，广播交易失败").c_str(), GetItemName("提示").c_str(), MB_OK);
			}
			else {
				MessageBoxA(NULL, Msg.c_str(), GetItemName("提示").c_str(), MB_OK);
			}
			
			pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			return 0;
		}
		//Sleep(5000);
		//Params = {5,Hash.c_str()};
		//JsonRPC("getTx", Params);
		SetListStatus(GetItemNameW(L"交易完成"), Lists);

		if (bMineManagerAccount) {
			std::string Ids = GlobalJson["PostID"].dump();
			Params = format("\"%s\",\"%s\",\"%s\",\"%s\",%s", GetConfig("Address").c_str(), GetConfig("Trading").c_str(), Hash.c_str(), ws2s(AssetSymbol.GetBuffer()).c_str(), Ids.c_str());
			PostServer("SetRewardingAccountStatus", Params);
		}
		SaveToFile(Hash, ws2s(AssetSymbol.GetBuffer()),Lists);


	}
	pMineManagerWnd->GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	return 0;
}