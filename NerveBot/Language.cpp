#include "stdafx.h"
#include <string>
#include "StringConvert.h"
#include "json.hpp"
#include "Language.h"

using json = nlohmann::json;
int CurrentLanguageId = 0;
std::string GetItemName(std::string Field) {
	std::string sField = G2U(Field.c_str());
	std::string LanguageContent = replace_str(GetFileString("language.json"), "\n", "");
	LanguageContent = replace_str(LanguageContent, "\t", "");
	LanguageContent = replace_str(LanguageContent, "\r", "");
	json J;
	try {
		J = json::parse(LanguageContent);
	}
	catch (std::exception& e) {

		MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	}
	return U2G(J[sField][CurrentLanguageId].get<std::string>().c_str());
}
CString GetItemNameW(CString Field) {
	CString Return = L"";
	std::string sField = ws2s(Field.GetBuffer()).c_str();
	sField = G2U(sField.c_str());
	std::string LanguageContent = replace_str(GetFileString("language.json"),"\n","");
	LanguageContent = replace_str(LanguageContent, "\t", "");
	LanguageContent = replace_str(LanguageContent, "\r", "");
	json J;
	try {
		J = json::parse(LanguageContent);
		Return = s2ws(U2G(J[sField][CurrentLanguageId].get<std::string>().c_str())).c_str();
	}
	catch (std::exception& e) {
		
		MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	}
	return Return;
	
}
void IniSetItemName() {

}