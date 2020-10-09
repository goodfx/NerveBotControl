#pragma once
#include <string>

// LoginWnd 对话框

class LoginWnd : public CDialogEx
{
	DECLARE_DYNAMIC(LoginWnd)

public:
	LoginWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LoginWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	void LoadLanguage();
	afx_msg void OnCbnSelchangeCombo2();
	CComboBox LanguageList;
	CComboBox Tradings;
	int CurIndex = 0;
	std::string JsonStr;
//	afx_msg void OnCbnEditchangeCombo3();
	afx_msg void OnCbnSelchangeCombo3();
};
