
// NerveBotDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "OrderWnd.h"
#include "MineManagerWnd.h"

// CNerveBotDlg 对话框
class CNerveBotDlg : public CDialogEx
{
// 构造
public:
	CNerveBotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NERVEBOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl MainTable;
	OrderWnd MyOrderWnd;
	MineManagerWnd MyMineManagerWnd;
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox LanguageList;
//	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnEditchangeCombo2();
	afx_msg void OnCbnSelchangeCombo2();
};
