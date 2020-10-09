#include "stdafx.h"
#include "BigNumber.h"
#include "StringConvert.h"
std::string hex2string(const std::string &strHex);
std::string string2hex(const std::uint8_t *data, int len);
int BigNumber::StringToMem(std::string Number,int Index) {
	char* Dst = NULL;
	if (Index == 1) {
		Dst = NumberBuffer;
	}
	else {
		Dst = NumberBuffer1;
	}
	for (int i = 0; i < Number.size(); i++) {
		int n;
		char a[2] = {0};
		memcpy(a, &Number[i], 1);
		if (a[0] == 0x2e) {
			n = 46;
		}
		else
		{
			n = atoi(a);
		}

		memcpy(Dst +i, &n, 1);
	}
	return Number.size();
}
int BigNumber::GetDecimalPos(int Len, int Index) {
	char* Dst = NULL;
	if (Index == 1) {
		Dst = NumberBuffer;
	}
	else {
		Dst = NumberBuffer1;
	}
	for (int i = 0; i < Len; i++) {
		if (Dst[i] == 0x2e) {
			return i;
		}
	}
	return 0;
}
void BigNumber::MoveDecimalPos(int StartPos, int Step,int Index) {
	char* Dst = NULL;
	if (Index == 1) {
		Dst = NumberBuffer;
	}
	else {
		Dst = NumberBuffer1;
	}
	for (int i = StartPos; i < StartPos + Step; i++) {
		Dst[i] = Dst[i + 1];
	}
	Dst[StartPos + Step] = 0x2e;
}
std::string BigNumber::Pow(std::string Number,int Decimal) {

	int Len = StringToMem(Number,1);
	int DecimalPos = GetDecimalPos(Len,1);
	int Decs = Len - 1 - DecimalPos;//小数位数
	//不包含小数的情况
	if (DecimalPos == 0) {
		Len = Len + Decimal;
	}
	//包含小数的情况
	else {
		//如果Decimal大于小数位数
		if (Decimal >= Decs) {
			Len = Len + (Decimal - Decs)-1;//没有小数后的数长，-1为减去原来小数点占用的一位
		}
		MoveDecimalPos(DecimalPos, Decimal,1);
	}
	Length = Len;
	
	std::string NumberStr = GetBigNumber();
	return NumberStr;
}
std::string BigNumber::GetBigNumber() {
	std::string Result = "";
	for (int i = 0; i < Length; i++) {
		int a = (int)NumberBuffer[i];
		if (a == 46) {
			Result += ".";
		}
		else {			
			if (i == Length - 1) {
				Result += _strrev((char*)format("%d", a).c_str());
			}
			else {
				Result += format("%d", a);
			}
			
		}

	}
	ZeroMemory(NumberBuffer, 1024);
	ZeroMemory(NumberBuffer1, 1024);
	return Result;
}

std::string BigNumber::Add(std::string Add1, std::string Add2) {
	int Len1 = StringToMem(Add1, 1)-1;
	int Len2 = StringToMem(Add2, 2)-1;
	int OriginPos = 63;
	int ResultStartPos = 63;
	//int DecPos1 = GetDecimalPos(Len1, 1);
	//int DecPos2 = GetDecimalPos(Len2, 2);

	////计算整数部分的长度
	//int IntLen1 = Len1;
	//int IntLen2 = Len2;
	//if (DecPos1 > 0)
	//	IntLen1 = DecPos1 - 1;
	//if (DecPos2 > 0)
	//	IntLen2 = DecPos2 - 1;
	int MaxLen = Len1 >= Len2 ? Len1 : Len2;
	MaxLen++;
	bool bCarry = false;
	while (1)
	{
		
		int a, b;
		if (Len1 < 0)
		{
			a = 0;
		}
		else {
			a = (int)NumberBuffer[Len1];
		}
		if (Len2 < 0) {
			b = 0;
		}
		else {
			b = (int)NumberBuffer1[Len2];
		}
		if (Len1 < 0 && Len2 < 0)
			break;
		Len1--;
		Len2--;
		int Sum = bCarry ? a + b + 1 : a + b;
		if (Sum >= 10 && (Len1 >= 0 || Len2>=0)) {
			bCarry = true;
			Sum = Sum - 10;
		}
		else {
			bCarry = false;
		}
		NumberBuffer[ResultStartPos] = Sum;
		ResultStartPos++;
	}
	memcpy(NumberBuffer, NumberBuffer + OriginPos, MaxLen);
	Length = MaxLen;
	std::string Result = _strrev((char*)GetBigNumber().c_str()) ;
	return Result;



}