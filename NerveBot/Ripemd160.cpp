#include "stdafx.h"
unsigned char R[80] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
	7,  4, 13,  1, 10,  6, 15,  3, 12,  0,  9,  5,  2, 14, 11,  8,
	3, 10, 14,  4,  9, 15,  8,  1,  2,  7,  0,  6, 13, 11,  5, 12,
	1,  9, 11, 10,  0,  8, 12,  4, 13,  3,  7, 15, 14,  5,  6,  2,
	4,  0,  5,  9,  7, 12,  2, 10, 14,  1,  3,  8, 11,  6, 15, 13 };

unsigned char RR[80] = {
	5, 14,  7,  0,  9,  2, 11,  4, 13,  6, 15,  8,  1, 10,  3, 12,
	6, 11,  3,  7,  0, 13,  5, 10, 14, 15,  8, 12,  4,  9,  1,  2,
   15,  5,  1,  3,  7, 14,  6,  9, 11,  8, 12,  2, 10,  0,  4, 13,
	8,  6,  4,  1,  3, 11, 15,  0,  5, 12,  2, 13,  9,  7, 10, 14,
   12, 15, 10,  4,  1,  5,  8,  7,  6,  2, 13, 14,  0,  3,  9, 11 };

unsigned char S[80] = {
   11, 14, 15, 12,  5,  8,  7,  9, 11, 13, 14, 15,  6,  7,  9,  8,
	7,  6,  8, 13, 11,  9,  7, 15,  7, 12, 15,  9, 11,  7, 13, 12,
   11, 13,  6,  7, 14,  9, 13, 15, 14,  8, 13,  6,  5, 12,  7,  5,
   11, 12, 14, 15, 14, 15,  9,  8,  9, 14,  5,  6,  8,  6,  5, 12,
	9, 15,  5, 11,  6,  8, 13, 12,  5, 12, 13, 14, 11,  8,  5,  6 };

unsigned char SS[80] = {
	8,  9,  9, 11, 13, 15, 15,  5,  7,  7,  8, 11, 14, 14, 12,  6,
	9, 13, 15,  7, 12,  8,  9, 11,  7,  7, 12,  7,  6, 15, 13, 11,
	9,  7, 15, 11,  8,  6,  6, 14, 12, 13,  5, 14, 13, 13,  7,  5,
   15,  5,  8, 11, 14, 14,  6, 14,  6,  9, 12,  9, 12,  5, 15,  8,
	8,  5, 12,  9, 12,  5, 14,  6,  8, 13,  6,  5, 15, 13, 11, 11 };

unsigned int K_values[5] = { 0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E };
unsigned int KK_values[4] = { 0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x00000000 };
unsigned int KK160_values[5] = { 0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000 };

#define K(n)  K_values[ (n) >> 4] 
#define KK(n) KK_values[(n) >> 4] 
#define KK160(n) KK160_values[(n) >> 4] 

#define ROLS(j, x) (((x) << S[j])  | ((x) >> (32 - S[j])))    
#define ROLSS(j, x) (((x) << SS[j]) | ((x) >> (32 - SS[j])))  
#define ROL(n, x) (((x) << n) | ((x) >> (32 - n)))            

#define FF0(x,y,z) ((x) ^ (y) ^ (z)) 
#define FF1(x,y,z) (((x) & (y)) | ((~(x)) & (z))) 
#define FF2(x,y,z) (((x) | (~(y))) ^ (z)) 
#define FF3(x,y,z) (((x) & (z)) | ((y) & (~(z)))) 
#define FF4(x,y,z) ((x) ^ ((y) | (~(z)))) 


