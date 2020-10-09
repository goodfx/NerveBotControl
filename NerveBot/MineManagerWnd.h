#pragma once


// MineManagerWnd 对话框

class MineManagerWnd : public CDialogEx
{
	DECLARE_DYNAMIC(MineManagerWnd)

public:
	MineManagerWnd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MineManagerWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void LoadLanguage();
	CListCtrl RewardList;
	CComboBox AssetList;
	CString RewardAmount;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnCbnSelchangeCombo1();
};
