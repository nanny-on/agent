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
#include "as_base32.h"

int base32_encode(const UINT8 *data, int length, UINT8 *result, int bufSize)
{
	int i = 0, j = 0, nLoop;
	int nLen = 0;
	BYTE abBase32[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234579";
	int nBaseLen = 0;

	if (data == NULL || length < 1 || length > 0xFFFFFFF || result == NULL || bufSize < 1)
	{
		return 0;
	}
	nBaseLen = (INT32)strlen((char *)abBase32);
	nLoop = length / 5;
	while(nLoop)
	{
		nLen = (INT32)((data[i]&0xF8) >> 3);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)(((data[i]&0x07) << 2) | (data[i+1] >> 6));
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)((data[i+1]&0x3E) >> 1);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)(((data[i+1]&0x01) << 4) | ((data[i+2]&0xF0) >> 4));
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)(((data[i+2]&0x0F) << 1) | ((data[i+3]&0x80) >> 7));
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)((data[i+3]&0x7C) >> 2);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)(((data[i+3]&0x03) << 3) | ((data[i+4]&0xE0) >> 5));
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLen = (INT32)(data[i+4]&0x1F);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];

		nLoop--;
		i += 5;
	}

	nLoop = length % 5;
	if (nLoop == 0)
	{
		return j;
	}

	nLen = (INT32)(data[i] >> 3);
	result[j++] = abBase32[nLen];

	if (nLoop == 1)
	{
		nLen = (INT32)(((data[i]&0x07) << 2) & 0x1C);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];
		return j;
	}

	nLen = (INT32)(((data[i]&0x07) << 2) | (data[i+1] >> 6));
	if (i == length || j == bufSize || nLen > nBaseLen)
		return j;
	result[j++] = abBase32[nLen];

	nLen = (INT32)((data[i+1]&0x3E) >> 1);
	if (i == length || j == bufSize || nLen > nBaseLen)
		return j;
	result[j++] = abBase32[nLen];

	if (nLoop == 2)
	{
		nLen = (INT32)(((data[i+1]&0x01) << 4) & 0x10);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];
		return j;
	}

	nLen = (INT32)(((data[i+1]&0x01) << 4) | ((data[i+2]&0xF0) >> 4));
	result[j++] = abBase32[nLen];
	if (nLoop == 3)
	{
		nLen = (INT32)(((data[i+2]&0x0F) << 1) & 0x1E);
		if (i == length || j == bufSize || nLen > nBaseLen)
			return j;
		result[j++] = abBase32[nLen];
		return j;
	}

	nLen = (INT32)(((data[i+2]&0x0F) << 1) | ((data[i+3]&0x80) >> 7));
	if (i == length || j == bufSize || nLen > nBaseLen)
		return j;
	result[j++] = abBase32[nLen];

	nLen = (INT32)((data[i+3]&0x7C) >> 2);
	if (i == length || j == bufSize || nLen > nBaseLen)
		return j;
	result[j++] = abBase32[nLen];

	nLen = (INT32)(((data[i+3]&0x03) << 3) & 0x18);
	if (i == length || j == bufSize || nLen > nBaseLen)
		return j;
	result[j++] = abBase32[nLen];

	return j;
}


BYTE decode_char(BYTE bInChar)
{
	BYTE bDecode = 0;
	if ((bInChar>='A') && (bInChar<='Z'))
		return (bInChar-'A');
	else if ((bInChar>='0') && (bInChar<='9'))
	{
		bDecode = bInChar-'0';
		switch (bDecode)
		{
		case 2:
			bDecode = 26;
			break;
		case 3:
			bDecode = 27;
			break;
		case 4:
			bDecode = 28;
			break;
		case 5:
			bDecode = 29;
			break;
		case 7:
			bDecode = 30;
			break;
		case 9:
			bDecode = 31;
			break;
		}
	}
	return bDecode;
}

int base32_decode(const UINT8 *encoded, UINT8 *result, int bufSize)
{
	INT32 i = 0, j = 0;
	BYTE bDecode1, bDecode2, bDecode3;
	int length = 0;

	if(encoded == NULL || bufSize < 1 || result == NULL)
	{
		return 0;
	}

	length = strlen((char *)encoded);
	if(length < 1)
	{
		return 0;
	}

	while(i < length)
	{
		bDecode1 = decode_char(encoded[i++]);
		bDecode2 = decode_char(encoded[i++]);
		result[j++] = ((bDecode1 << 3)&0xF8) | ((bDecode2&0x1C)>>2);
		if (i == length || j == bufSize)
			return j;

		bDecode1 = decode_char(encoded[i++]);
		bDecode3 = decode_char(encoded[i++]);
		result[j++] = ((bDecode2&0x03)<<6) | (bDecode1<<1) | ((bDecode3&0x10)>>4);
		if (i == length || j == bufSize)
			return j;

		bDecode1 = decode_char(encoded[i++]);
		result[j++] = ((bDecode3&0x0F)<<4) | ((bDecode1&0x1E)>>1);
		if (i == length || j == bufSize)
			return j;

		bDecode2 = decode_char(encoded[i++]);
		bDecode3 = decode_char(encoded[i++]);
		result[j++] = ((bDecode1&0x01)<<7) | (bDecode2<<2) | ((bDecode3&0x18)>>3);
		if (i == length || j == bufSize)
			return j;

		bDecode1 = decode_char(encoded[i++]);
		result[j++] = ((bDecode3&0x07)<<5) | (bDecode1&0x1F);
		if (i == length || j == bufSize)
			return j;
	}

	return j;
}

