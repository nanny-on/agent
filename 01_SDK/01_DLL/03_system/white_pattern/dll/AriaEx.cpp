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

#include "stdafx.h"
#include "com_struct.h"
#include "CRC32.h"
#include "AriaReal.h"
#include "AriaEx.h"

INT32	AriaEnCryptSteram(char *szUKey, char *szInData, char *szOutData, INT32 nLen, char *acLogMsg)
{
	static BYTE abRoundKey[16 * 17] = {0};
	static BYTE abMasterKey[SIZE_USERKEY+1] = {0};
	static INT32 nKey = 0;
	static BOOL bKeySetup = FALSE;
	CHAR szUserKey[SIZE_USERKEY+1] = {0};
	CHAR szInTemp[SIZE_BLOCK+1] = {0};
	CHAR szOutTemp[SIZE_BLOCK+1] = {0};
	INT32 nOutSize = 0;
	INT32 nPart = 0, i;

	if(szUKey == NULL || szInData == NULL || szOutData == NULL || nLen < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptSteram] invalid input data");
		return -1;
	}

	strncpy(szUserKey, szUKey, SIZE_USERKEY);

	if(bKeySetup == FALSE || memcmp(szUserKey, abMasterKey, SIZE_USERKEY))
	{
		memcpy(abMasterKey, szUserKey, SIZE_USERKEY);
		nKey = EncKeySetup(abMasterKey, abRoundKey, SIZE_KRYBITS);
		bKeySetup = TRUE;
	}

	nPart = nLen / SIZE_BLOCK;
	if(nLen % SIZE_BLOCK != 0)
	{
		nPart += 1;
	}

	if(nPart == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptSteram] invalid input length (%d)", nLen);
		return -2;
	}

	for(i=0; i<nPart; i++)
	{
		memcpy(szInTemp, szInData+(i*SIZE_BLOCK), SIZE_BLOCK);
		Crypt((BYTE*)szInTemp, nKey, abRoundKey, (BYTE*)szOutTemp);
		memcpy(szOutData+(i*SIZE_BLOCK), szOutTemp, SIZE_BLOCK);
		nOutSize += SIZE_BLOCK;
	}

	return nOutSize;
}

INT32	AriaDeCryptSteram(char *szUKey, char *szInData, char *szOutData, INT32 nLen, char *acLogMsg)
{
	static BYTE abRoundKey[16 * 17] = {0};
	static BYTE abMasterKey[SIZE_USERKEY+1] = {0};
	static INT32 nKey = 0;
	static BOOL bKeySetup = FALSE;
	CHAR szInTemp[SIZE_BLOCK+1] = {0};
	CHAR szOutTemp[SIZE_BLOCK+1] = {0};
	INT32 nOutSize = 0;
	INT32 nPart = 0, i;
	CHAR szUserKey[SIZE_USERKEY+1] = {0};

	if(szUKey == NULL || szInData == NULL || szOutData == NULL || nLen < 1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptSteram] invalid input data");
		return -1;
	}
	strncpy(szUserKey, szUKey, SIZE_USERKEY);

	if(bKeySetup == FALSE || memcmp(szUserKey, abMasterKey, SIZE_USERKEY))
	{
		memcpy(abMasterKey, szUserKey, SIZE_USERKEY);
		nKey = DecKeySetup(abMasterKey, abRoundKey, SIZE_KRYBITS);
		bKeySetup = TRUE;
	}

	nPart = nLen / SIZE_BLOCK;
	if(nPart == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptSteram] invalid input length (%d)", nLen);
		return -2;
	}

	for(i=0; i<nPart; i++)
	{
		memcpy(szInTemp, szInData+(i*SIZE_BLOCK), SIZE_BLOCK);
		Crypt((BYTE*)szInTemp, nKey, abRoundKey, (BYTE*)szOutTemp);
		memcpy(szOutData+(i*SIZE_BLOCK), szOutTemp, SIZE_BLOCK);
		nOutSize += SIZE_BLOCK;
	}

	return nOutSize;
}

