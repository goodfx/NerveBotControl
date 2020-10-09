#include "stdafx.h"
#include <string>
#include "Base58.h"
static const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz)
{
	const uint8_t *bin = (const uint8_t*)data;
	int carry;
	size_t i, j, high, zcount = 0;
	uint8_t buf[12 * 1024] = { 0 };
	size_t size;

	//计算待编码数据前面 0 的个数 
	while (zcount < (size_t)binsz && !bin[zcount])
		++zcount;

	//计算存放转换数据所需要的数组的大小    138/100 --> log(256)/log(58)
	size = (binsz - zcount) * 138 / 100 + 1;
	memset(buf, 0, size);

	//遍历待转换数据
	for (i = zcount, high = size - 1; i < (size_t)binsz; ++i, high = j)
	{
		//将数据从后往前依次存放
		for (carry = bin[i], j = size - 1; (j > high) || carry; --j)
		{
			carry += 256 * buf[j];
			buf[j] = carry % 58;
			carry /= 58;
		}
	}

	for (j = 0; j < (size_t)size && !buf[j]; ++j);

	//if (*b58sz <= zcount + size - j)
	//{
	//	*b58sz = zcount + size - j + 1;
	//	return false;
	//}

	if (zcount)
		memset(b58, '1', zcount);
	for (i = zcount; j < (size_t)size; ++i, ++j)
		b58[i] = b58digits_ordered[buf[j]];
	b58[i] = '\0';
	*b58sz = i + 1;

	return true;
}
void Base58() {
	char plainText[] = "abcdefg";
	int i;
	long long sum = 0;
	int len = strlen(plainText) * 138 / 100 + 1;// len * log(2)256 / log(2)(58) + 1
	char base58Table[59] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	char *encryption = (char *)malloc(len * sizeof(char));
	int index = 0;

	memset(encryption, 0, len * sizeof(char));

	while (index < strlen(plainText)) {
		int each = plainText[index];
		for (i = len - 1; ; i--) {
			each += encryption[i] * 256;
			encryption[i] = each % 58;
			each /= 58;
			if (0 == each)
				break;
		}

		i = 0;//输出
		while (!encryption[i])
			i++;
		for (; i <= len - 1; i++) {
			printf("%d ", encryption[i]);
		}

		index++;
	}

	i = 0;
	while (!encryption[i])
		i++;
	for (; i <= len - 1; i++) {
		char t = base58Table[encryption[i]];
	}
}