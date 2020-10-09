// IniSettingWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "NerveBot.h"
#include "IniSettingWnd.h"
#include "afxdialogex.h"
#include "Stringconvert.h"
#include "NerveBotDlg.h"
#include "API.h"
#include "json.hpp"
#include "OpenSSLAES.h"
#include "Config.h"
#include "Language.h"
#include "GetAddress.h"
#include "PostServer.h"
using json = nlohmann::json;
extern int LoginState;
extern CNerveBotDlg *MainWnd;
extern std::string UserPassword;
extern  int ChainId;
extern  std::string MainPrefix;
extern int CurrentLanguageId;
std::string GetPubKey(std::string PrivateKey);

// IniSettingWnd 对话框

IMPLEMENT_DYNAMIC(IniSettingWnd, CDialogEx)

IniSettingWnd::IniSettingWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_Password(_T(""))
	, m_RePassword(_T(""))
	, m_PriKey(_T(""))
{

}

IniSettingWnd::~IniSettingWnd()
{
}

void IniSettingWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, Tradings);
	DDX_Control(pDX, IDC_EDIT1, PriKey);
	DDX_Control(pDX, IDC_EDIT6, Password);
	DDX_Control(pDX, IDC_EDIT7, RePassword);
	DDX_Text(pDX, IDC_EDIT6, m_Password);
	DDX_Text(pDX, IDC_EDIT7, m_RePassword);
	DDX_Text(pDX, IDC_EDIT1, m_PriKey);
	DDX_Control(pDX, IDC_COMBO3, LanguageList);
}


BEGIN_MESSAGE_MAP(IniSettingWnd, CDialogEx)
	ON_BN_CLICKED(IDOK, &IniSettingWnd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &IniSettingWnd::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_EN_KILLFOCUS(IDC_EDIT1, &IniSettingWnd::OnEnKillfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT1, &IniSettingWnd::OnEnSetfocusEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &IniSettingWnd::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// IniSettingWnd 消息处理程序
void IniSettingWnd::LoadLanguage() {
	CString mOk, mCancel,mPriKey, mPassword, mRePassword,mTradingSymbol, mTitle;
	mPriKey = GetItemNameW(L"私钥：");
	mPassword = GetItemNameW(L"密码：");
	mRePassword = GetItemNameW(L"确认密码：");
	mTradingSymbol = GetItemNameW(L"选择交易对：");

	mOk = GetItemNameW(L"确认");
	mCancel = GetItemNameW(L"取消");
	mTitle = GetItemNameW(L"设置");

	SetDlgItemText(IDC_STATIC1, mPriKey);
	SetDlgItemText(IDC_STATIC2, mPassword);
	SetDlgItemText(IDC_STATIC3, mRePassword);
	SetDlgItemText(IDC_STATIC4, mTradingSymbol);

	SetDlgItemText(IDOK, mOk);
	SetDlgItemText(IDCANCEL, mCancel);
	
	this->SetWindowTextW(mTitle);

}

BOOL IniSettingWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LanguageList.InsertString(0, L"English");
	LanguageList.InsertString(1, L"中文");
	std::string Lang = GetConfig("Language");
	if (Lang == "" || Lang == "0") {
		CurrentLanguageId = 0;
		LanguageList.SetCurSel(1);
	}
	else {
		CurrentLanguageId = 1;
		LanguageList.SetCurSel(0);
	}
	LoadLanguage();
	// TODO:  在此添加额外的初始化
	CenterWindow(GetDesktopWindow());
	std::string JsonStr = GetTradings();
	json J ;
	try {
		J = json::parse(JsonStr);
		if (J["msg"].get<std::string>() == "success") {
			int Index = 0;
			for (auto &i : J["data"]["result"].items()) {
				for(auto &t:i.value()["tradingDtoList"].items()){
					std::string Symbol = replace_str(t.value()["tradingName"].get<std::string>(),"/","-");
					Tradings.InsertString(Index, s2ws(Symbol).c_str());
					Index++;
				}
			}
			Tradings.SetCurSel(0);
		}
	}
	catch (...) {
		std::string Caption = GetItemName("网络异常，请重试");
		std::string Title = GetItemName("提示");
		MessageBoxA(NULL, Caption.c_str(), Title.c_str(), MB_OK);
		exit(-1);
		return TRUE;
	}
	
	
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

unsigned int __stdcall UpLoadAccountInfo(void* lp) {
	CString TradingName = s2ws(GetConfig("Trading")).c_str();
	TradingName.Replace(L"-", L"/");
	std::string TradingHash = GetTradingHash(ws2s(TradingName.GetBuffer()));

	SetConfig("TradingHash", TradingHash);
	std::string Param = format("\"%s\",\"%s\"", GetConfig("Address").c_str(), GetConfig("Trading").c_str());
	PostServer("CreateNewAccount", Param);
	LoginState = 1;
	return 0;
}

void IniSettingWnd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_PriKey == L"") {
		SetDlgItemText(IDC_STATIC_Notifaction, GetItemNameW(L"请输入私钥"));
		return;
	}
	if (m_Password == L"") {
		SetDlgItemText(IDC_STATIC_Notifaction, GetItemNameW(L"请输入密码"));
		return;
	}
	if (m_Password != m_RePassword) {
		SetDlgItemText(IDC_STATIC_Notifaction, GetItemNameW(L"两次输入的密码不一致"));
		return;
	}
	CString PassWord;
	GetDlgItemText(IDC_EDIT6, PassWord);
	//UpdateData();
	UserPassword = ws2s(PassWord.GetBuffer());
	std::string EncPriKey = AesEncrypt(ws2s(m_PriKey.GetBuffer()), ws2s(PassWord.GetBuffer()));
	SetConfig("EncPriKey", EncPriKey);
	SetConfig("PubKey", GetPubKey(ws2s(m_PriKey.GetBuffer())));
	CString TradingStr = L"";
	Tradings.GetWindowText(TradingStr);
	SetConfig("Trading", ws2s(TradingStr.GetBuffer()));
	SetConfig("Address", GetAddressBuyPubKey(ChainId, 1, GetConfig("PubKey"), MainPrefix));
	_beginthreadex(NULL, NULL, UpLoadAccountInfo, NULL, 0, 0);
	MainWnd->CenterWindow();	
	DestroyWindow();
	
}


void IniSettingWnd::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_CLOSE);
}


//void IniSettingWnd::OnDestroy()
//{
//	CDialogEx::OnDestroy();
//
//	// TODO: 在此处添加消息处理程序代码
//}


//void IniSettingWnd::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	
//	CDialogEx::OnClose();
//}


//void IniSettingWnd::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CDialogEx::OnShowWindow(bShow, nStatus);
//	
//	// TODO: 在此处添加消息处理程序代码
//}


//BOOL IniSettingWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//
//	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
//}


BOOL IniSettingWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_CLOSE)
	{
		exit(-1);
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}

CString PriKey = L"";
void IniSettingWnd::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码

	PriKey.SetPasswordChar('*');

}


void IniSettingWnd::OnEnSetfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	PriKey.SetPasswordChar('\0');
}


void IniSettingWnd::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Lang = L"";
	LanguageList.GetLBText(LanguageList.GetCurSel(), Lang);

	if (Lang == L"English") {
		SetConfig("Language", "1");
	}
	else {
		SetConfig("Language", "0");
	}
	WinExec("NerveBot.exe", SW_HIDE);
	exit(-1);
}
