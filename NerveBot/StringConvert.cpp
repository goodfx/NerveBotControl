#include "stdafx.h"
#include <string>
#include <vector>

#include <algorithm>
#include <fstream>
#include "StringConvert.h"

#pragma warning(disable:4996)
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
//GB2312到UTF-8的转换
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
//字符串分割函数
void StringSplitW(std::vector<std::wstring> &vlist, std::wstring &str, std::wstring pattern)
{
	std::wstring::size_type pos;

	//str+=pattern;//扩展字符串以方便操作
	int size = str.length();
	int OldPos = 0;
	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		//char buf[MAX_PATH]={0};
		//itoa(pos,buf,10);
		//MessageBoxA(NULL,buf,"xx",MB_OK);
		if (pos != std::wstring::npos)
		{

			std::wstring s = str.substr(OldPos, pos - OldPos);
			vlist.push_back(s);
			OldPos = pos + pattern.size();
			i = pos + pattern.size();
		}

	}
	std::wstring s = str.substr(OldPos);
	vlist.push_back(s);
}

//字符串分割函数
void StringSplitA(std::vector<std::string> &vlist, std::string &str, std::string pattern)
{
	std::string::size_type pos;

	//str+=pattern;//扩展字符串以方便操作
	int size = str.length();
	int OldPos = 0;
	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		//char buf[MAX_PATH]={0};
		//itoa(pos,buf,10);
		//MessageBoxA(NULL,buf,"xx",MB_OK);
		if (pos != std::string::npos)
		{

			std::string s = str.substr(OldPos, pos - OldPos);
			vlist.push_back(s);
			OldPos = pos + pattern.size();
			i = pos + pattern.size();
		}

	}
	std::string s = str.substr(OldPos);
	vlist.push_back(s);
}


std::string& replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
	{
		pos = str.find(to_replaced, pos);
		if (pos != std::string::npos)
			str.replace(pos, to_replaced.length(), newchars);
		else
			break;
	}
	return   str;
}



std::string format(const char *pszFmt, ...)
{
	std::string str;
	//va_list args;
	//va_start(args, pszFmt);
	//{
	//	int nLength = _vscprintf(pszFmt, args);
	//	nLength += 1;  //上面返回的长度是包含\0，这里加上
	//	std::vector<char> vectorChars(nLength);
	//	_vsnprintf(vectorChars.data(), nLength, pszFmt, args);
	//	str.assign(vectorChars.data());
	//}
	//va_end(args);
	if (NULL != pszFmt)
	{
		va_list marker = NULL;
		va_start(marker, pszFmt);                            //初始化变量参数 
		size_t nLength = _vscprintf(pszFmt, marker) + 1;    //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, pszFmt, marker);
		if (nWritten>0)
		{
			str = &vBuffer[0];
		}
		va_end(marker);                                    //重置变量参数
	}

	return str;
}



std::string GetFileString(std::string FilePath)
{
	std::ifstream t(FilePath, std::ifstream::in | std::ifstream::binary);
	std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
	return str;
}
std::wstring s2ws(const std::string &s)
{
	size_t i;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _source = s.c_str();
	size_t _dsize = s.size() + 1;
	wchar_t* _dest = new wchar_t[_dsize];
	wmemset(_dest, 0x0, _dsize);
	mbstowcs_s(&i, _dest, _dsize, _source, _dsize);
	std::wstring result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string ws2s(const std::wstring &ws)
{
	size_t i;
	std::string curLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const wchar_t* _source = ws.c_str();
	size_t _dsize = 2 * ws.size() + 1;
	char* _dest = new char[_dsize];
	memset(_dest, 0x0, _dsize);
	wcstombs_s(&i, _dest, _dsize, _source, _dsize);
	std::string result = _dest;
	delete[] _dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

time_t StringToDatetime(char *str)
{
	tm tm_;
	int year, month, day, hour, minute, second;
	sscanf(str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;

	time_t t_ = mktime(&tm_); //已经减了8个时区  
	return t_; //秒时间  
}
std::string TimeToString(time_t Time, std::string TimeFormat)
{
	struct tm *t = localtime(&Time);
	char StartTime[MAX_PATH] = { 0 };
	//sprintf_s(StartTime, TimeFormat.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour/*, t->tm_min, t->tm_sec*/);
	if (TimeFormat.find(":%") == std::string::npos)
	{
		sprintf_s(StartTime, TimeFormat.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour);
	}
	else
	{
		sprintf_s(StartTime, TimeFormat.c_str(), t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	}

	return std::string(StartTime);
}
std::wstring& trimW(std::wstring &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(L" "));
	s.erase(s.find_last_not_of(L" ") + 1);
	return s;
}
std::string& trimA(std::string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}