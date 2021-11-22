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

// SHA2Util.cpp: implementation of the CSHA2Util class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "SHA2Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*
*  Define the circular shift macro
*/
#define SHA256CircularShift(bits,word) \
	((((word) << (bits)) & 0xFFFFFFFF) | \
	((word) >> (32-(bits))))

#ifndef min
#define min( x, y ) ( ( x ) < ( y ) ? ( x ) : ( y ) ) 
#endif


#define S(x,n) ( ((x)>>(n)) | ((x)<<(32-(n))) ) 
#define R(x,n) ( (x)>>(n) ) 


#define Ch(x,y,z) ( ((x) & (y)) | (~(x) & (z)) ) 
#define Maj(x,y,z) ( ( (x) & (y) ) | ( (x) & (z) ) | ( (y) & (z) ) ) 


#define SIG0(x) ( S(x, 2) ^ S(x,13) ^ S(x,22) ) 
#define SIG1(x) ( S(x, 6) ^ S(x,11) ^ S(x,25) ) 
#define sig0(x) ( S(x, 7) ^ S(x,18) ^ R(x, 3) ) 
#define sig1(x) ( S(x,17) ^ S(x,19) ^ R(x,10) ) 

//////////////////////////////////////////////////////////////////////


CSHA2Util::CSHA2Util()
{
	
}

CSHA2Util::~CSHA2Util()
{
	
}


void	CSHA2Util::SHA256Reset()
{	
	tSha256Context.Length_Low             = 0;
    tSha256Context.Length_High            = 0;
    tSha256Context.Message_Block_Index    = 0;
	
    tSha256Context.Message_Digest[0]      = 0x6a09e667;
    tSha256Context.Message_Digest[1]      = 0xbb67ae85;
    tSha256Context.Message_Digest[2]      = 0x3c6ef372;
    tSha256Context.Message_Digest[3]      = 0xa54ff53a;
    tSha256Context.Message_Digest[4]      = 0x510e527f;
    tSha256Context.Message_Digest[5]      = 0x9b05688c;
    tSha256Context.Message_Digest[6]      = 0x1f83d9ab;
    tSha256Context.Message_Digest[7]      = 0x5be0cd19;
	
    tSha256Context.Computed   = 0;
    tSha256Context.Corrupted  = 0;
}

/*  
*  SHA256Result
*
*  Description:
*      This function will return the 160-bit message digest into the
*      Message_Digest array within the SHA256Context provided
*
*  Parameters:
*      context: [in/out]
*          The context to use to calculate the SHA-1 hash.
*
*  Returns:
*      1 if successful, 0 if it failed.
*
*  Comments:
*
*/

int		CSHA2Util::SHA256Result(LPSTR lpBuffer, DWORD dwLegth)
{
	if(lpBuffer == NULL || dwLegth < 33)
	{
        return -1;
	}
	
    if (tSha256Context.Corrupted)
    {
        return -1;
    }
	
    if (!tSha256Context.Computed)
    {
        SHA256PadMessage();
        tSha256Context.Computed = 1;
    }

	snprintf(lpBuffer, dwLegth-1, "%08X%08X%08X%08X%08X%08X%08X%08X",
								tSha256Context.Message_Digest[0],
								tSha256Context.Message_Digest[1],
								tSha256Context.Message_Digest[2],
								tSha256Context.Message_Digest[3],
								tSha256Context.Message_Digest[4],
								tSha256Context.Message_Digest[5],
								tSha256Context.Message_Digest[6],
								tSha256Context.Message_Digest[7]);

	lpBuffer[dwLegth-1] = 0;
    return 0;
}

int		CSHA2Util::SHA256ResultByte(PBYTE lpHash)
{	
    if (tSha256Context.Corrupted)
    {
        return -1;
    }
	
    if (!tSha256Context.Computed)
    {
        SHA256PadMessage();
        tSha256Context.Computed = 1;
    }

	memcpy(lpHash, tSha256Context.Message_Digest, sizeof(tSha256Context.Message_Digest));
	return 0;
}

/*  
*  SHA256Input
*
*  Description:
*      This function accepts an array of octets as the next portion of
*      the message.
*
*  Parameters:
*      context: [in/out]
*          The SHA-1 context to update
*      message_array: [in]
*          An array of characters representing the next portion of the
*          message.
*      length: [in]
*          The length of the message in message_array
*
*  Returns:
*      Nothing.
*
*  Comments:
*
*/
void	CSHA2Util::SHA256Input(LPCTSTR message_array, unsigned length)
{
    if (!length)
    {
        return;
    }
	
    if (tSha256Context.Computed || tSha256Context.Corrupted)
    {
        tSha256Context.Corrupted = 1;
        return;
    }
	
    while(length-- && !tSha256Context.Corrupted)
    {
        tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] =
			(*message_array & 0xFF);
		
        tSha256Context.Length_Low += 8;
        /* Force it to 32 bits */
        tSha256Context.Length_Low &= 0xFFFFFFFF;
        if (tSha256Context.Length_Low == 0)
        {
            tSha256Context.Length_High++;
            /* Force it to 32 bits */
            tSha256Context.Length_High &= 0xFFFFFFFF;
            if (tSha256Context.Length_High == 0)
            {
                /* Message is too long */
                tSha256Context.Corrupted = 1;
            }
        }
		
        if (tSha256Context.Message_Block_Index == 64)
        {
            SHA256ProcessMessageBlock();
        }
		
        message_array++;
    }
}

