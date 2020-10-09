#include "stdafx.h"
#include <string>
#include <sstream> 
#include <iomanip>
#include <openssl/aes.h>
#include "OpenSSLAES.h"
#include "StringConvert.h"
#pragma warning(disable:4996)

std::string hex2string(const std::string &strHex)
{
	if (strHex.size() % 2 != 0)
	{
		return "";
	}
	std::string strBin;
	strBin.resize(strHex.size() / 2);
	for (size_t i = 0; i < strBin.size(); i++)
	{
		uint8_t cTemp = 0;
		for (size_t j = 0; j < 2; j++)
		{
			char cCur = strHex[2 * i + j];
			if (cCur >= '0' && cCur <= '9')
			{
				cTemp = (cTemp << 4) + (cCur - '0');
			}
			else if (cCur >= 'a' && cCur <= 'f')
			{
				cTemp = (cTemp << 4) + (cCur - 'a' + 10);
			}
			else if (cCur >= 'A' && cCur <= 'F')
			{
				cTemp = (cTemp << 4) + (cCur - 'A' + 10);
			}
			else
			{
				return "";
			}
		}
		strBin[i] = cTemp;
	}
	return strBin;
}

std::string string2hex(const std::uint8_t *data, int len)
{
	std::string result;
	std::string tmp;
	std::stringstream ss;
	for (int i = 0; i < len; i++)
	{
		int r = int(data[i]);
		ss << std::hex << std::setw(2)
			<< std::setfill('0') << r << std::endl;
		ss >> tmp;
		result += tmp;
	}
	return result;
}

std::string AesEncrypt(std::string input_string,std::string secret) {
	char encrypt_string[8192] = { 0 };
	AES_KEY aes;
	char key[MAX_PATH] = {0};
	char iv[17] = "fe309af0f4d35982";
	input_string = G2U(input_string.c_str());
	int nLen = input_string.length();
	int nBei = nLen / AES_BLOCK_SIZE + 1;
	int nTotal = nBei * AES_BLOCK_SIZE;
	char enc_s[1024] = { 0 };
	int nNumber;
	if (nLen % 16 > 0)
		nNumber = nTotal - nLen;
	else
		nNumber = 16;
	memset(enc_s, nNumber, nTotal);
	memcpy(enc_s, input_string.data(), nLen);
	memcpy(key, secret.c_str(), secret.size());
	if (AES_set_encrypt_key((unsigned char *)key, 128, &aes) < 0) {
		exit(-1);
	}
	AES_cbc_encrypt((unsigned char *)enc_s, (unsigned char *)encrypt_string, nBei * 16,
		&aes,
		(unsigned char *)iv, AES_ENCRYPT);
	return string2hex((unsigned char *)encrypt_string, nBei * 16);
	//char *res = Base64Encode((const unsigned char *)encrypt_string, nBei * 16);
	//printf("the encrypt result is %s\n", res);
}
std::string AesEncrypt(std::string input_string) {
	char encrypt_string[8192] = { 0 };
	AES_KEY aes;
	char key[17] = "wildlove";
	char iv[17] = "fe309af0f4d35982";
	input_string = G2U(input_string.c_str());
	int nLen = input_string.length();
	int nBei = nLen / AES_BLOCK_SIZE + 1;
	int nTotal = nBei * AES_BLOCK_SIZE;
	char enc_s[1024] = { 0 };
	int nNumber;
	if (nLen % 16 > 0)
		nNumber = nTotal - nLen;
	else
		nNumber = 16;
	memset(enc_s, nNumber, nTotal);
	memcpy(enc_s, input_string.data(), nLen);
	if (AES_set_encrypt_key((unsigned char *)key, 128, &aes) < 0) {
		exit(-1);
	}
	AES_cbc_encrypt((unsigned char *)enc_s, (unsigned char *)encrypt_string, nBei * 16,
		&aes,
		(unsigned char *)iv, AES_ENCRYPT);
	return string2hex((unsigned char *)encrypt_string, nBei * 16);
	//char *res = Base64Encode((const unsigned char *)encrypt_string, nBei * 16);
	//printf("the encrypt result is %s\n", res);
}

std::string AesDecrypt(std::string encode_string,std::string secret ) {
	std::string DecryptString = "";
	char encrypt_string[8192] = { 0 };
	AES_KEY aes;
	char key[MAX_PATH] = { 0 };
	char iv[17] = "fe309af0f4d35982";
	std::string Tmp = hex2string(encode_string);
	memcpy(key, secret.c_str(), secret.size());
	char *decode = (char*)hex2string(encode_string).c_str();
	if (AES_set_decrypt_key((unsigned char *)key, 128, &aes) < 0) {
		exit(-1);
	}

	AES_cbc_encrypt((unsigned char *)decode, (unsigned char *)encrypt_string, strlen((decode)),
		&aes,
		(unsigned char *)iv, AES_DECRYPT);
	int decryptSize = strlen(encrypt_string);
	int k = decryptSize;
	for (int i = 0; i < decryptSize; i++) {
		if ((int)(encrypt_string[i]) <= 16) {
			k = i;
			break;
		}
	}
	char result[1024] = { 0 };
	strncpy(result, encrypt_string, k);
	DecryptString = result;
	return DecryptString;
}

