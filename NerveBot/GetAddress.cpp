#include "stdafx.h"
#include "sha256.h"
#include "Ripemd-160.h"
#include "GetAddress.h"
#include "Base58.h"
#define RMDsize  160
std::string string2hex(const std::uint8_t *data, int len);
std::string hex2string(const std::string &strHex);
int  ripemd160(unsigned char  *ldat, int ldatlen, unsigned char  *lout);
byte *RMD(byte *message)
/*
* returns RMD(message)
* message should be a string terminated by '\0'
*/
{
	dword         MDbuf[RMDsize / 32];   /* contains (A, B, C, D(, E))   */
	static byte   hashcode[RMDsize / 8]; /* for final hash-value         */
	dword         X[16];               /* current 16-word chunk        */
	unsigned int  i;                   /* counter                      */
	dword         length;              /* length in bytes of message   */
	dword         nbytes;              /* # of bytes not yet processed */

									   /* initialize */
	MDinit(MDbuf);
	length = (dword)strlen((char *)message);

	/* process message in 16-word chunks */
	for (nbytes = length; nbytes > 63; nbytes -= 64) {
		for (i = 0; i < 16; i++) {
			X[i] = BYTES_TO_DWORD(message);
			message += 4;
		}
		compress(MDbuf, X);
	}                                    /* length mod 64 bytes left */

										 /* finish: */
	MDfinish(MDbuf, message, length, 0);

	for (i = 0; i < RMDsize / 8; i += 4) {
		hashcode[i] = MDbuf[i >> 2];         /* implicit cast to byte  */
		hashcode[i + 1] = (MDbuf[i >> 2] >> 8);  /*  extracts the 8 least  */
		hashcode[i + 2] = (MDbuf[i >> 2] >> 16);  /*  significant bits.     */
		hashcode[i + 3] = (MDbuf[i >> 2] >> 24);
	}

	return (byte *)hashcode;
}



std::string GetAddressBuyPubKey(int ChainId, int Type,std::string PubKey, std::string Prefix) {
	char CAndRip[46] = { 0 };
	char ChainIdAndType[4] = {0};
	
	

	uint8_t ChainId01 = 0xFF & ChainId >> 0;
	uint8_t ChainId02 = 0xFF & ChainId >> 8;
	memcpy_s(ChainIdAndType, 1, &ChainId01, 1);
	memcpy_s(ChainIdAndType + 1, 1, &ChainId02, 1);
	memcpy_s(ChainIdAndType + 2, 1, &Type, 1);



	SHA256 sha256;
	std::string Sha256PubKey = hex2string(sha256(hex2string(PubKey)));


	unsigned char RIP[21] = { 0 };
	ripemd160((unsigned char*)Sha256PubKey.c_str(), Sha256PubKey.size(), RIP);

	//byte* b = (byte*)Sha256PubKey.c_str();
	//byte *hashcode = RMD(b);
	memcpy_s(CAndRip, 3, ChainIdAndType, 3);
	memcpy_s(CAndRip+3, 20, RIP, 20);
	uint8_t Xor = 0x00;
	for (int i = 0; i < 23; i++)
	{
		Xor ^= CAndRip[i];
	}
	memcpy_s(CAndRip + 23, 1, (const void*)&Xor, 1);	
	int Len = 24;
	std::string Hash = string2hex((const uint8_t*)CAndRip, Len);
	char AddressBuf[MAX_PATH] = { 0 };
	int AddressLen = 0;	
	b58enc(AddressBuf, (size_t*)&AddressLen, CAndRip, (size_t)Len);
	char Constant[6] = "abcde";
	int Index = Prefix.size()-1;
	std::string Address = Prefix;
	Address += Constant[Index];
	Address += AddressBuf;
	return Address;
}