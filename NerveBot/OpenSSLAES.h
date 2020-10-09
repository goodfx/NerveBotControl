#pragma once
#include <string>
std::string AesEncrypt(std::string input_string);
std::string AesEncrypt(std::string input_string, std::string key);
std::string AesDecrypt(std::string encode_string, std::string key);