//void handleErrors(void)
//{
//	//ERR_print_errors_fp(stderr);
//	abort();
//}
//
//int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
//	unsigned char *iv, unsigned char *ciphertext)
//{
//	EVP_CIPHER_CTX *ctx;
//
//	int len;
//
//	int ciphertext_len;
//
//	/* Create and initialise the context */
//	if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//	/* Initialise the encryption operation. IMPORTANT - ensure you use a key
//	* and IV size appropriate for your cipher
//	* In this example we are using 256 bit AES (i.e. a 256 bit key). The
//	* IV size for *most* modes is the same as the block size. For AES this
//	* is 128 bits */
//	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
//		handleErrors();
//
//	/* Provide the message to be encrypted, and obtain the encrypted output.
//	* EVP_EncryptUpdate can be called multiple times if necessary
//	*/
//	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//		handleErrors();
//	ciphertext_len = len;
//
//	/* Finalise the encryption. Further ciphertext bytes may be written at
//	* this stage.
//	*/
//	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
//	ciphertext_len += len;
//
//	/* Clean up */
//	EVP_CIPHER_CTX_free(ctx);
//
//	return ciphertext_len;
//}
//
//int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
//	unsigned char *iv, unsigned char *plaintext)
//{
//	EVP_CIPHER_CTX *ctx;
//
//	int len;
//
//	int plaintext_len;
//
//	/* Create and initialise the context */
//	if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
//	* and IV size appropriate for your cipher
//	* In this example we are using 256 bit AES (i.e. a 256 bit key). The
//	* IV size for *most* modes is the same as the block size. For AES this
//	* is 128 bits */
//	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
//		handleErrors();
//
//	/* Provide the message to be decrypted, and obtain the plaintext output.
//	* EVP_DecryptUpdate can be called multiple times if necessary
//	*/
//	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
//		handleErrors();
//	plaintext_len = len;
//
//	/* Finalise the decryption. Further plaintext bytes may be written at
//	* this stage.
//	*/
//	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
//	plaintext_len += len;
//
//	/* Clean up */
//	EVP_CIPHER_CTX_free(ctx);
//
//	return plaintext_len;
//}
//
//
//
//
//int TestEnCryp()
//{
//	/* Set up the key and iv. Do I need to say to not hard code these in a
//	* real application? :-)
//	*/
//
//	/* A 256 bit key */
//	unsigned char *key = (unsigned char*)"1234567890123456";
//
//	/* A 128 bit IV */
//	unsigned char *iv = (unsigned char*)"1234567890123456";
//
//	/* Message to be encrypted */
//	unsigned char *plaintext = (unsigned char*)"aaaaaaaaaaaaaaaa";
//
//	/* Buffer for ciphertext. Ensure the buffer is long enough for the
//	* ciphertext which may be longer than the plaintext, dependant on the
//	* algorithm and mode
//	*/
//	unsigned char ciphertext[1024] = {0};
//
//	/* Buffer for the decrypted text */
//	unsigned char decryptedtext[1024];
//
//	int decryptedtext_len, ciphertext_len;
//
//	/* Initialise the library */
//	/*  ERR_load_crypto_strings();
//	OpenSSL_add_all_algorithms();
//	OPENSSL_config(NULL);*/
//
//	printf("Plaintext is:\n%s\n", plaintext);
//
//	/* Encrypt the plaintext */
//	ciphertext_len = encrypt(plaintext, strlen((const char*)plaintext), key, iv,
//		ciphertext);
//
//	/* Do something useful with the ciphertext here */
//	printf("Ciphertext is %d bytes long:\n", ciphertext_len);
//	BIO_dump_fp(stdout, (const char*)ciphertext, ciphertext_len);
//
//	/* Decrypt the ciphertext */
//	decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
//		decryptedtext);
//
//	/* Add a NULL terminator. We are expecting printable text */
//	decryptedtext[decryptedtext_len] = '\0';
//
//	/* Show the decrypted text */
//	printf("Decrypted text is:\n");
//	printf("%s\n", decryptedtext);
//
//	/* Clean up */
//	EVP_cleanup();
//	//ERR_free_strings();
//	system("pause");
//
//	return 0;
//}