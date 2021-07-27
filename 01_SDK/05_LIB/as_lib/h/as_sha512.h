/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public 
 * License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 *
 */
#ifndef _AS_SHA512_H__
#define _AS_SHA512_H__

#include <stdint.h>

#define AS_SHA512_DIGEST_SIZE		64
#define AS_SHA512_HASH_SIZE			128
#define AS_SHA512_HASH_WORDS		8

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))
#define ROTL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))
#define ROTR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))

#define Ch512(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj512(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define SIGMA0(x) (ROTR64((x), 28) ^ ROTR64((x), 34) ^ ROTR64((x), 39))
#define SIGMA1(x) (ROTR64((x), 14) ^ ROTR64((x), 18) ^ ROTR64((x), 41))
#define sigma0(x) (ROTR64((x), 1) ^ ROTR64((x), 8) ^ ((x) >> 7))
#define sigma1(x) (ROTR64((x), 19) ^ ROTR64((x), 61) ^ ((x) >> 6))

#define DO_ROUND() { \
	t1 = h + SIGMA1(e) + Ch512(e, f, g) + *(Kp++) + *(W++); \
	t2 = SIGMA0(a) + Maj512(a, b, c); \
	h = g; \
	g = f; \
	f = e; \
	e = d + t1; \
	d = c; \
	c = b; \
	b = a; \
	a = t1 + t2; \
}


typedef struct _SHA512Context {
	UINT64 totalLength[2];
	UINT64 hash[AS_SHA512_HASH_WORDS];
	UINT32 bufferLength;

	union {
		UINT64 words[16];
		UINT8  bytes[128];
	} buffer;

	int littleEndian;
} SHA512Context;

extern INT32 get_sha512_digest(char *pStr, int nStrLen, unsigned char *pDigest, int nDigestLen);
extern INT32 get_sha512_hash(char *pStr, int nStrLen, char *pcHash, int nHashLen);
extern INT32 get_sha512_hash_from_file(char *pPath, char *pHash, int nHashLen, off_t max_read_byte = 0);
extern INT32 comp_sha512_digest(unsigned char *digest1, unsigned char *digest2);
extern INT32 conv_sha512_digest(unsigned char *pDigest, char *pHash, int nHashLen);

#endif	/* _AS_SHA512_H__ */


