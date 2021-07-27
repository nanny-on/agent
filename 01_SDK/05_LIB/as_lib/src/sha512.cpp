/*
 * Define SHA512_TEST to test the implementation using the NIST's
 * sample messages. The output should be:
 *
 * * 9b71d224bd62f378 5d96d46ad3ea3d73 319bfbc2890caada e2dff72519673ca7 
 * * 2323c3d99ba5c11d 7c7acc6e14b8c5da 0c4663475c2e5c3a def46f73bcdec043 
 * * 8e959b75dae313da 8cf4f72814fc143f 8f7779c6eb9f7fa1 7299aeadb6889018
 * * 501d289e4900f7e4 331b99dec4b5433a c7d329eeb6dd2654 5e96e55b874be909
 * * e718483d0ce76964 4e2e42c7bc15b463 8e1f98b13b204428 5632a803afa973eb
 * * de0ff244877ea60a 4cb0432ce577c31b eb009c5c2c49aa2e 4eadb217ad8cc09b
 */

#include "stdafx.h"
#include "as_sha512.h"

#define BYTESWAP(x) _byteswap(pSc->littleEndian, x)
#define BYTESWAP64(x) _byteswap64(pSc->littleEndian, x)
#define _BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
		(ROTL((x), 8) & 0x00ff00ffL))
#define _BYTESWAP64(x) __byteswap64(x)


static const UINT64 K[80] = {
	0x428a2f98d728ae22LL, 0x7137449123ef65cdLL,
	0xb5c0fbcfec4d3b2fLL, 0xe9b5dba58189dbbcLL,
	0x3956c25bf348b538LL, 0x59f111f1b605d019LL,
	0x923f82a4af194f9bLL, 0xab1c5ed5da6d8118LL,
	0xd807aa98a3030242LL, 0x12835b0145706fbeLL,
	0x243185be4ee4b28cLL, 0x550c7dc3d5ffb4e2LL,
	0x72be5d74f27b896fLL, 0x80deb1fe3b1696b1LL,
	0x9bdc06a725c71235LL, 0xc19bf174cf692694LL,
	0xe49b69c19ef14ad2LL, 0xefbe4786384f25e3LL,
	0x0fc19dc68b8cd5b5LL, 0x240ca1cc77ac9c65LL,
	0x2de92c6f592b0275LL, 0x4a7484aa6ea6e483LL,
	0x5cb0a9dcbd41fbd4LL, 0x76f988da831153b5LL,
	0x983e5152ee66dfabLL, 0xa831c66d2db43210LL,
	0xb00327c898fb213fLL, 0xbf597fc7beef0ee4LL,
	0xc6e00bf33da88fc2LL, 0xd5a79147930aa725LL,
	0x06ca6351e003826fLL, 0x142929670a0e6e70LL,
	0x27b70a8546d22ffcLL, 0x2e1b21385c26c926LL,
	0x4d2c6dfc5ac42aedLL, 0x53380d139d95b3dfLL,
	0x650a73548baf63deLL, 0x766a0abb3c77b2a8LL,
	0x81c2c92e47edaee6LL, 0x92722c851482353bLL,
	0xa2bfe8a14cf10364LL, 0xa81a664bbc423001LL,
	0xc24b8b70d0f89791LL, 0xc76c51a30654be30LL,
	0xd192e819d6ef5218LL, 0xd69906245565a910LL,
	0xf40e35855771202aLL, 0x106aa07032bbd1b8LL,
	0x19a4c116b8d2d0c8LL, 0x1e376c085141ab53LL,
	0x2748774cdf8eeb99LL, 0x34b0bcb5e19b48a8LL,
	0x391c0cb3c5c95a63LL, 0x4ed8aa4ae3418acbLL,
	0x5b9cca4f7763e373LL, 0x682e6ff3d6b2b8a3LL,
	0x748f82ee5defb2fcLL, 0x78a5636f43172f60LL,
	0x84c87814a1f0ab72LL, 0x8cc702081a6439ecLL,
	0x90befffa23631e28LL, 0xa4506cebde82bde9LL,
	0xbef9a3f7b2c67915LL, 0xc67178f2e372532bLL,
	0xca273eceea26619cLL, 0xd186b8c721c0c207LL,
	0xeada7dd6cde0eb1eLL, 0xf57d4f7fee6ed178LL,
	0x06f067aa72176fbaLL, 0x0a637dc5a2c898a6LL,
	0x113f9804bef90daeLL, 0x1b710b35131c471bLL,
	0x28db77f523047d84LL, 0x32caab7b40c72493LL,
	0x3c9ebe0a15c9bebcLL, 0x431d67c49c100d4cLL,
	0x4cc5d4becb3e42b6LL, 0x597f299cfc657e2aLL,
	0x5fcb6fab3ad6faecLL, 0x6c44198c4a475817LL
};


