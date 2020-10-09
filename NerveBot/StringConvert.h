#pragma once
#include <vector>
char* U2G(const char* utf8);
char* G2U(const char* gb2312);
//×Ö·û´®·Ö¸îº¯Êý
void StringSplitW(std::vector<std::wstring> &vlist, std::wstring &str, std::wstring pattern);
//×Ö·û´®·Ö¸îº¯Êý
void StringSplitA(std::vector<std::string> &vlist, std::string &str, std::string pattern);
std::string& replace_str(std::string& str, const std::string& to_replaced, const std::string& newchars);
std::string format(const char *pszFmt, ...);
std::string GetFileString(std::string FilePath);
std::string GetFileString(std::string FilePath);
std::wstring s2ws(const std::string &s);
std::string ws2s(const std::wstring &ws);
time_t StringToDatetime(char *str);
std::string& trimA(std::string &s);
std::wstring& trimW(std::wstring &s);
std::string TimeToString(time_t Time, std::string TimeFormat);