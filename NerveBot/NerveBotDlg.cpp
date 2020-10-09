
// NerveBotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NerveBot.h"
#include "NerveBotDlg.h"
#include "afxdialogex.h"
#include "Login.h"
#include "IniSettingWnd.h"
#include "MineManagerWnd.h"
#include "IniWndStatus.h"
#include "libcurl.h"
#include "Language.h"
#include "Config.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern int LoginState;
extern int CurrentLanguageId;
CNerveBotDlg *MainWnd = NULL;
bool bMineManagerAccount = false;//识别当前登录的账户是否为矿池管理地址
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNerveBotDlg 对话框



CNerveBotDlg::CNerveBotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NERVEBOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CNerveBotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, MainTable);
	DDX_Control(pDX, IDC_COMBO2, LanguageList);
}

BEGIN_MESSAGE_MAP(CNerveBotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CNerveBotDlg::OnTcnSelchangeTab1)
//	ON_CBN_SELCHANGE(IDC_COMBO2, &CNerveBotDlg::OnCbnSelchangeCombo2)
	ON_CBN_EDITCHANGE(IDC_COMBO2, &CNerveBotDlg::OnCbnEditchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CNerveBotDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CNerveBotDlg 消息处理程序

BOOL CNerveBotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	IniLibCurl();
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


	ShowLoginWnd(NULL);
	::SetWindowPos(this->m_hWnd, HWND_TOP, 5000, 0, 0, 0, SWP_NOSIZE);
	MainWnd = this;
	MainTable.InsertItem(0, GetItemNameW(L"当前挂单"));
	MainTable.InsertItem(1, L"");
	MainTable.SetDlgItemTextW(1, L"02126564");
	
	CRect Ret;
	MainTable.GetClientRect(&Ret);
	Ret.top += 22;
	Ret.bottom -= 2;
	Ret.left += 1;
	Ret.right -= 4;
	MyOrderWnd.Create(IDD_DIALOG1, &MainTable);
	MyOrderWnd.MoveWindow(&Ret);
	MyOrderWnd.ShowWindow(true);

	MyMineManagerWnd.Create(IDD_DIALOG4, &MainTable);
	MyMineManagerWnd.MoveWindow(&Ret);
	MyMineManagerWnd.ShowWindow(false);


	_beginthreadex(NULL, NULL, IniWndStatus, NULL, 0, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNerveBotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNerveBotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNerveBotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int TimesOfPaint = 0;
LRESULT CNerveBotDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类


	

	return CDialogEx::DefWindowProc(message, wParam, lParam);
	
}


BOOL CNerveBotDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	//if (message == WM_SHOWWINDOW) {
	//	AfxMessageBox(L"ss");
	//	return TRUE;
	//	TimesOfPaint++;
	//}
	//else
	//{
	//	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
	//}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CNerveBotDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int cur_sel = MainTable.GetCurSel();
	switch (cur_sel)
	{
	case 0:
		MyOrderWnd.ShowWindow(true);
		MyMineManagerWnd.ShowWindow(false);


		break;
	case 1:
		//if (bMineManagerAccount == false) {
		//	AfxMessageBox(GetItemNameW(L"非矿池管理地址！"));
		//	break;
		//}
		MyOrderWnd.ShowWindow(false);
		MyMineManagerWnd.ShowWindow(true);

		break;

	default:
		break;
	}
	*pResult = 0;
}


//void CNerveBotDlg::OnCbnSelchangeCombo2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString Lang = L"";
//	LanguageList.GetLBText(LanguageList.GetCurSel(), Lang);
//
//	if (Lang == L"English") {
//		SetConfig("Language", "1");
//	}
//	else {
//		SetConfig("Language", "0");
//	}
//	WinExec("NerveBot.exe", SW_HIDE);
//	exit(-1);
//}


void CNerveBotDlg::OnCbnEditchangeCombo2()
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


void CNerveBotDlg::OnCbnSelchangeCombo2()
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