/*  
*  SHA256ProcessMessageBlock
*
*  Description:
*      This function will process the next 512 bits of the message
*      stored in the Message_Block array.
*
*  Parameters:
*      None.
*
*  Returns:
*      Nothing.
*
*  Comments:
*      Many of the variable names in the SHAContext, especially the
*      single character names, were used because those were the names
*      used in the publication.
*         
*
*/
void	CSHA2Util::SHA256ProcessMessageBlock()
{
	
    int         t;                  /* Loop counter                 */
    unsigned    T1,T2;               /* Temporary word value         */
    unsigned    W[80];              /* Word sequence                */
    unsigned    A, B, C, D, E, F, G, H;      /* Word buffers                 */
	
											 /*
											 *  Initialize the first 16 words in the array W
	*/
	const unsigned K[]  = { 
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
			0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
			0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
			0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
			0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
			0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
			0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
			0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
			0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 
	};
	
    for(t = 0; t < 16; t++)
    {
        W[t] = ((unsigned) tSha256Context.Message_Block[t * 4]) << 24;
        W[t] |= ((unsigned) tSha256Context.Message_Block[t * 4 + 1]) << 16;
        W[t] |= ((unsigned) tSha256Context.Message_Block[t * 4 + 2]) << 8;
        W[t] |= ((unsigned) tSha256Context.Message_Block[t * 4 + 3]);
    }
	
	for ( t = 16; t < 64; t++ ) 
	{ 
		W[ t ] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16]; 
	}
	
	
    A = tSha256Context.Message_Digest[0];
    B = tSha256Context.Message_Digest[1];
    C = tSha256Context.Message_Digest[2];
    D = tSha256Context.Message_Digest[3];
    E = tSha256Context.Message_Digest[4];
	F = tSha256Context.Message_Digest[5];
	G = tSha256Context.Message_Digest[6];
	H = tSha256Context.Message_Digest[7];
	for ( t = 0; t < 64; t++ ) 
	{
		
		
		T1 = H + SIG1(E) + Ch(E,F,G) + K[t] + W[t]; 
		T2 = SIG0(A) + Maj(A,B,C); 
		H = G; 
		G = F; 
		F = E; 
		E = D + T1; 
		D = C; 
		C = B; 
		B = A; 
		A = T1 + T2; 
		
	} 
	
    tSha256Context.Message_Digest[0] =
		(tSha256Context.Message_Digest[0] + A) & 0xFFFFFFFF;
    tSha256Context.Message_Digest[1] =
		(tSha256Context.Message_Digest[1] + B) & 0xFFFFFFFF;
    tSha256Context.Message_Digest[2] =
		(tSha256Context.Message_Digest[2] + C) & 0xFFFFFFFF;
    tSha256Context.Message_Digest[3] =
		(tSha256Context.Message_Digest[3] + D) & 0xFFFFFFFF;
    tSha256Context.Message_Digest[4] =
		(tSha256Context.Message_Digest[4] + E) & 0xFFFFFFFF;
	tSha256Context.Message_Digest[5] =
		(tSha256Context.Message_Digest[5] + F) & 0xFFFFFFFF;
	tSha256Context.Message_Digest[6] =
		(tSha256Context.Message_Digest[6] + G) & 0xFFFFFFFF;
	tSha256Context.Message_Digest[7] =
		(tSha256Context.Message_Digest[7] + H) & 0xFFFFFFFF;
	
    tSha256Context.Message_Block_Index = 0;
}

/*  
*  SHA256PadMessage
*
*  Description:
*      According to the standard, the message must be padded to an even
*      512 bits.  The first padding bit must be a '1'.  The last 64
*      bits represent the length of the original message.  All bits in
*      between should be 0.  This function will pad the message
*      according to those rules by filling the Message_Block array
*      accordingly.  It will also call SHA256ProcessMessageBlock()
*      appropriately.  When it returns, it can be assumed that the
*      message digest has been computed.
*
*  Parameters:
*      context: [in/out]
*          The context to pad
*
*  Returns:
*      Nothing.
*
*  Comments:
*
*/
void	CSHA2Util::SHA256PadMessage()
{
/*
*  Check to see if the current message block is too small to hold
*  the initial padding bits and length.  If so, we will pad the
*  block, process it, and then continue padding into a second
*  block.
	*/
    if (tSha256Context.Message_Block_Index > 55)
    {
        tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] = 0x80;
        while(tSha256Context.Message_Block_Index < 64)
        {
            tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] = 0;
        }
		
        SHA256ProcessMessageBlock();
		
        while(tSha256Context.Message_Block_Index < 56)
        {
            tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] = 0;
        }
    }
    else
    {
        tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] = 0x80;
        while(tSha256Context.Message_Block_Index < 56)
        {
            tSha256Context.Message_Block[tSha256Context.Message_Block_Index++] = 0;
        }
    }
	
    /*
	*  Store the message length as the last 8 octets
	*/
    tSha256Context.Message_Block[56] = (tSha256Context.Length_High >> 24) & 0xFF;
    tSha256Context.Message_Block[57] = (tSha256Context.Length_High >> 16) & 0xFF;
    tSha256Context.Message_Block[58] = (tSha256Context.Length_High >> 8) & 0xFF;
    tSha256Context.Message_Block[59] = (tSha256Context.Length_High) & 0xFF;
    tSha256Context.Message_Block[60] = (tSha256Context.Length_Low >> 24) & 0xFF;
    tSha256Context.Message_Block[61] = (tSha256Context.Length_Low >> 16) & 0xFF;
    tSha256Context.Message_Block[62] = (tSha256Context.Length_Low >> 8) & 0xFF;
    tSha256Context.Message_Block[63] = (tSha256Context.Length_Low) & 0xFF;
	
    SHA256ProcessMessageBlock();
}

