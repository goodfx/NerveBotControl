#include "stdafx.h"
#include <string>
#include <sstream> 
#include <iomanip>
#include <assert.h>
#include "StringConvert.h"
#include "secp256k1.h"
#include "secp256k1_ecdh.h"
#include "secp256k1_recovery.h"
std::string hex2string(const std::string &strHex);
std::string string2hex(const std::uint8_t *data, int len);
std::uint8_t HexStringToByte(unsigned char* Str);
std::string GetPubKey(std::string PrivateKey) {
	secp256k1_context *Ctx = nullptr;
	secp256k1_pubkey PubKey;

	//std::uint8_t PrivateKey[] = "71361500124b2e4ca11f68c9148a064bb77fe319d8d27b798af4dda3f4d910cc";
	//std::uint8_t PrivateKey[] = "53b02c91605451ea35175df894b4c47b7d1effbd05d6b269b3e7c785f3f6dc18";
	int PrivateKeySize = std::extent<decltype(PrivateKey)>::value;

	Ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);

	//二进制私钥
	std::string PriKey = hex2string((const char*)PrivateKey.c_str());

	//根据私钥生成公钥
	secp256k1_ec_pubkey_create(Ctx, &PubKey, (const unsigned char*)PriKey.c_str());

	unsigned char PubKeyStr[1024] = { 0 };
	size_t PubLen = 65;


	secp256k1_ec_pubkey_serialize(Ctx, PubKeyStr, &PubLen, &PubKey, SECP256K1_EC_COMPRESSED);
	//std::string SerPubKeyHex = string2hex((const unsigned char *)PubKeyStr, 1024);

	char SerPubKeyStr[1024] = { 0 };
	for (int i = 0; i <= 32; i++)
	{
		char buf[4] = { 0 };
		sprintf_s(buf, "%02x", PubKeyStr[i]);
		strcat_s(SerPubKeyStr, buf);
	}
	return std::string(SerPubKeyStr);
}
std::string Singture(std::string PrivateKey,std::string Hash)
{
	secp256k1_context *Ctx = nullptr;
	secp256k1_pubkey PubKey;

	//std::uint8_t PrivateKey[] = "71361500124b2e4ca11f68c9148a064bb77fe319d8d27b798af4dda3f4d910cc";
	//std::uint8_t PrivateKey[] = "53b02c91605451ea35175df894b4c47b7d1effbd05d6b269b3e7c785f3f6dc18";
	int PrivateKeySize = std::extent<decltype(PrivateKey)>::value;

	Ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);

	//二进制私钥
	std::string PriKey = hex2string((const char*)PrivateKey.c_str());

	//根据私钥生成公钥
	secp256k1_ec_pubkey_create(Ctx, &PubKey, (const unsigned char*)PriKey.c_str());

	unsigned char PubKeyStr[1024] = { 0 };
	size_t PubLen = 65;


	secp256k1_ec_pubkey_serialize(Ctx, PubKeyStr, &PubLen, &PubKey, SECP256K1_EC_COMPRESSED);
	//std::string SerPubKeyHex = string2hex((const unsigned char *)PubKeyStr, 1024);

	char SerPubKeyStr[1024] = { 0 };
	for (int i = 0; i <= 32; i++)
	{
		char buf[4] = { 0 };
		sprintf_s(buf, "%02x", PubKeyStr[i]);
		strcat_s(SerPubKeyStr, buf);
	}

	//签名

	//std::uint8_t Hash[] = "0533529e794bc9560159cd48481af141897cd88f3baa292ea698397a25da9d25";
	std::string HashStr = hex2string((const char*)Hash.c_str());


	secp256k1_ecdsa_signature sig;
	int ret = secp256k1_ecdsa_sign(Ctx, &sig, (const unsigned char *)HashStr.c_str(), (const unsigned char *)PriKey.c_str(), NULL, NULL);
	//unsigned char sig64[65] = { 0 };
	//secp256k1_ecdsa_signature_serialize_compact(Ctx, sig64, &sig);
	//std::string SerSig = string2hex(sig64, 64);
	unsigned char Der[100] = { 0 };
	size_t DerLen = 100;
	secp256k1_ecdsa_signature_serialize_der(Ctx, Der, &DerLen, &sig);
	std::string SerDer = string2hex(Der, DerLen);

	ret = secp256k1_ecdsa_verify(Ctx, &sig, (const unsigned char *)HashStr.c_str(), &PubKey);

	int DerSize = SerDer.size() / 2;
	int PubSize = 33;
	int TotalSize = PubSize + DerSize + 2;

	std::string ComSigStr = format("%x%x%s%x%s", TotalSize, PubSize, SerPubKeyStr, DerSize, SerDer.c_str());

	LogA("DerLen:%d", SerDer.size());
	LogA("SigData:%s", SerDer.c_str());
	LogA("PubKeyStr:%s", SerPubKeyStr);
	LogA("ComSigStr:%s", ComSigStr.c_str());
	return ComSigStr;
}

//std::string hex2string(const std::string &strHex)
//{
//	if (strHex.size() % 2 != 0)
//	{
//		return "";
//	}
//	std::string strBin;
//	strBin.resize(strHex.size() / 2);
//	for (size_t i = 0; i < strBin.size(); i++)
//	{
//		uint8_t cTemp = 0;
//		for (size_t j = 0; j < 2; j++)
//		{
//			char cCur = strHex[2 * i + j];
//			if (cCur >= '0' && cCur <= '9')
//			{
//				cTemp = (cTemp << 4) + (cCur - '0');
//			}
//			else if (cCur >= 'a' && cCur <= 'f')
//			{
//				cTemp = (cTemp << 4) + (cCur - 'a' + 10);
//			}
//			else if (cCur >= 'A' && cCur <= 'F')
//			{
//				cTemp = (cTemp << 4) + (cCur - 'A' + 10);
//			}
//			else
//			{
//				return "";
//			}
//		}
//		strBin[i] = cTemp;
//	}
//	return strBin;
//}
//
//std::string string2hex(const std::uint8_t *data, int len)
//{
//	std::string result;
//	std::string tmp;
//	std::stringstream ss;
//	for (int i = 0; i < len; i++)
//	{
//		int r = int(data[i]);
//		ss << std::hex << std::setw(2)
//			<< std::setfill('0') << r << std::endl;
//		ss >> tmp;
//		result += tmp;
//	}
//	return result;
//}

std::uint8_t HexStringToByte(unsigned char* Str)
{
	int V;
	std::uint8_t Ret;
	char Bytes[100] = { 0 };
	for (int i = 0; i < strlen((const char*)Str) / 2; i++)
	{
		sscanf_s((char*)Str + i * 2, "%2X", &V);
		Bytes[i] = (char)V;
	}
	Ret = (std::uint8_t)Bytes;
	return Ret;
}