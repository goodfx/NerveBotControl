#pragma once
std::string PostServer(std::string Method, std::string Param);
std::string SaveAccountInfoToServer(std::string Field, std::string Value);
std::string SaveDateToServer(std::string Table, std::string Values);
std::string GetAccountInfo(std::string Field);
std::string DeleteOrderList(std::string RoundHash);
std::string UpdataToServer(std::string Table, std::string Values);