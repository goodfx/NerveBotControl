// MineManagerWnd.cpp: 实现文件
//
#include "stdafx.h"
#include "NerveBot.h"
#include "MineManagerWnd.h"
#include "afxdialogex.h"
#include "language.h"
#include "MineManager.h"
#include "StringConvert.h"
#include "Config.h"

// MineManagerWnd 对话框
extern bool bMineManagerAccount;//识别当前登录的账户是否为矿池管理地址
IMPLEMENT_DYNAMIC(MineManagerWnd, CDialogEx)
MineManagerWnd * pMineManagerWnd = NULL;
MineManagerWnd::MineManagerWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, RewardAmount(_T(""))
{

}

MineManagerWnd::~MineManagerWnd()
{
}

void MineManagerWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, RewardList);
	DDX_Control(pDX, IDC_COMBO1, AssetList);
	DDX_Text(pDX, IDC_EDIT1, RewardAmount);
}


BEGIN_MESSAGE_MAP(MineManagerWnd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MineManagerWnd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MineManagerWnd::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MineManagerWnd::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &MineManagerWnd::OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &MineManagerWnd::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// MineManagerWnd 消息处理程序

void MineManagerWnd::LoadLanguage() {
	
}
BOOL MineManagerWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pMineManagerWnd = this;

	_beginthreadex(NULL, 0, IniMineManager, NULL, 0, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void MineManagerWnd::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (bMineManagerAccount) {
		_beginthreadex(NULL, 0, GetRewardingList, NULL, 0, 0);
	}
	else
	{
		AfxMessageBox(GetItemNameW(L"批量转账文件格式为   地址|数量|备注  每行一条记录"));
		TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
		// 构造打开文件对话框   
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
		CString strFilePath;

		// 显示打开文件对话框   
		if (IDOK == fileDlg.DoModal())
		{
			// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
			strFilePath = fileDlg.GetPathName();

		}
		std::wstring FileContent = s2ws(GetFileString(ws2s(strFilePath.GetBuffer())));

		RewardList.DeleteAllItems();
		CString Address;

		std::vector<std::wstring> ImportAddressList;
		StringSplitW(ImportAddressList, FileContent, L"\n");
		for (int i = 0; i < ImportAddressList.size(); i++)
		{

			std::vector<std::wstring> ImportAddressInfo;
			StringSplitW(ImportAddressInfo, ImportAddressList[i], L"|");
			std::wstring Address = ImportAddressInfo[0];

			//WCHAR Number[MAX_PATH] = { 0 };
			//::GetDlgItemTextW(g_Wnd->m_hWnd, IDC_EDIT5, Number, MAX_PATH);
			std::wstring TransferNumber = L"";

			//WCHAR Demo[MAX_PATH] = { 0 };
			//::GetDlgItemTextW(g_Wnd->m_hWnd, IDC_EDIT6, Demo, MAX_PATH);
			std::wstring TransferDemo = L"";

			std::wstring LockTime = L"";

			if (ImportAddressInfo.size() >= 2)
			{
				TransferNumber = ImportAddressInfo[1];
			}
			if (ImportAddressInfo.size() >= 3)
			{
				TransferDemo = ImportAddressInfo[2];
			}
			if (ImportAddressInfo.size() >= 4)
			{
				LockTime = ImportAddressInfo[3];
			}


			int cRow = RewardList.GetItemCount();
			int nRow = RewardList.InsertItem(cRow, L"");
			RewardList.SetItemText(nRow, 0, Address.c_str());
			RewardList.SetItemText(nRow, 4, TransferNumber.c_str());
			RewardList.SetItemText(nRow, 5, TransferDemo.c_str());
			RewardList.SetCheck(nRow);

		}
	}
	
}


void MineManagerWnd::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString AssetSymbol;
	AssetList.GetWindowTextW(AssetSymbol);
	std::string AlertStr = ws2s(AssetSymbol.GetBuffer()) + " " +  GetItemName("资产即将转入到所列出账号，确认吗？");
	int Type = MessageBoxA(NULL, AlertStr.c_str(), GetItemName("确认转账").c_str(), MB_YESNO);
	if (Type != 7) {
		_beginthreadex(NULL, 0, TransfeThread, NULL, 0, 0);
	}
	
	
}


void MineManagerWnd::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), L"record", NULL, NULL, SW_SHOWNORMAL);
}


void MineManagerWnd::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString RewardAmount = L"";
	GetDlgItemText(IDC_EDIT1, RewardAmount);
	SetConfig("RewardAmount", ws2s(RewardAmount.GetBuffer()));
}


void MineManagerWnd::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Asset = L"";
	AssetList.GetLBText(AssetList.GetCurSel(), Asset);
	SetConfig("RewardAsset", ws2s(Asset.GetBuffer()));
}
