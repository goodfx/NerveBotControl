#include "stdafx.h"
#include <string>
#include "StringConvert.h"
#include "json.hpp"
#include "Config.h"
using json = nlohmann::json;

void SetConfig(std::string Field,std::string Value) {
	std::string ConfigContent = GetFileString("config.json");
	json J = json::parse(ConfigContent);
	J[Field] = Value;
	ConfigContent = J.dump();
	FILE *FileObj = NULL;
	fopen_s(&FileObj, "config.json", "w+");
	fwrite(ConfigContent.c_str(), 1, ConfigContent.size(), FileObj);
	fclose(FileObj);
}
std::string GetConfig(std::string Field){
	std::string Result = "";
	std::string ConfigContent = GetFileString("config.json");
	json J ;
	try{
		J = json::parse(ConfigContent);
		Result = J[Field].get<std::string>();
	}
	catch (std::exception& e) {
		//MessageBoxA(NULL, e.what(), e.what(), MB_OK);
	}
	catch (...) {
		//AfxMessageBox(L"");
	}
	return Result;
}