static const UINT8 padding[128] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


static inline void setEndian(int *littleEndianp);
static inline UINT64 __byteswap64(UINT64 x);
static inline UINT32 _byteswap(int littleEndian, UINT32 x);
static inline UINT64 _byteswap64(int littleEndian, UINT64 x);
static INT32 SHA512Init(SHA512Context *pSc);
static INT32 SHA512Update(SHA512Context *pSc, const void *pInData, UINT32 nInLen);
static INT32 SHA512Guts(SHA512Context *pSc, const UINT64 *pddwBuf);
static void burnStack(int size);
static INT32 SHA512Final(SHA512Context *pSc, UINT8 hash[AS_SHA512_DIGEST_SIZE]);


INT32 conv_sha512_digest(unsigned char *pDigest, char *pHash, int nHashLen)
{
	int i;
	char tmp[4] = {0,};
	if(pDigest == NULL || pHash == NULL || nHashLen < AS_SHA512_DIGEST_SIZE*2+1)
		return -1;

	memset(pHash, 0, nHashLen);
	for (i=0; i<AS_SHA512_DIGEST_SIZE; i++)
	{
		snprintf(tmp, 3, "%02X", pDigest[i]);
		tmp[3] = 0;
		if(i==0)
			strncpy(pHash, tmp, 2);
		else
			strncat(pHash, tmp, 2);
	}
	pHash[AS_SHA512_DIGEST_SIZE*2] = 0;
	return 0;
}

