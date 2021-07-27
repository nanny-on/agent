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
 * 
 */


#if HAVE_INTTYPES_H
# include <inttypes.h>
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#include "stdafx.h"
#include "as_string.h"
#include "as_sha1.h"

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define F_0_19(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define F_20_39(x, y, z) ((x) ^ (y) ^ (z))
#define F_40_59(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define F_60_79(x, y, z) ((x) ^ (y) ^ (z))

#define DO_ROUND(F, K) { \
  temp = ROTL(a, 5) + F(b, c, d) + e + *(W++) + K; \
  e = d; \
  d = c; \
  c = ROTL(b, 30); \
  b = a; \
  a = temp; \
}

#define K_0_19 0x5a827999L
#define K_20_39 0x6ed9eba1L
#define K_40_59 0x8f1bbcdcL
#define K_60_79 0xca62c1d6L

#if defined(asi_BIG_ENDIAN)
#define BYTESWAP(x) (x)
#define BYTESWAP64(x) (x)
#else
#define BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
		     (ROTL((x), 8) & 0x00ff00ffL))
#define BYTESWAP64(x) _byteswap64(x)

static inline uint64_t _byteswap64(uint64_t x)
{
  uint32_t a = x >> 32;
  uint32_t b = (uint32_t) x;
  return ((uint64_t) BYTESWAP(b) << 32) | (uint64_t) BYTESWAP(a);
}
#endif /* asi_BIG_ENDIAN */


static const uint8_t padding[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void SHA1Init (SHA1Context *sc)
{
  sc->totalLength = 0LL;
  sc->hash[0] = 0x67452301L;
  sc->hash[1] = 0xefcdab89L;
  sc->hash[2] = 0x98badcfeL;
  sc->hash[3] = 0x10325476L;
  sc->hash[4] = 0xc3d2e1f0L;
  sc->bufferLength = 0L;
}

static void burnStack (int size)
{
  char buf[128] = { 0,};

  memset (buf, 0, sizeof (buf));
  size -= sizeof (buf);
  if (size > 0)
    burnStack (size);
}

static void SHA1Guts (SHA1Context *sc, const uint32_t *cbuf)
{
  uint32_t buf[80];
  uint32_t *W, *W3, *W8, *W14, *W16;
  uint32_t a, b, c, d, e, temp;
  int i;

  W = buf;

  for (i = 15; i >= 0; i--) {
    *(W++) = BYTESWAP(*cbuf);
    cbuf++;
  }

  W16 = &buf[0];
  W14 = &buf[2];
  W8 = &buf[8];
  W3 = &buf[13];

  for (i = 63; i >= 0; i--) {
    *W = *(W3++) ^ *(W8++) ^ *(W14++) ^ *(W16++);
    *W = ROTL(*W, 1);
    W++;
  }

  a = sc->hash[0];
  b = sc->hash[1];
  c = sc->hash[2];
  d = sc->hash[3];
  e = sc->hash[4];

  W = buf;

#ifndef SHA1_UNROLL
#define SHA1_UNROLL 20
#endif /* !SHA1_UNROLL */

#if SHA1_UNROLL == 1
  for (i = 19; i >= 0; i--)
    DO_ROUND(F_0_19, K_0_19);

  for (i = 19; i >= 0; i--)
    DO_ROUND(F_20_39, K_20_39);

  for (i = 19; i >= 0; i--)
    DO_ROUND(F_40_59, K_40_59);

  for (i = 19; i >= 0; i--)
    DO_ROUND(F_60_79, K_60_79);
#elif SHA1_UNROLL == 2
  for (i = 9; i >= 0; i--) {
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
  }

  for (i = 9; i >= 0; i--) {
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
  }

  for (i = 9; i >= 0; i--) {
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
  }

  for (i = 9; i >= 0; i--) {
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
  }
#elif SHA1_UNROLL == 4
  for (i = 4; i >= 0; i--) {
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
  }

  for (i = 4; i >= 0; i--) {
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
  }

  for (i = 4; i >= 0; i--) {
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
  }

  for (i = 4; i >= 0; i--) {
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
  }
#elif SHA1_UNROLL == 5
  for (i = 3; i >= 0; i--) {
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
  }

  for (i = 3; i >= 0; i--) {
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
  }

  for (i = 3; i >= 0; i--) {
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
  }

  for (i = 3; i >= 0; i--) {
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
  }
#elif SHA1_UNROLL == 10
  for (i = 1; i >= 0; i--) {
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
    DO_ROUND(F_0_19, K_0_19);
  }

  for (i = 1; i >= 0; i--) {
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
    DO_ROUND(F_20_39, K_20_39);
  }

  for (i = 1; i >= 0; i--) {
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
    DO_ROUND(F_40_59, K_40_59);
  }

  for (i = 1; i >= 0; i--) {
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
    DO_ROUND(F_60_79, K_60_79);
  }
#elif SHA1_UNROLL == 20
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);
  DO_ROUND(F_0_19, K_0_19);

  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);
  DO_ROUND(F_20_39, K_20_39);

  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);
  DO_ROUND(F_40_59, K_40_59);

  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
  DO_ROUND(F_60_79, K_60_79);
#else /* SHA1_UNROLL */
#error SHA1_UNROLL must be 1, 2, 4, 5, 10 or 20!
#endif

  sc->hash[0] += a;
  sc->hash[1] += b;
  sc->hash[2] += c;
  sc->hash[3] += d;
  sc->hash[4] += e;
}

