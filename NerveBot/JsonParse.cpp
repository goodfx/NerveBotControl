#include "stdafx.h"
#include <string>
#include "json.hpp"
#include "JsonParse.h"
using json = nlohmann::json;
std::string GetResultByLoopField(std::string JsonStr, std::string PreField, std::string JudgeField ,std::string JudgeValue , std::string ReturnField) {
	
	std::string Result = "";
	json J;
	try {
		J = json::parse(JsonStr);
		for (auto &i : J[PreField].items()) {			
			if (i.value()[JudgeField].get<std::string>() == JudgeValue) {
				if (i.value()[ReturnField].is_number()) {
					char buf[100] = { 0 };
					sprintf_s(buf, "%f", i.value()[ReturnField].get<float>());
					Result = buf;
				}
				else {
					Result = i.value()[ReturnField].get<std::string>();
				}
				
				break;
			}
		}		
	}
	catch (...) {

	}
	return Result;
}
json GetJsonByField(std::string JsonStr, std::string PreField,std::string CompField,std::string Field) {
	json Return = "";
	json J;
	try {
		J = json::parse(JsonStr);
		for (auto &i : J[PreField].items()) {
			if (i.value()[CompField].get<std::string>() == Field) {
				Return = i.value();
			}
		}
	}
	catch (...) {

	}
	return Return;
}
json ParseJsonStr(std::string JsonStr) {
	json Return = "";
	try {
		Return = json::parse(JsonStr);
	}
	catch (...) {

	}
	return Return;
}