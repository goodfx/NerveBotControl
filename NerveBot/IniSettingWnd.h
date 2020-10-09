#pragma once
#include "afxwin.h"


// IniSettingWnd 对话框

class IniSettingWnd : public CDialogEx
{
	DECLARE_DYNAMIC(IniSettingWnd)

public:
	IniSettingWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~IniSettingWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	afx_msg void OnDestroy();
//	afx_msg void OnClose();
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CComboBox Tradings;
	afx_msg void OnEnKillfocusEdit1();
	CEdit PriKey;
	afx_msg void OnEnSetfocusEdit1();
	CEdit Password;
	CEdit RePassword;
	CString m_Password;
	CString m_RePassword;
	CString m_PriKey;
	void LoadLanguage();
	CComboBox LanguageList;
	afx_msg void OnCbnSelchangeCombo3();
};