/* 입력된 문자열의 sha512 digest 값을 구함 */
INT32 get_sha512_digest(char *pStr, int nStrLen, unsigned char *pDigest, int nDigestLen)
{
	INT32 nRetVal = 0;
	SHA512Context ctx;
	if(pStr == NULL || nStrLen < 1 || pDigest == NULL || nDigestLen < AS_SHA512_DIGEST_SIZE)
	{
		return -1;
	}
	memset(pDigest, 0, nDigestLen);

	SHA512Init(&ctx);
	nRetVal = SHA512Update(&ctx, (unsigned char *)pStr, nStrLen);
	if(nRetVal != 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	nRetVal = SHA512Final(&ctx, pDigest);
	if(nRetVal != 0)
	{
		nRetVal -= 200;
		return nRetVal;
	}
	return 0;
}

INT32 get_sha512_hash(char *pStr, int nStrLen, char *pcHash, int nHashLen)
{
	INT32 nRetVal = 0;
	unsigned char acDigest[AS_SHA512_DIGEST_SIZE+1] = {0,};
	SHA512Context ctx;

	if(pStr == NULL || nStrLen < 1 || pcHash == NULL || nHashLen < 2*AS_SHA512_DIGEST_SIZE+1)
	{
		return -1;
	}
	memset(pcHash, 0x00, nHashLen);
	do{
		SHA512Init(&ctx);
		nRetVal = SHA512Update(&ctx, (unsigned char *)pStr, nStrLen);
		if(nRetVal != 0)
		{
			nRetVal -= 100;
			break;
		}

		nRetVal = SHA512Final(&ctx, acDigest);
		if(nRetVal != 0)
		{
			nRetVal -= 200;
			break;
		}

		acDigest[AS_SHA512_DIGEST_SIZE] = 0;

		nRetVal = conv_sha512_digest(acDigest, pcHash, nHashLen);
		if(nRetVal != 0)
		{
			nRetVal -= 300;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}


/* hash1, hash2 digest 값을 비교하여 같으면 0, 틀리면 -1 리턴 */
INT32 comp_sha512_digest(unsigned char *digest1, unsigned char *digest2)
{
	INT32 idx = 64;

	while (idx)
	{
		idx--;
		if (digest1[idx] == digest2[idx])
		{
			continue;
		}
		return -1;
	}
	return 0;
}


INT32 get_sha512_hash_from_file(char *pPath, char *pHash, int nHashLen, off_t max_read_byte)
{
	off_t size_to_read = 0;
	INT32 fd = 0;
	INT32 nRead = 0;
	INT32 nRetVal = 0;
	char *pcBuf = NULL;
	UINT8 abDigest[AS_SHA512_DIGEST_SIZE+1] = {0,};
	SHA512Context ctx;
	struct stat f_stat;

	if(pPath == NULL || pHash == NULL || nHashLen < AS_SHA512_DIGEST_SIZE*2+1)
		return -1;

	memset(pHash, 0x00, nHashLen);
	memset(abDigest, 0x00, AS_SHA512_DIGEST_SIZE);

	if (stat(pPath, &f_stat) == -1)
	{
		return -2;
	}

	pcBuf = (char *)malloc(AS_FILE_READ_MAX+1);
	if(pcBuf == NULL)
	{
		return -3;
	}

	if ( (fd = open(pPath, O_RDONLY)) == -1)
	{
		safe_free(pcBuf);
		return -4;
	}

	if (max_read_byte == 0 || f_stat.st_size <= max_read_byte)
	{
		size_to_read = f_stat.st_size;
	}
	else
	{
		size_to_read = max_read_byte;
	}

	nRetVal = SHA512Init(&ctx);
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		safe_free(pcBuf);
		close(fd);
		return nRetVal;
	}

	do {
		memset(pcBuf, 0, AS_FILE_READ_MAX+1);

		if ( (nRead = read(fd, pcBuf, AS_FILE_READ_MAX)) <= 0)
		{
			break;
		}

		nRetVal = SHA512Update(&ctx, pcBuf, nRead);
		if(nRetVal < 0)
		{
			nRetVal -= 2000;
			safe_free(pcBuf);
			close(fd);
			return nRetVal;
		}
		size_to_read -= nRead;

	} while (size_to_read > 0);

	close(fd);
	safe_free(pcBuf);

	if (size_to_read != 0)
	{
		return -5;
	}

	nRetVal = SHA512Final(&ctx, abDigest);
	if(nRetVal < 0)
	{
		nRetVal -= 3000;
		return nRetVal;
	}

	nRetVal = conv_sha512_digest(abDigest, pHash, nHashLen);
	if(nRetVal < 0)
	{
		nRetVal -= 4000;
		return nRetVal;
	}
	return 0;
}


static inline void setEndian(int *littleEndianp)
{
	union {
		UINT32 w;
		UINT8 b[4];
	} endian;
	if(littleEndianp == NULL)
		return;

	endian.w = 1L;
	*littleEndianp = (endian.b[0] != 0);
}


static inline UINT64 __byteswap64(UINT64 x)
{
	UINT32 a = x >> 32;
	UINT32 b = (UINT32) x;
	return ((UINT64) _BYTESWAP(b) << 32) | (UINT64) _BYTESWAP(a);
}


static inline UINT32 _byteswap(int littleEndian, UINT32 x)
{
	if (! littleEndian)
		return x;
	else
		return _BYTESWAP(x);
}


static inline UINT64 _byteswap64(int littleEndian, UINT64 x)
{
	if (! littleEndian)
		return x;
	else
		return _BYTESWAP64(x);
}


static INT32 SHA512Init(SHA512Context *pSc)
{
	if(pSc == NULL)
		return -1;

	setEndian(&pSc->littleEndian);

	pSc->totalLength[0] = 0LL;
	pSc->totalLength[1] = 0LL;
	pSc->hash[0] = 0x6a09e667f3bcc908LL;
	pSc->hash[1] = 0xbb67ae8584caa73bLL;
	pSc->hash[2] = 0x3c6ef372fe94f82bLL;
	pSc->hash[3] = 0xa54ff53a5f1d36f1LL;
	pSc->hash[4] = 0x510e527fade682d1LL;
	pSc->hash[5] = 0x9b05688c2b3e6c1fLL;
	pSc->hash[6] = 0x1f83d9abfb41bd6bLL;
	pSc->hash[7] = 0x5be0cd19137e2179LL;
	pSc->bufferLength = 0L;
	return 0;
}


static INT32 SHA512Update(SHA512Context *pSc, const void *pInData, UINT32 nInLen)
{
	const UINT8 *pData = (UINT8 *) pInData;
	UINT32 dwBufferBytesLeft = 0;
	UINT32 dwBytesToCopy = 0;
	UINT64 ddwCarryCheck = 0;
	int nNeedBurn = 0;

	if(pSc == NULL || pInData == NULL)
		return -1;

#ifdef SHA512_FAST_COPY
	if (pSc->bufferLength) {
		dwBufferBytesLeft = 128L - pSc->bufferLength;

		dwBytesToCopy = dwBufferBytesLeft;
		if (dwBytesToCopy > nInLen)
		{
			dwBytesToCopy = nInLen;
		}

		memcpy(&pSc->buffer.bytes[pSc->bufferLength], pData, dwBytesToCopy);

		ddwCarryCheck = pSc->totalLength[1];
		pSc->totalLength[1] += dwBytesToCopy * 8L;
		if (pSc->totalLength[1] < ddwCarryCheck) {
			pSc->totalLength[0]++;
		}

		pSc->bufferLength += dwBytesToCopy;
		pData += dwBytesToCopy;
		nLen -= dwBytesToCopy;

		if (pSc->bufferLength == 128L) {
			SHA512Guts(pSc, pSc->buffer.words);
			nNeedBurn = 1;
			pSc->bufferLength = 0L;
		}
	}

	while (nLen > 127) 
	{
		ddwCarryCheck = pSc->totalLength[1];
		pSc->totalLength[1] += 1024L;
		if (pSc->totalLength[1] < ddwCarryCheck) {
			pSc->totalLength[0]++;
		}

		SHA512Guts(pSc, pData);
		nNeedBurn = 1;

		pData += 128L;
		nInLen -= 128L;
	}

	if (nInLen)
	{
		memcpy(&pSc->buffer.bytes[pSc->bufferLength], pData, nInLen);

		ddwCarryCheck = pSc->totalLength[1];
		pSc->totalLength[1] += nInLen * 8L;
		if (pSc->totalLength[1] < ddwCarryCheck) {
			pSc->totalLength[0]++;
		}

		pSc->bufferLength += nInLen;
	}
#else /* SHA512_FAST_COPY */
	while (nInLen)
	{
		dwBufferBytesLeft = 128L - pSc->bufferLength;

		dwBytesToCopy = dwBufferBytesLeft;
		if (dwBytesToCopy > nInLen) {
			dwBytesToCopy = nInLen;
		}

		memcpy(&pSc->buffer.bytes[pSc->bufferLength], pData, dwBytesToCopy);

		ddwCarryCheck = pSc->totalLength[1];
		pSc->totalLength[1] += dwBytesToCopy * 8L;
		if (pSc->totalLength[1] < ddwCarryCheck) {
			pSc->totalLength[0]++;
		}

		pSc->bufferLength += dwBytesToCopy;
		pData += dwBytesToCopy;
		nInLen -= dwBytesToCopy;

		if (pSc->bufferLength == 128L) {
			SHA512Guts(pSc, pSc->buffer.words);
			nNeedBurn = 1;
			pSc->bufferLength = 0L;
		}
	}
#endif /* SHA512_FAST_COPY */

	if (nNeedBurn)
	{
		burnStack(sizeof (UINT64[90]) + sizeof (UINT64 *[6]) + sizeof (int));
	}
	return 0;
}


static INT32 SHA512Guts(SHA512Context *pSc, const UINT64 *pddwBuf)
{
	const UINT64 *Kp;
	UINT64 aBuf[80] = {0,};
	UINT64 *W, *W2, *W7, *W15, *W16;
	UINT64 a, b, c, d, e, f, g, h;
	UINT64 t1, t2;
	int i;

	if(pSc == NULL || pddwBuf == NULL)
		return -1;

	W = aBuf;

	for (i = 15; i >= 0; i--) {
		*(W++) = BYTESWAP64(*pddwBuf);
		pddwBuf++;
	}

	W16 = &aBuf[0];
	W15 = &aBuf[1];
	W7 = &aBuf[9];
	W2 = &aBuf[14];

	for (i = 63; i >= 0; i--)
	{
		*(W++) = sigma1(*W2) + *(W7++) + sigma0(*W15) + *(W16++);
		W2++;
		W15++;
	}

	a = pSc->hash[0];
	b = pSc->hash[1];
	c = pSc->hash[2];
	d = pSc->hash[3];
	e = pSc->hash[4];
	f = pSc->hash[5];
	g = pSc->hash[6];
	h = pSc->hash[7];

	Kp = K;
	W = aBuf;

	for (i = 79; i >= 0; i--) {
		DO_ROUND();
	}

	pSc->hash[0] += a;
	pSc->hash[1] += b;
	pSc->hash[2] += c;
	pSc->hash[3] += d;
	pSc->hash[4] += e;
	pSc->hash[5] += f;
	pSc->hash[6] += g;
	pSc->hash[7] += h;
	return 0;
}

#ifndef _AS_SHA1_H_
static void burnStack(int size)
{
	char * buf[128] = {0,};

	size -= sizeof(buf);
	if (size > 0) {
		burnStack(size);
	}
}
#endif

INT32 SHA512Final(SHA512Context *pSc, UINT8 abHash[AS_SHA512_DIGEST_SIZE])
{
	UINT32 dwBytesToPad = 0;
	UINT64 ddwLengthPad[2] = {0,};
	INT32 i = 0;
	INT32 nRetVal = 0;

	if(pSc == NULL)
		return -1;


	dwBytesToPad = 240L - pSc->bufferLength;
	if (dwBytesToPad > 128L)
	{
		dwBytesToPad -= 128L;
	}

	ddwLengthPad[0] = BYTESWAP64(pSc->totalLength[0]);
	ddwLengthPad[1] = BYTESWAP64(pSc->totalLength[1]);

	nRetVal = SHA512Update(pSc, padding, dwBytesToPad);
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}
	nRetVal = SHA512Update(pSc, ddwLengthPad, 16L);
	if(nRetVal < 0)
	{
		nRetVal -= 20;
		return nRetVal;
	}

	if (abHash)
	{
		for (i = 0; i < AS_SHA512_HASH_WORDS; i++) {
#ifdef SHA384_FAST_COPY
			*((UINT64 *) abHash) = BYTESWAP64(pSc->hash[i]);
#else
			abHash[0] = (UINT8) (pSc->hash[i] >> 56);
			abHash[1] = (UINT8) (pSc->hash[i] >> 48);
			abHash[2] = (UINT8) (pSc->hash[i] >> 40);
			abHash[3] = (UINT8) (pSc->hash[i] >> 32);
			abHash[4] = (UINT8) (pSc->hash[i] >> 24);
			abHash[5] = (UINT8) (pSc->hash[i] >> 16);
			abHash[6] = (UINT8) (pSc->hash[i] >> 8);
			abHash[7] = (UINT8) pSc->hash[i];
#endif
			abHash += 8;
		}
	}
	return 0;
}
