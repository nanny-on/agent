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

#ifndef _AS_SHA1_H_
#define _AS_SHA1_H_

#ifdef  ASI_BIG_ENDIAN
#undef	ASI_BIG_ENDIAN
#endif

#include <stdint.h>

#define SHA1_DIGEST_SIZE	20
#define SHA1_HASH_SIZE		40	

/* Hash size in 32-bit words */
#define SHA1_HASH_WORDS 5

// Basic MD5 functions.
#define F( x, y, z ) ((( x ) & ( y )) | (( ~x ) & ( z )))
#define G( x, y, z ) ((( x ) & ( z )) | (( y ) & ( ~z )))
#define H( x, y, z ) (( x ) ^ ( y ) ^ ( z ))
#define I( x, y, z ) (( y ) ^ (( x ) | ( ~z )))

#define	HASH_SUCCESS	1;
#define	ERRHASH_NOTSUPPORTEDALGORITHM	-1
#define ERRHASH_NOTHEXDIGIT             -2
#define ERRHASH_MEMORYALLOCATION        -3


//////////////////////// for SHA1 /////////////////////


// ROTATE_LEFT rotates x left n bits.
#define ROTATE_LEFT( x, n ) ((( x ) << ( n )) | ( ( x ) >> ( 32-( n ))))

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
#define FF( a, b, c, d, x, s, ac ) \
{ \
 (a) += F ( (b), (c), (d) ) + (x) + (ULONG)(ac); \
 (a) = ROTATE_LEFT( (a), (s) ); \
 (a) += (b); \
}

#define GG( a, b, c, d, x, s, ac ) \
{ \
 (a) += G( (b), (c), (d) ) + (x) + (ULONG)(ac); \
 (a) = ROTATE_LEFT( (a), (s) ); \
 (a) += (b); \
}

#define HH( a, b, c, d, x, s, ac ) \
{ \
 (a) += H( (b), (c), (d) ) + (x) + (ULONG)(ac); \
 (a) = ROTATE_LEFT( (a), (s) ); \
 (a) += (b); \
}

#define II( a, b, c, d, x, s, ac ) \
{ \
 (a) += I( (b), (c), (d) ) + (x) + (ULONG)(ac); \
 (a) = ROTATE_LEFT( (a), (s) ); \
 (a) += (b); \
}

#define rol( value, bits ) (((value) << (bits)) | ((value) >> (32 - (bits))))

#ifdef ASI_BIG_ENDIAN
#define blk0(i) block->l[i]
#else
#define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
		|(rol(block->l[i],8)&0x00FF00FF))
#endif


#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
		^block->l[(i+2)&15]^block->l[i&15],1))

#define _R0( v,w,x,y,z,i ) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5); \
		w=rol(w,30);
#define _R1( v,w,x,y,z,i ) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5); \
		w=rol(w,30);
#define _R2( v,w,x,y,z,i ) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define _R3( v,w,x,y,z,i ) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5); \
		w=rol(w,30);
#define _R4( v,w,x,y,z,i ) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

typedef struct
{
    UINT state[5];
    UINT count[2];
    UCHAR buffer[64];
} SHA1_CTX;

typedef struct _SHA1Context {
	uint64_t totalLength;
	uint32_t hash[SHA1_HASH_WORDS];
	uint32_t bufferLength;
	union {
		uint32_t words[16];
		uint8_t bytes[64];
	} buffer;
#ifdef RUNTIME_ENDIAN
	int littleEndian;
#endif /* RUNTIME_ENDIAN */
}SHA1Context;

extern void SHA1Init( SHA1_CTX* context );
extern void SHA1Update( SHA1_CTX* context, UCHAR *data, UINT len );
extern void SHA1Final( UCHAR *digest, SHA1_CTX* context );
extern void SHA1Digest(LPBYTE pcData, LPBYTE pcDigest, UINT uiLength);
extern int get_sha1_digest(char *str, int str_len, unsigned char *digest, int digest_len);
extern void get_sha1_hash(char *str, int str_len, char *hash, int hash_len);
extern int get_sha1_hash_from_file(char *path, char *hash, int hash_len);


#endif	/* _AS_SHA1_H_ */


