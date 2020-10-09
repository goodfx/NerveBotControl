#pragma once
#include <string>
class BigNumber {
public:
	int Length;
	char NumberBuffer[1024] = { 0 };
	int Length1;
	char NumberBuffer1[1024] = { 0 };
	std::string Pow(std::string Number, int Decimal);
	int StringToMem(std::string Number, int Index);
	int GetDecimalPos(int Len, int Index);
	void MoveDecimalPos(int StartPos, int Step, int Index);
	std::string GetBigNumber();
	std::string Add(std::string Add1, std::string Add2);
};