int  ripemd160(unsigned char  *ldat, int ldatlen, unsigned char  *lout)
{
	int i, j, k, t1, t2;
	int L;  unsigned int *data;
	unsigned int *x;
	unsigned int  tk;
	unsigned int  A, B, C, D, E;
	unsigned int  a, b, c, d, e, aa, bb, cc, dd, ee, tmp;

	unsigned char ltmp[128];





	//需字节长度
	L = ((ldatlen >> 6) + 1) << 6;
	if ((ldatlen & 0x3f) >= 56) L += 64;


	//	printf("%d\n",L);
	data = (unsigned int *)malloc((L >> 2) * sizeof(int));
	if (data == NULL)
	{
		for (i = 0; i < 20; i++) lout[i] = 0;
		return 0;
	}



	//pad...... －－－ltmp
	ltmp[0] = 0x80;
	j = L - 8 - ldatlen;//if(data==NULL) printf("&&&&&&&&&&&&&7\n");
	for (i = 1; i < j; i++) ltmp[i] = 0;
	tk = ldatlen << 3;

	t1 = L - ldatlen;
	for (i = t1 - 8; i < t1 - 4; i++) { ltmp[i] = tk & 0xff; tk >>= 8; }  /////RRRRRRRRRRRRRRRRRRRRRRRRRRRRr
	for (i = t1 - 4; i < t1; i++) { ltmp[i] = tk & 0xff; tk >>= 8; }
	//		for(i=j;i<t1;i++) ltmp[i]=0;
	for (i = j; i < t1 - 8; i++) ltmp[i] = 0;

	//填充高低字节摆放问题

	//变成整形


	t2 = (ldatlen >> 2) << 2;
	for (k = 0, i = 0; i < t2; i += 4, k++)
	{
		for (tk = 0, j = 0; j < 4; j++) tk = (tk << 8) ^ ldat[i + 3 - j];//printf("***************** %d %x\n",k,K);	
		data[k] = tk;
	}


	t2 = ldatlen & 0x3;
	for (tk = 0, j = 3 - t2; j >= 0; j--) tk = (tk << 8) ^ ltmp[j];
	for (j = ldatlen - 1; j >= i; j--) tk = (tk << 8) ^ ldat[j];  ////RRRRRRRRRRRRRRRRR

	data[k] = tk; k++;
	for (i = 4 - t2; i < t1; i += 4, k++)
	{
		for (tk = 0, j = 0; j < 4; j++) tk = (tk << 8) ^ ltmp[i + 3 - j];
		data[k] = tk;
	}  ///////RRRRRRRRRRRRRRRRRRRRRRRRRRRRRR data 中高低字节的摆放问题 


	L >>= 6;


	A = 0x67452301;
	B = 0xefcdab89;
	C = 0x98badcfe;
	D = 0x10325476;
	E = 0xC3D2E1F0;

	for (x = data, i = 0; i < L; i++, x += 16)
	{
		a = A; b = B; c = C; d = D; e = E;
		aa = A; bb = B; cc = C; dd = D; ee = E;

		for (j = 0; j < 16; j++)
		{
			tmp = ROLS(j, a + FF0(b, c, d) + x[R[j]] + K(j)) + e;
			a = e; e = d; d = ROL(10, c); c = b; b = tmp;
			tmp = ROLSS(j, aa + FF4(bb, cc, dd) + x[RR[j]] + KK160(j)) + ee;
			aa = ee; ee = dd; dd = ROL(10, cc); cc = bb; bb = tmp;
		}

		for (j = 16; j < 32; j++)
		{
			tmp = ROLS(j, a + FF1(b, c, d) + x[R[j]] + K(j)) + e;
			a = e; e = d; d = ROL(10, c); c = b; b = tmp;
			tmp = ROLSS(j, aa + FF3(bb, cc, dd) + x[RR[j]] + KK160(j)) + ee;
			aa = ee; ee = dd; dd = ROL(10, cc); cc = bb; bb = tmp;
		}

		for (j = 32; j < 48; j++)
		{
			tmp = ROLS(j, a + FF2(b, c, d) + x[R[j]] + K(j)) + e;
			a = e; e = d; d = ROL(10, c); c = b; b = tmp;
			tmp = ROLSS(j, aa + FF2(bb, cc, dd) + x[RR[j]] + KK160(j)) + ee;
			aa = ee; ee = dd; dd = ROL(10, cc); cc = bb; bb = tmp;
		}

		for (j = 48; j < 64; j++)
		{
			tmp = ROLS(j, a + FF3(b, c, d) + x[R[j]] + K(j)) + e;
			a = e; e = d; d = ROL(10, c); c = b; b = tmp;
			tmp = ROLSS(j, aa + FF1(bb, cc, dd) + x[RR[j]] + KK160(j)) + ee;
			aa = ee; ee = dd; dd = ROL(10, cc); cc = bb; bb = tmp;
		}

		for (j = 64; j < 80; j++)
		{
			tmp = ROLS(j, a + FF4(b, c, d) + x[R[j]] + K(j)) + e;
			a = e; e = d; d = ROL(10, c); c = b; b = tmp;
			tmp = ROLSS(j, aa + FF0(bb, cc, dd) + x[RR[j]] + KK160(j)) + ee;
			aa = ee; ee = dd; dd = ROL(10, cc); cc = bb; bb = tmp;
		}
		tmp = B + c + dd;
		B = C + d + ee;
		C = D + e + aa;
		D = E + a + bb;
		E = A + b + cc;
		A = tmp;
	}

	free(data);



	i = 0;
	for (j = 0; j < 4; j++, i++) { lout[i] = A & 0xff; A >>= 8; }
	for (j = 0; j < 4; j++, i++) { lout[i] = B & 0xff; B >>= 8; }
	for (j = 0; j < 4; j++, i++) { lout[i] = C & 0xff; C >>= 8; }
	for (j = 0; j < 4; j++, i++) { lout[i] = D & 0xff; D >>= 8; }
	for (j = 0; j < 4; j++, i++) { lout[i] = E & 0xff; E >>= 8; }
	return 1;
}