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

#ifndef _AS_SHA256_H_
#define _AS_SHA256_H_

#define ASI_SHA256_ENCODE_TO_STRING		1
#define ASI_SHA256_ENCODE_TO_FILE		2

#ifndef ASI_SHA256_DIGEST_SIZE
#define ASI_SHA256_DIGEST_SIZE			32
#endif

#ifndef ASI_SHA256_HASH_SIZE
#define ASI_SHA256_HASH_SIZE			64
#endif

// Signed variables are for wimps
typedef unsigned char BYTE, *LPBYTE, *PBYTE;
typedef unsigned int UINT32, *PUINT32;

// DBL_INT_ADD treats two unsigned ints a and b as one 64-bit integer and adds c to it
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef struct
{
	BYTE data[ASI_SHA256_HASH_SIZE];
	UINT32 datalen;
	UINT32 bitlen[2];
	UINT32 state[8];
} SHA256_CTX;

extern int get_sha256_digest(char *str, int str_len, unsigned char *digest, int digest_len);
extern int get_sha256_hash(char *str, int str_len, char *hash, int hash_len);
extern int get_sha256_hash_from_file(char *path, char *hash, int hash_len);
extern int comp_sha256_digest(unsigned char *digest1, unsigned char *digest2);
extern int conv_sha256_digest(unsigned char *digest, char *hash, int hash_len);


#endif /* _AS_SHA256_H_ */