// 1 = Enc,2= Dec
INT32 AriaEnCryptFile(char *szUKey, char *szInFile, char *szOutFile, char *acLogMsg)
{
	DWORD dwCRC32 = 0;
	FILE *fpIn = NULL;
	FILE *fpOut = NULL;
	PCHAR pcBuffer = NULL;
	PCHAR pcOutdata = NULL;
	INT32 nReadSize = 0;
	INT32 nWriteSize = 0;
	AINFO stInfo;
	DWORD dwFileSize = 0;
	INT32 nRetVal = 0;

	if(szUKey == NULL || szInFile == NULL || szOutFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] invalid input data");
		return -1;
	}
	memset(&stInfo, 0, sizeof(AINFO));

	do{
		dwFileSize = GetFileSizeExt(szInFile);
		if(dwFileSize == 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] invalid input file size (%s)", szInFile);
			nRetVal = -2;
			break;
		}

		nRetVal = getCRC32(&dwCRC32, szInFile);
		if(nRetVal < 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to get crc32 code (%s) (%d)", szInFile, nRetVal);
			nRetVal -= 10;
			break;
		}

		pcBuffer = (PCHAR)malloc(MAX_BUFF+1);
		if(pcBuffer == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to allocate memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pcBuffer, 0, MAX_BUFF+1);
		pcOutdata = (PCHAR)malloc(MAX_BUFF + SIZE_BLOCK + 1);
		if(pcOutdata == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to allocate memory (%d)", errno);
			nRetVal = -4;
			break;
		}
		memset(pcOutdata, 0, MAX_BUFF + SIZE_BLOCK + 1);

		fpIn = fopen(szInFile, "rb");
		if(fpIn == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to open file %s (%d)", szInFile, errno);
			nRetVal = -5;
			break;
		}

		fpOut = fopen(szOutFile, "wb");
		if(fpOut == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to open file %s (%d)", szOutFile, errno);
			nRetVal = -6;
			break;
		}

		memcpy(stInfo.szHeader, ARIA_HEADER, SIZE_ARIA);
		stInfo.dwCRC32 = dwCRC32;
		stInfo.nSize = (INT32)dwFileSize;

		fwrite((char *)&stInfo, 1, SIZE_HEADER, fpOut);

		while(TRUE)
		{
			nRetVal = 0;
			nReadSize = (INT32)fread(pcBuffer, 1, MAX_BUFF, fpIn);
			if(nReadSize <= 0)
			{
				break;
			}

			if(nReadSize < MAX_BUFF)
				nRetVal = AriaEnCryptSteram(szUKey, pcBuffer, pcOutdata, nReadSize, acLogMsg);
			else
				nRetVal = AriaEnCryptSteram(szUKey, pcBuffer, pcOutdata, MAX_BUFF, acLogMsg);

			if(nRetVal < 0)
			{
				nRetVal -= 20;
				break;
			}
			nWriteSize = nRetVal;
			fwrite(pcOutdata, 1, nWriteSize, fpOut);
		}

		if(nRetVal < 0)
			break;

		nRetVal = 0;
	}while(FALSE);
	if(fpIn != NULL)
		fclose(fpIn);
	if(fpOut != NULL)
		fclose(fpOut);
	safe_free(pcBuffer);
	safe_free(pcOutdata);

	return nRetVal;
}

// 1 = Enc,2= Dec
INT32 AriaDeCryptFile(char *szUKey, char *szInFile, char *szOutFile, char *acLogMsg)
{
	PCHAR pcBuffer = NULL;
	PCHAR pcOutdata = NULL;
	INT32 nReadSize = 0;
	INT32 nWriteSize = 0;
	INT32 nRetVal = 0;
	INT32 nFileSize = 0;
	DWORD dwCRC32 = 0;
	AINFO stInfo;
	FILE *fpIn = NULL;
	FILE *fpOut = NULL;

	if(szUKey == NULL || szInFile == NULL || szOutFile == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] invalid input data");
		return -1;
	}

	memset(&stInfo, 0, sizeof(AINFO));

	do{
		pcBuffer = (PCHAR)malloc(MAX_BUFF+1);
		if(pcBuffer == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to allocate memory (%d)", errno);
			nRetVal = -2;
			break;
		}
		memset(pcBuffer, 0, MAX_BUFF+1);
		pcOutdata = (PCHAR)malloc(MAX_BUFF + 1);
		if(pcOutdata == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptFile] fail to allocate memory (%d)", errno);
			nRetVal = -3;
			break;
		}
		memset(pcOutdata, 0, MAX_BUFF + 1);

		fpIn = fopen(szInFile, "rb");
		if(fpIn == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] fail to open file %s (%d)", szInFile, errno);
			nRetVal = -4;
			break;
		}

		fread((char *)&stInfo, 1, SIZE_HEADER, fpIn);

		if(memcmp(stInfo.szHeader, ARIA_HEADER, SIZE_ARIA))
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] invalid header %s", stInfo.szHeader);
			nRetVal = -5;
			break;
		}

		fpOut = fopen(szOutFile, "wb");
		if(fpOut == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] fail to open file %s (%d)", szOutFile, errno);
			nRetVal = -6;
			break;
		}

		nFileSize = stInfo.nSize;
		if(nFileSize < 0)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] invalid file size (%d)", nFileSize);
			nRetVal = -7;
			break;
		}

		while(TRUE)
		{
			nRetVal = 0;
			nReadSize = (INT32)fread(pcBuffer, 1, MAX_BUFF, fpIn);
			if(nReadSize <= 0)
			{
				break;
			}

			if(nReadSize < MAX_BUFF)
				nRetVal = AriaDeCryptSteram(szUKey, pcBuffer, pcOutdata, nReadSize, acLogMsg);
			else
				nRetVal = AriaDeCryptSteram(szUKey, pcBuffer, pcOutdata, MAX_BUFF, acLogMsg);

			if(nRetVal < 0)
			{
				nRetVal -= 10;
				break;
			}
			nWriteSize = nRetVal;
			if(nFileSize < nWriteSize)
				nWriteSize = nFileSize;

			fwrite(pcOutdata, 1, nWriteSize, fpOut);
			nFileSize -= nWriteSize;
		}

		if(nRetVal < 0)
		{
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	if(fpIn)
		fclose(fpIn);
	if(fpOut)
		fclose(fpOut);
	safe_free(pcBuffer);
	safe_free(pcOutdata);

	if(nRetVal < 0)
	{
		unlink(szOutFile);
		return nRetVal;
	}

	nRetVal = getCRC32(&dwCRC32, szOutFile);
	if(nRetVal < 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] fail to get crc32 code (%s) (%d)", szOutFile, nRetVal);
		nRetVal -= 20;
		unlink(szOutFile);
		return nRetVal;
	}

	if(stInfo.dwCRC32 != dwCRC32)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptFile] invalid crc code (%x) (%x)", stInfo.dwCRC32, dwCRC32);
		nRetVal = -8;
		unlink(szOutFile);
		return nRetVal;
	}
	return 0;
}

