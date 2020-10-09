// OrderWnd.cpp : 实现文件
//

#include "stdafx.h"
#include <vector>
#include "NerveBot.h"
#include "OrderWnd.h"
#include "afxdialogex.h"
#include "OpenSSLAES.h"
#include "StringConvert.h"
#include "Language.h"
#include "Config.h"
#include "Login.h"
#include "OrderControl.h"
#include "json.hpp"
#include "sha256.h"
#include "API.h"
#include "libcurl.h"
#include "IniWndStatus.h"
#include "OrderCancel.h"
using json = nlohmann::json;
extern json GlobalJson;
extern std::vector<DO_ORDER_INFO> DoOrderInfo;
//是否进行自动挂单的判断 
extern bool IsAutoMakeOrder;
// OrderWnd 对话框

IMPLEMENT_DYNAMIC(OrderWnd, CDialogEx)
OrderWnd *pOrderWnd = NULL;
OrderWnd::OrderWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

OrderWnd::~OrderWnd()
{
}

void OrderWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, BuyList);
	DDX_Control(pDX, IDC_LIST3, SellList);

}


BEGIN_MESSAGE_MAP(OrderWnd, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON3, &OrderWnd::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &OrderWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &OrderWnd::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &OrderWnd::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT4, &OrderWnd::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT2, &OrderWnd::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &OrderWnd::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON4, &OrderWnd::OnBnClickedButton4)
END_MESSAGE_MAP()


// OrderWnd 消息处理程序


BOOL OrderWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = BuyList.GetExtendedStyle();//获取当前风格
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_DOUBLEBUFFER;
	BuyList.SetExtendedStyle(dwStyle);
	BuyList.InsertColumn(0, L"Hash", LVCFMT_LEFT, 120);
	BuyList.InsertColumn(1, GetItemNameW(L"状态"), LVCFMT_LEFT, 70);
	BuyList.InsertColumn(2, GetItemNameW(L"数量"), LVCFMT_LEFT, 80);
	BuyList.InsertColumn(3, GetItemNameW(L"价格"), LVCFMT_LEFT, 70);
	BuyList.InsertColumn(4, GetItemNameW(L"时间"), LVCFMT_LEFT, 130);

	dwStyle = SellList.GetExtendedStyle();//获取当前风格
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_DOUBLEBUFFER;
	SellList.SetExtendedStyle(dwStyle);
	SellList.InsertColumn(0, L"Hash", LVCFMT_LEFT, 120);
	SellList.InsertColumn(1, GetItemNameW(L"状态"), LVCFMT_LEFT, 70);
	SellList.InsertColumn(2, GetItemNameW(L"数量"), LVCFMT_LEFT, 80);
	SellList.InsertColumn(3, GetItemNameW(L"价格"), LVCFMT_LEFT, 70);
	SellList.InsertColumn(4, GetItemNameW(L"时间"), LVCFMT_LEFT, 130);
	LoadLanguage();
	//unit.InsertString(0, L"%");
	//unit.InsertString(1, L"个");
	//unit.SetCurSel(0);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(0);
	pOrderWnd = this;
	_beginthreadex(NULL, 0, GetOrderListFromServer, NULL, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//void OrderWnd::OnBnClickedButton4()
//{
//	char PriKey[MAX_PATH] = {0};
//	GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, PriKey, MAX_PATH);
//	char Password[MAX_PATH] = { 0 };
//	GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, Password, MAX_PATH);
//	std::string AESString = AesEncrypt(PriKey, Password);
//	SetDlgItemTextA(this->m_hWnd, IDC_EDIT4, AESString.c_str());
//	// TODO: 在此添加控件通知处理程序代码
//}


//void OrderWnd::OnBnClickedButton6()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	char PriKey[MAX_PATH] = { 0 };
//	GetDlgItemTextA(this->m_hWnd, IDC_EDIT4, PriKey, MAX_PATH);
//	char Password[MAX_PATH] = { 0 };
//	GetDlgItemTextA(this->m_hWnd, IDC_EDIT5, Password, MAX_PATH);
//	std::string AESString = AesDecrypt(PriKey, Password);
//	SetDlgItemTextA(this->m_hWnd, IDC_EDIT4, AESString.c_str());
//}
void OrderWnd::LoadLanguage() {
	SetDlgItemText(IDC_STATIC1, GetItemNameW(L"买单列表："));
	SetDlgItemText(IDC_STATIC2, GetItemNameW(L"卖单列表："));
	SetDlgItemText(IDC_STATIC3, GetItemNameW(L"挂单数量："));
	SetDlgItemText(IDC_STATIC4, GetItemNameW(L"加入资金比例："));
	SetDlgItemText(IDC_STATIC5, GetItemNameW(L"挂单间隔百分比："));
	SetDlgItemText(IDC_STATIC6, GetItemNameW(L"重新挂单振幅："));
	SetDlgItemText(IDC_BUTTON3, GetItemNameW(L"保存"));
	SetDlgItemText(IDC_BUTTON2, GetItemNameW(L"挂单"));
	SetDlgItemText(IDC_BUTTON1, GetItemNameW(L"撤销所有订单"));
}

void OrderWnd::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Amount;
	GetDlgItemText(IDC_EDIT1, Amount);
	SetConfig("OrderNum", ws2s(Amount.GetBuffer()));
	CString DropInPercent;
	GetDlgItemText(IDC_EDIT4, DropInPercent);
	SetConfig("DropInPercent", ws2s(DropInPercent.GetBuffer()));
	CString Amplitude;
	GetDlgItemText(IDC_EDIT3, Amplitude);
	SetConfig("Amplitude", ws2s(Amplitude.GetBuffer()));
	CString OrderDistance;
	GetDlgItemText(IDC_EDIT2, OrderDistance);
	SetConfig("OrderDistance", ws2s(OrderDistance.GetBuffer()));
}

std::string hex2string(const std::string &strHex);
void OrderWnd::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//SHA256 sha256;
	//std::string hash = "020016f55c5f156e756c735f676f6c645f706f6f6c2072657761726400750117090001d897036b71275d92563eb2a7891796f751037bb809000100a08601000000000000000000000000000000000000000000000000000000000008a01a451fd92487410001000900010000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	//hash = hex2string(hash);
	//hash = sha256(hash);
	//hash = hex2string(hash);
	//hash = sha256(hash);
	//hash = sha256(hex2string(sha256(hex2string(hash))));
	//MakeTmpOrder(10);
	DoOrder();
	//GetOrderListFromServer(NULL);
	//_beginthreadex(NULL, 0, Broadcast, (void*)OrderNum, 0, 0);
	return;
}


void OrderWnd::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	_beginthreadex(NULL, 0, CancelOrders, NULL, 0, 0);
}


void OrderWnd::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
}


void OrderWnd::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
}


void OrderWnd::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
}


void OrderWnd::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON3)->EnableWindow(1);
}


void OrderWnd::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetConfig("AutoOrder") == "1") {
		SetConfig("AutoOrder", "0");
		SetDlgItemText(IDC_BUTTON4, GetItemNameW(L"开启自动挂单"));
		IsAutoMakeOrder = false;
	}
	else
	{
		SetConfig("AutoOrder", "1");
		SetDlgItemText(IDC_BUTTON4, GetItemNameW(L"关闭自动挂单"));
		IsAutoMakeOrder = true;
	}
}
