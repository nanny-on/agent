/*
 * Copyright (C) 2021-2026 ASHINi Corp.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version, with the additional
 * special exception to link portions of this program with the OpenSSL
 * library. See LICENSE for more details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.  
 * 
 *
 */
#include "stdafx.h"
#include "as_sha256.h"

#define TRUE 1
#define FALSE 0

UINT32 k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void sha256_transform(SHA256_CTX *ctx, BYTE data[])
{
	UINT32 a,b,c,d,e,f,g,h,i,j,t1,t2,m[64];
	if(ctx == NULL || data == NULL)
		return;

	for (i=0,j=0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j+1] << 16) | (data[j+2] << 8) | (data[j+3]);

	for ( ; i < 64; ++i)
		m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i)
	{
		t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a,b,c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
	if(ctx != NULL)
	{
		ctx->datalen = 0;
		ctx->bitlen[0] = 0;
		ctx->bitlen[1] = 0;
		ctx->state[0] = 0x6a09e667;
		ctx->state[1] = 0xbb67ae85;
		ctx->state[2] = 0x3c6ef372;
		ctx->state[3] = 0xa54ff53a;
		ctx->state[4] = 0x510e527f;
		ctx->state[5] = 0x9b05688c;
		ctx->state[6] = 0x1f83d9ab;
		ctx->state[7] = 0x5be0cd19;
	}
}

int sha256_update(SHA256_CTX *ctx, BYTE data[], UINT32 len)
{
	UINT32 i = 0;
	if(ctx == NULL || data == NULL || len < 1)
	{
		return -1;
	}
	for (i=0; i < len; ++i)
	{
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64)
		{
			sha256_transform(ctx,ctx->data);
			DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],512);
			ctx->datalen = 0;
		}
	}
	return 0;
}

int sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
	UINT32 i = 0;

	if(ctx == NULL || hash == NULL)
	{
		return -1;
	}

	i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56)
	{
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else
	{
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		sha256_transform(ctx,ctx->data);
		memset(ctx->data,0,56);
	}

	// Append to the padding the total message's length in bits and transform.
	DBL_INT_ADD(ctx->bitlen[0],ctx->bitlen[1],ctx->datalen * 8);
	ctx->data[63] = ctx->bitlen[0];
	ctx->data[62] = ctx->bitlen[0] >> 8;
	ctx->data[61] = ctx->bitlen[0] >> 16;
	ctx->data[60] = ctx->bitlen[0] >> 24;
	ctx->data[59] = ctx->bitlen[1];
	ctx->data[58] = ctx->bitlen[1] >> 8;
	ctx->data[57] = ctx->bitlen[1] >> 16;
	ctx->data[56] = ctx->bitlen[1] >> 24;
	sha256_transform(ctx,ctx->data);

	// Since this implementation uses little endian byte ordering and SHA uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i=0; i < 4; ++i)
	{
		hash[i]    = (ctx->state[0] >> (24-i*8)) & 0x000000ff;
		hash[i+4]  = (ctx->state[1] >> (24-i*8)) & 0x000000ff;
		hash[i+8]  = (ctx->state[2] >> (24-i*8)) & 0x000000ff;
		hash[i+12] = (ctx->state[3] >> (24-i*8)) & 0x000000ff;
		hash[i+16] = (ctx->state[4] >> (24-i*8)) & 0x000000ff;
		hash[i+20] = (ctx->state[5] >> (24-i*8)) & 0x000000ff;
		hash[i+24] = (ctx->state[6] >> (24-i*8)) & 0x000000ff;
		hash[i+28] = (ctx->state[7] >> (24-i*8)) & 0x000000ff;
	}
	return 0;
}