INT32 AriaEnCryptBuffer(char *szUKey, char *szInData, INT32 nLen, char *szOutData, INT32 *pnOutLen, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 nSize = 0;
	DWORD dwCRC32 = 0;
	AINFO stInfo;

	if(szUKey == NULL || szInData == NULL || nLen < 1 || szOutData == NULL || pnOutLen == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptBuffer] invalid input data");
		return -1;
	}
	memset(&stInfo, 0, sizeof(AINFO));

	nSize = SIZE_HEADER + ((nLen - 1) / SIZE_BLOCK + 1) * SIZE_BLOCK;
	if(nSize > *pnOutLen)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptBuffer] invalid data length (sz:%d) (out:%d)", nSize, *pnOutLen);
		*pnOutLen = nSize;
		return -2;
	}

	if(getCRC32Buffer(&dwCRC32, szInData, nLen) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaEnCryptBuffer] fail to get crc code");
		return -3;
	}

	memcpy(stInfo.szHeader, ARIA_HEADER, SIZE_ARIA);
	stInfo.nSize = nLen;
	stInfo.dwCRC32 = dwCRC32;

	memcpy(szOutData, (char *)&stInfo, SIZE_HEADER);

	nRetVal = AriaEnCryptSteram(szUKey, szInData, &szOutData[SIZE_HEADER], nLen, acLogMsg);
	if(nRetVal < 0)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	nRetVal += SIZE_HEADER;
	return nRetVal;
}

INT32 AriaDeCryptBuffer(char *szUKey, char *szInData, char *szOutData, INT32 *pnOutLen, char *acLogMsg)
{
	INT32 nRetVal = 0;
	AINFO stInfo;
	DWORD dwCRC32 = 0;
	INT32 nSize = 0;

	if(szUKey == NULL || szInData == NULL || szOutData == NULL || pnOutLen == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] invalid input data");
		return -1;
	}
	memset(&stInfo, 0, sizeof(AINFO));

	if(*pnOutLen < SIZE_HEADER)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] invalid input data (%d)", *pnOutLen);
		*pnOutLen = 0;
		return -2;
	}

	if(memcmp(szInData, ARIA_HEADER, SIZE_ARIA))
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] invalid input data (in:%s)", szInData);
		*pnOutLen = 0;
		return -3;
	}

	memcpy((char *)&stInfo, szInData, SIZE_HEADER);
	if(*pnOutLen < stInfo.nSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] invalid input data size (in:%d) (hd:%d)", *pnOutLen, stInfo.nSize);
		*pnOutLen = stInfo.nSize;
		return -4;
	}

	nSize = ((stInfo.nSize - 1) / SIZE_BLOCK + 1) * SIZE_BLOCK;
	nRetVal = AriaDeCryptSteram(szUKey, szInData+SIZE_HEADER, szOutData, nSize, acLogMsg);
	if(nRetVal < 0)
	{
		*pnOutLen = stInfo.nSize;
		nRetVal -= 10;
		return nRetVal;
	}

	if(getCRC32Buffer(&dwCRC32, szOutData, stInfo.nSize) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] fail to get crc data (hd:%d)", stInfo.nSize);
		*pnOutLen = stInfo.nSize;
		nRetVal = -5;
		return nRetVal;
	}

	if(stInfo.dwCRC32 == dwCRC32)
	{
		szOutData[stInfo.nSize] = 0;
		return stInfo.nSize;
	}

	if(getCRC32Old(&dwCRC32, szOutData, stInfo.nSize) == FALSE)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] fail to get crc old data (hd:%d)", stInfo.nSize);
		*pnOutLen = stInfo.nSize;
		nRetVal = -6;
		return nRetVal;
	}
	if(stInfo.dwCRC32 == dwCRC32)
	{
		szOutData[stInfo.nSize] = 0;
		return stInfo.nSize;
	}

	if(acLogMsg)
		snprintf(acLogMsg, MAX_LOGMSG, "[AriaDeCryptBuffer] invalid crc32 data (hd:%08x) (ck:%08x)", stInfo.dwCRC32, dwCRC32);

	return -7;
}

