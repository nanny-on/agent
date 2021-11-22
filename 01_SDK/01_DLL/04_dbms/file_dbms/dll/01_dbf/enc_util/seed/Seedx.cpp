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
 */

#include "stdafx.h"
#include "seedx.h"
#include "seedx.tab"
//---------------------------------------------------------------------------

/******************* Encryption/Decryption *******************/
 

#define GetB0(A)  ( (BYTE)((A)    ) )
#define GetB1(A)  ( (BYTE)((A)>> 8) )
#define GetB2(A)  ( (BYTE)((A)>>16) )
#define GetB3(A)  ( (BYTE)((A)>>24) )

#define SeedRound(L0, L1, R0, R1, K) {             \
    T0 = R0 ^ (K)[0];                              \
    T1 = R1 ^ (K)[1];                              \
    T1 ^= T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    T0 = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^         \
         SS2[GetB2(T0)] ^ SS3[GetB3(T0)];          \
    T1 += T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    L0 ^= T0; L1 ^= T1;                            \
}


#define EndianChange(dwS)                       \
    ( (ROTL((dwS),  8) & (DWORD)0x00ff00ff) |   \
      (ROTL((dwS), 24) & (DWORD)0xff00ff00) )


/************************ Block Encryption *************************/

void SeedEncrypt(BYTE *pbData, DWORD *pdwRoundKey)
{
	DWORD L0, L1, R0, R1, T0, T1, *K = pdwRoundKey;

    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
	R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

    SeedRound(L0, L1, R0, R1, K   ); /*   1 */
    SeedRound(R0, R1, L0, L1, K+ 2); /*   2 */
    SeedRound(L0, L1, R0, R1, K+ 4); /*   3 */
    SeedRound(R0, R1, L0, L1, K+ 6); /*   4 */
    SeedRound(L0, L1, R0, R1, K+ 8); /*   5 */
    SeedRound(R0, R1, L0, L1, K+10); /*   6 */
    SeedRound(L0, L1, R0, R1, K+12); /*   7 */
    SeedRound(R0, R1, L0, L1, K+14); /*   8 */
    SeedRound(L0, L1, R0, R1, K+16); /*   9 */
    SeedRound(R0, R1, L0, L1, K+18); /*  10 */
    SeedRound(L0, L1, R0, R1, K+20); /*  11 */
    SeedRound(R0, R1, L0, L1, K+22); /*  12 */
#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+24); /*  13 */
    SeedRound(R0, R1, L0, L1, K+26); /*  14 */
    SeedRound(L0, L1, R0, R1, K+28); /*  15 */
    SeedRound(R0, R1, L0, L1, K+30); /*  16 */
#endif

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}


/* same as encrypt, except that round keys are applied in reverse order. */
void SeedDecrypt(BYTE *pbData, DWORD *pdwRoundKey)
{
    DWORD L0, L1, R0, R1, T0, T1, *K=pdwRoundKey;

    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
    R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

#if NoRounds == 16
    SeedRound(L0, L1, R0, R1, K+30); /*   1 */
    SeedRound(R0, R1, L0, L1, K+28); /*   2 */
    SeedRound(L0, L1, R0, R1, K+26); /*   3 */
    SeedRound(R0, R1, L0, L1, K+24); /*   4 */
#endif
    SeedRound(L0, L1, R0, R1, K+22); /*   5 */
    SeedRound(R0, R1, L0, L1, K+20); /*   6 */
    SeedRound(L0, L1, R0, R1, K+18); /*   7 */
    SeedRound(R0, R1, L0, L1, K+16); /*   8 */
    SeedRound(L0, L1, R0, R1, K+14); /*   9 */
    SeedRound(R0, R1, L0, L1, K+12); /*  10 */
    SeedRound(L0, L1, R0, R1, K+10); /*  11 */
    SeedRound(R0, R1, L0, L1, K+ 8); /*  12 */
    SeedRound(L0, L1, R0, R1, K+ 6); /*  13 */
    SeedRound(R0, R1, L0, L1, K+ 4); /*  14 */
    SeedRound(L0, L1, R0, R1, K+ 2); /*  15 */
    SeedRound(R0, R1, L0, L1, K+ 0); /*  16 */

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}