/* hash1, hash2 digest 값을 비교하여 같으면 0, 틀리면 -1 리턴 */
int comp_sha256_digest(unsigned char *digest1, unsigned char *digest2)
{
	int  idx = 32;

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


/* 입력된 hash digest를 문자열로 변환 */
int conv_sha256_digest(unsigned char *digest, char *hash, int hash_len)
{
	int i;
	char tmp[4] = {0,};

	if(digest == NULL || hash == NULL || hash_len < ASI_SHA256_HASH_SIZE+1)
	{
		return -1;
	}

	memset(hash, 0x00, hash_len);

	for (i=0; i<ASI_SHA256_DIGEST_SIZE; i++)
	{
		snprintf(tmp, 3, "%02X", digest[i]);
		tmp[3] = 0;
		if(i==0)
			strncpy(hash, tmp, 2);
		else
			strncat(hash, tmp, 2);
	}
	hash[ASI_SHA256_HASH_SIZE] = 0;
	return 0;
}



/* 입력된 문자열의 sha256 digest 값을 구함 */
int get_sha256_digest(char *str, int str_len, unsigned char *digest, int digest_len)
{
	int nRetVal = 0;
	SHA256_CTX  ctx;
	if(str == NULL || str_len < 1 || digest == NULL || digest_len < ASI_SHA256_DIGEST_SIZE)
	{
		return -1;
	}
	memset(digest, 0, digest_len);

	sha256_init(&ctx);
	nRetVal = sha256_update(&ctx, (unsigned char *)str, str_len);
	if(nRetVal != 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}
	nRetVal = sha256_final(&ctx, digest);
	if(nRetVal != 0)
	{
		nRetVal -= 20;
		return nRetVal;
	}
	return 0;
}

int get_sha256_hash(char *str, int str_len, char *hash, int hash_len)
{
	int nRetVal = 0;
	unsigned char acDigest[ASI_SHA256_DIGEST_SIZE+1] = {0,};
	SHA256_CTX ctx;

	if(str == NULL || str_len < 1 || hash == NULL || hash_len < ASI_SHA256_HASH_SIZE+1)
	{
		return -1;
	}
	memset(hash, 0x00, hash_len);
	do{
		sha256_init(&ctx);
		nRetVal = sha256_update(&ctx, (unsigned char *)str, str_len);
		if(nRetVal != 0)
		{
			nRetVal -= 10;
			break;
		}

		nRetVal = sha256_final(&ctx, acDigest);
		if(nRetVal != 0)
		{
			nRetVal -= 20;
			break;
		}

		acDigest[ASI_SHA256_DIGEST_SIZE] = 0;

		nRetVal = conv_sha256_digest(acDigest, hash, hash_len);
		if(nRetVal != 0)
		{
			nRetVal -= 30;
			break;
		}
		nRetVal = 0;
	}while(FALSE);
	return nRetVal;
}


/* 특정 파일의 hash 값을 구함 */
int get_sha256_hash_from_file(char *path, char *hash, int hash_len)
{
	FILE *fp = NULL;
	int nRead = 0;
	int nRetVal = 0;
	char acBuf[4096] = { 0,};
	unsigned char acDigest[ASI_SHA256_DIGEST_SIZE+1] = {0,};
	SHA256_CTX ctx;

	if(path == NULL || hash == NULL || hash_len < ASI_SHA256_HASH_SIZE+1)
	{
		return -1;
	}

	memset(hash, 0x00, hash_len);

	fp = fopen(path, "r");
	if (fp == NULL)
	{
		return -2;
	}
	sha256_init(&ctx);
	while((nRead = fread(acBuf, 1, sizeof(acBuf), fp)) > 0)
	{
		nRetVal = sha256_update(&ctx, (unsigned char *)acBuf, nRead);
		if(nRetVal != 0)
		{
			fclose(fp);
			nRetVal -= 10;
			return nRetVal;
		}
	}

	nRetVal = sha256_final(&ctx, acDigest);
	if(nRetVal != 0)
	{
		fclose(fp);
		nRetVal -= 20;
		return nRetVal;
	}

	nRetVal = conv_sha256_digest(acDigest, hash, hash_len);
	if(nRetVal != 0)
	{
		fclose(fp);
		nRetVal -= 30;
		return nRetVal;
	}

	fclose(fp);
	return 0;
}