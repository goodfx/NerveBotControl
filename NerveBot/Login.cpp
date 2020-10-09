#include "stdafx.h"
#include "resource.h"
#include "IniSettingWnd.h"
#include "Login.h"
#include "LoginWnd.h"
#include "Config.h"
IniSettingWnd *IniSetting = new IniSettingWnd();
LoginWnd *Login = new LoginWnd();
int LoginState = 0;//0为未登录，1为登录成功，3为退出登录
unsigned int __stdcall ShowLoginWnd(void* lp) {
	
	if (GetConfig("EncPriKey") == "") {
		IniSetting->Create(IDD_DIALOG2);
		IniSetting->ShowWindow(SW_SHOW);
		IniSetting->UpdateWindow();
	}
	else {
		Login->Create(IDD_DIALOG3);
		Login->ShowWindow(SW_SHOW);
		Login->UpdateWindow();
	}

	return 0;
}