void SHA1Update (SHA1Context *sc, const void *vdata, uint32_t len)
{
  const uint8_t *data = (uint8_t *) vdata;
  uint32_t bufferBytesLeft;
  uint32_t bytesToCopy;
  int needBurn = 0;

#ifdef SHA1_FAST_COPY
  if (sc->bufferLength) {
    bufferBytesLeft = 64L - sc->bufferLength;

    bytesToCopy = bufferBytesLeft;
    if (bytesToCopy > len)
      bytesToCopy = len;

    memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

    sc->totalLength += bytesToCopy * 8L;

    sc->bufferLength += bytesToCopy;
    data += bytesToCopy;
    len -= bytesToCopy;

    if (sc->bufferLength == 64L) {
      SHA1Guts (sc, sc->buffer.words);
      needBurn = 1;
      sc->bufferLength = 0L;
    }
  }

  while (len > 63) {
    sc->totalLength += 512L;

    SHA1Guts (sc, data);
    needBurn = 1;

    data += 64L;
    len -= 64L;
  }

  if (len) {
    memcpy (&sc->buffer.bytes[sc->bufferLength], data, len);

    sc->totalLength += len * 8L;

    sc->bufferLength += len;
  }
#else /* SHA1_FAST_COPY */
  while (len) {
    bufferBytesLeft = 64L - sc->bufferLength;

    bytesToCopy = bufferBytesLeft;
    if (bytesToCopy > len)
      bytesToCopy = len;

    memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

    sc->totalLength += bytesToCopy * 8L;

    sc->bufferLength += bytesToCopy;
    data += bytesToCopy;
    len -= bytesToCopy;

    if (sc->bufferLength == 64L) {
      SHA1Guts (sc, sc->buffer.words);
      needBurn = 1;
      sc->bufferLength = 0L;
    }
  }
#endif /* SHA1_FAST_COPY */

  if (needBurn)
    burnStack (sizeof (uint32_t[86]) + sizeof (uint32_t *[5]) + sizeof (int));
}

void SHA1Final (SHA1Context *sc, uint8_t hash[SHA1_DIGEST_SIZE])
{
  uint32_t bytesToPad;
  uint64_t lengthPad;
  int i;

  bytesToPad = 120L - sc->bufferLength;
  if (bytesToPad > 64L)
    bytesToPad -= 64L;

  lengthPad = BYTESWAP64(sc->totalLength);

  SHA1Update (sc, padding, bytesToPad);
  SHA1Update (sc, &lengthPad, 8L);

  if (hash) {
    for (i = 0; i < SHA1_HASH_WORDS; i++) {
#ifdef SHA1_FAST_COPY
      *((uint32_t *) hash) = BYTESWAP(sc->hash[i]);
#else /* SHA1_FAST_COPY */
      hash[0] = (uint8_t) (sc->hash[i] >> 24);
      hash[1] = (uint8_t) (sc->hash[i] >> 16);
      hash[2] = (uint8_t) (sc->hash[i] >> 8);
      hash[3] = (uint8_t) sc->hash[i];
#endif /* SHA1_FAST_COPY */
      hash += 4;
    }
  }
}

int get_sha1_digest(char *str, int str_len, unsigned char *digest, int digest_len)
{
	SHA1Context    ctx;
	if(str == NULL || str_len < 1 || digest == NULL || digest_len < SHA1_DIGEST_SIZE)
	{
		return -1;
	}
	memset(digest, 0, digest_len);

	SHA1Init(&ctx);
	SHA1Update(&ctx, str, str_len);
	SHA1Final(&ctx, digest);

	return 0;
}


void get_sha1_hash(char *str, int str_len, char *hash, int hash_len)
{
	int            i;
	char           tmp[4] = { 0,};
	unsigned char  digest[SHA1_DIGEST_SIZE] = { 0,};
	SHA1Context    ctx;

	memset(hash, 0x00, hash_len);
	memset(digest, 0x00, sizeof(digest));

	SHA1Init(&ctx);
	SHA1Update(&ctx, str, str_len);
	SHA1Final(&ctx, digest);

	for (i = 0; i < SHA1_DIGEST_SIZE; i++)
	{
		snprintf(tmp, sizeof(tmp), "%02X", digest[i]);
		strlcat(hash, tmp, hash_len);
	}
}


int get_sha1_hash_from_file(char *path, char *hash, int hash_len)
{
	FILE           *fp = NULL;
	int            i, nread;
	char           tmp[4] = { 0,};
	char           buf[4096] = { 0,};
	unsigned char  digest[SHA1_DIGEST_SIZE] = { 0,};
	SHA1Context    ctx;

	memset(hash, 0x00, hash_len);
	memset(digest, 0x00, sizeof(digest));

	if ( (fp = fopen(path, "rb")) == NULL) {
		return -1;
	}

	SHA1Init(&ctx);

	while ( (nread = fread(buf, 1, sizeof(buf), fp)) != 0)
	{
		SHA1Update(&ctx, buf, nread);
	}

	SHA1Final(&ctx, digest);

	for (i = 0; i < SHA1_DIGEST_SIZE; i++)
	{
		snprintf(tmp, sizeof(tmp), "%02X", digest[i]);
		strlcat(hash, tmp, hash_len);
	}

	fclose(fp);
	return 0;
}