/******************** Key Scheduling **********************/

/* Constants for key schedule:
KC0 = golden ratio; KCi = ROTL(KCi-1, 1) */
#define KC0     0x9e3779b9UL
#define KC1     0x3c6ef373UL
#define KC2     0x78dde6e6UL
#define KC3     0xf1bbcdccUL
#define KC4     0xe3779b99UL
#define KC5     0xc6ef3733UL
#define KC6     0x8dde6e67UL
#define KC7     0x1bbcdccfUL
#define KC8     0x3779b99eUL
#define KC9     0x6ef3733cUL
#define KC10    0xdde6e678UL
#define KC11    0xbbcdccf1UL
#if NoRounds == 16
#define KC12    0x779b99e3UL
#define KC13    0xef3733c6UL
#define KC14    0xde6e678dUL
#define KC15    0xbcdccf1bUL
#endif

#define EncRoundKeyUpdate0(K, A, B, C, D, KC) {  \
    T0 = A;                                      \
    A = (A>>8) ^ (B<<24);                        \
    B = (B>>8) ^ (T0<<24);                       \
    T0 = A + C - KC;                             \
    T1 = B + KC - D;                             \
    (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^   \
             SS2[GetB2(T0)] ^ SS3[GetB3(T0)];    \
    (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^   \
             SS2[GetB2(T1)] ^ SS3[GetB3(T1)];    \
}


#define EncRoundKeyUpdate1(K, A, B, C, D, KC) {  \
    T0 = C;                                      \
    C = (C<<8) ^ (D>>24);                        \
    D = (D<<8) ^ (T0>>24);                       \
    T0 = A + C - KC;                             \
    T1 = B + KC - D;                             \
    (K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^   \
             SS2[GetB2(T0)] ^ SS3[GetB3(T0)];    \
    (K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^   \
             SS2[GetB2(T1)] ^ SS3[GetB3(T1)];    \
}


/* Encryption key schedule */
void SeedEncRoundKey(DWORD *pdwRoundKey, BYTE *pbUserKey)
{
  DWORD A, B, C, D, T0, T1, *K=pdwRoundKey;

  A = ((DWORD *)pbUserKey)[0];
  B = ((DWORD *)pbUserKey)[1];
  C = ((DWORD *)pbUserKey)[2];
  D = ((DWORD *)pbUserKey)[3];
#ifndef BIG_ENDIAN
  A = EndianChange(A);
  B = EndianChange(B);
  C = EndianChange(C);
  D = EndianChange(D);
#endif

    T0 = A + C - KC0;
    T1 = B - D + KC0;
    K[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^
           SS2[GetB2(T0)] ^ SS3[GetB3(T0)];
    K[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^
           SS2[GetB2(T1)] ^ SS3[GetB3(T1)];

    EncRoundKeyUpdate0(K+ 2, A, B, C, D, KC1 );
    EncRoundKeyUpdate1(K+ 4, A, B, C, D, KC2 );
    EncRoundKeyUpdate0(K+ 6, A, B, C, D, KC3 );
    EncRoundKeyUpdate1(K+ 8, A, B, C, D, KC4 );
    EncRoundKeyUpdate0(K+10, A, B, C, D, KC5 );
    EncRoundKeyUpdate1(K+12, A, B, C, D, KC6 );
    EncRoundKeyUpdate0(K+14, A, B, C, D, KC7 );
    EncRoundKeyUpdate1(K+16, A, B, C, D, KC8 );
    EncRoundKeyUpdate0(K+18, A, B, C, D, KC9 );
    EncRoundKeyUpdate1(K+20, A, B, C, D, KC10);
    EncRoundKeyUpdate0(K+22, A, B, C, D, KC11);
#if NoRounds == 16
    EncRoundKeyUpdate1(K+24, A, B, C, D, KC12);
    EncRoundKeyUpdate0(K+26, A, B, C, D, KC13);
    EncRoundKeyUpdate1(K+28, A, B, C, D, KC14);
    EncRoundKeyUpdate0(K+30, A, B, C, D, KC15);
#endif
}

/************************ END ****************************/
