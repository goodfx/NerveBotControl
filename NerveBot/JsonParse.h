#pragma once

using json = nlohmann::json;
std::string GetResultByLoopField(std::string JsonStr, std::string PreField, std::string JudgeField, std::string JudgeValue, std::string ReturnField);
json GetJsonByField(std::string JsonStr, std::string PreField, std::string CompField, std::string Field);
json ParseJsonStr(std::string JsonStr);