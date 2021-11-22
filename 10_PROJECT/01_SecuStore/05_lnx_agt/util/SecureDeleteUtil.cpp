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
#include "com_struct.h"
#include "SecureDeleteUtil.h"

char *g_acWipeMethods[NMETHODS][35]={
	{ "\x00\x00\x00", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // British HMG 1
	{ "\x00\x00\x00", "rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // Russian Gost
	{ "\x00\x00\x00", "\xff\xff\xff" ,"rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // British HMG 2
	{ "\xff\xff\xff", "\x27\xFF\xFF\xFF", "rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // NAVSO P-5239-26 (RLL)
	{ "\xff\xff\xff", "\x7F\xFF\xFF\xFF", "rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },  // NAVSO P-5239-26 (MFM)
	{ "\xeb\xeb\xeb", "\x14\x14\x14" ,"\x5c\x5c\x5c", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // NAVSO P-5239-26 (ALT)
	{ "\x00\x00\x00", "\xff\xff\xff" ,"rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // DoD 5220.22-M
	{ "\x00\x00\x00", "\xff\xff\xff" ,"rnd", "rnd", "\x00\x00\x00", "\xff\xff\xff" ,"rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, //
	{ "\x00\x00\x00", "\xff\xff\xff", "\x00\x00\x00", "\xff\xff\xff","\x00\x00\x00", "\xff\xff\xff", "rnd", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" },
	{ "\x00\x00\x00", "\xff\xff\xff", "\x00\x00\x00", "\xff\xff\xff","\x00\x00\x00", "\xff\xff\xff","\xaa\xaa\xaa", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // vsitr
	{ "\xff\xff\xff", "\x00\x00\x00", "rnd", "rnd", "rnd", "rnd", "rnd",  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "" }, // Bruce Scheier
	{"rnd", "rnd", "rnd", "rnd", "\x55\x55\x55", "\xaa\xaa\xaa",
	"\x92\x49\x24", "\x49\x24\x92", "\x24\x92\x49", "\x00\x00\x00",
	"\x11\x11\x11","\x22\x22\x22","\x33\x33\x33","\x44\x44\x44",
	"\x55\x55\x55","\x66\x66\x66","\x77\x77\x77","\x88\x88\x88",
	"\x99\x99\x99","\xaa\xaa\xaa","\xbb\xbb\xbb","\xcc\xcc\xcc",
	"\xdd\xdd\xdd","\xee\xee\xee","\xff\xff\xff",
	"\x92\x49\x24", "\x49\x24\x92", "\x24\x92\x49",
	"\x6d\xb6\xdb", "\xb6\xdb\x6d", "\xdb\x6d\xb6",
	"rnd", "rnd", "rnd", "rnd"}
};

char g_acVerified[NMETHODS]={0x01,0,0,0x04,0x04,0x04,0x07,0,0x64,0,0,0};


CSecureDeleteUtil::CSecureDeleteUtil(void)
{
	m_strUtilName	= "util secure delete";

	m_nMethod	= DOD5220;

	m_pBufff = NULL;
	m_pBuffr = NULL;
	m_pBuff7 = NULL;

	SetBufferSize();
}

CSecureDeleteUtil::~CSecureDeleteUtil(void)
{
}

INT32 CSecureDeleteUtil::AllocateMemory()
{
	if (m_dwBufferSize < 1)
	{
		m_dwBufferSize = 1024;
	}

	m_pBufff = (char*)malloc(m_dwBufferSize);
	m_pBuffr = (char*)malloc(m_dwBufferSize);
	m_pBuff7 = (char*)malloc(m_dwBufferSize);

	if (m_pBufff==NULL || m_pBuffr==NULL || m_pBuff7==NULL)
	{
		safe_free(m_pBufff);
		safe_free(m_pBuffr);
		safe_free(m_pBuff7);
		return -1;
	}

	return 0;
}

VOID CSecureDeleteUtil::FreeMemory()
{
	safe_free(m_pBufff);
	safe_free(m_pBuffr);
	safe_free(m_pBuff7);
}

INT32 CSecureDeleteUtil::WipeFilesA(char* pcPath, INT32 nWipingCount)
{
	INT32 nRetVal = 0;

	if(pcPath == NULL || nWipingCount < 1)
		return -1;
	
	nRetVal = AllocateMemory();
	if (nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}

	nRetVal = WipeObjectA(pcPath, nWipingCount);
	if(nRetVal != 0)
	{
		nRetVal -= 2000;
	}
	FreeMemory();
	return nRetVal;
}

void	CSecureDeleteUtil::SetBufferSize(unsigned long dwBytes)
{
	m_dwBufferSize = dwBytes;
}

INT32 CSecureDeleteUtil::WipeObjectA(char *pcPath, INT32 nWipingCount)
{
	INT32 nRetVal = 0;
	if (pcPath == NULL || nWipingCount < 1)
		return -1;

	if(is_file(pcPath) != 0)
	{
		return -2;
	}

	nRetVal = WipeFileA(pcPath, nWipingCount);
	if(nRetVal != 0)
	{
		nRetVal -= 100;
		return nRetVal;
	}
	return 0;
}


INT32 CSecureDeleteUtil::WipeFileA(char *pcFile, INT32 nWipingCount)
{
	FILE *fp = NULL;
	char acNewFilePath[MAX_PATH] = {0, };
	INT32 nRetVal = 0;
	INT32 nCount = 0;
	DWORD dwFileLen = 0;

	if(pcFile == NULL || nWipingCount < 1)
		return -1;

	if(get_file_size(pcFile, &dwFileLen) != 0)
		return -2;
		
	if (dwFileLen < 1)
	{
		if(WipeFileNameA(pcFile, acNewFilePath, MAX_PATH) == 0)
		{
			unlink(acNewFilePath);
		}
		else
		{
			unlink(pcFile);
		}
		return 0;
	}

	fp = fopen(pcFile, "r+b");
	if (fp == NULL)
	{
		return -2;
	}
	fseek(fp, 0, SEEK_SET);
	
	for(nCount = 0; nCount < nWipingCount; nCount++)
		WipeFileStreamA(fp, dwFileLen, nCount % WIPE_METHOD_MAX);
	
	fclose(fp);

	if(WipeFileNameA(pcFile, acNewFilePath, MAX_PATH) == 0)
	{
		unlink(acNewFilePath);
	}
	else
	{
		unlink(pcFile);
	}
		
	return 0;
}

INT32 CSecureDeleteUtil::WipeFileStreamA(FILE *fp, DWORD dwFileLen, INT32 nWipeType)
{
	char cVerify = g_acVerified[DOD5220];
	char *pBuff = NULL, *pvfBuff = NULL;
	DWORD i, dwCursor = 0;
	DWORD dwCount, dwMod = 0;

	if(fp == NULL || dwFileLen < 1 || nWipeType < 0 || nWipeType > WIPE_METHOD_MAX-1)
		return -1;

	pBuff=GetBuffer(nWipeType);
	if(pBuff == NULL)
		return -2;
	
	if (dwFileLen > m_dwBufferSize)
	{
		dwCursor = 0;
		fseek(fp, dwCursor, SEEK_SET);
		dwCount = (DWORD)(dwFileLen / m_dwBufferSize);
		dwMod = (DWORD)(dwFileLen % m_dwBufferSize);
		
		for(i = 1; i<dwCount; i++)
		{
			fwrite(pBuff, 1, m_dwBufferSize, fp);
			dwCursor += m_dwBufferSize;
			if(dwCursor >= dwFileLen)
				break;
			fseek(fp, dwCursor, SEEK_SET);
		}
		if(dwMod > 0)
			fwrite(pBuff, 1, dwMod, fp);
		
		if(cVerify & (1<<nWipeType))
		{
			pvfBuff=(char*)malloc(m_dwBufferSize);
			if(pvfBuff == NULL)
				return -3;
			memset(pvfBuff, 0, m_dwBufferSize);
			dwCursor = 0;
			fseek(fp, dwCursor, SEEK_SET);
			dwCount = (DWORD)(dwFileLen / m_dwBufferSize);
			dwMod = (DWORD)(dwFileLen % m_dwBufferSize);
			for(i = 1; i < dwCount; i++)
			{
				fread(pvfBuff, 1, m_dwBufferSize, fp);
				dwCursor += m_dwBufferSize;
				if(dwCursor >= dwFileLen)
					break;
				fseek(fp, dwCursor, SEEK_SET);
				
				if(memcmp(pvfBuff, pBuff, m_dwBufferSize)!=0)
				{
					safe_free(pvfBuff);
					return -4;
				}
			}
			if(dwMod > 0)
				fread(pvfBuff, 1, dwMod, fp);
			if(memcmp(pvfBuff, pBuff, dwMod)!=0)
			{
				safe_free(pvfBuff);
				return -5;
			}
			safe_free(pvfBuff);
		}
	} 
	else
	{
		fseek(fp, 0, SEEK_SET);
		fwrite(pBuff, 1, dwFileLen, fp);
		if(cVerify & (1<<nWipeType))
		{
			pvfBuff=(char*)malloc(dwFileLen);
			if(pvfBuff == NULL)
				return -6;
			memset(pvfBuff, 0, dwFileLen);
			
			fseek(fp, 0, SEEK_SET);
			fread(pvfBuff, 1, dwFileLen, fp);
			if(memcmp(pvfBuff, pBuff, dwFileLen)!=0)
			{
				safe_free(pvfBuff);
				return -7;
			}
			safe_free(pvfBuff);
		}
	}

	fflush(fp);

	return 0;
}

INT32 CSecureDeleteUtil::WipeFileNameA(char *pcFilePath, char *pcNewFilePath, INT32 nNewFileMax)
{
	char acPath[MAX_PATH] = {0,};
	if(pcFilePath == NULL || pcNewFilePath == NULL || nNewFileMax < 1)
		return -1;

	strncpy(acPath, pcFilePath, MAX_PATH-1);

	if(get_dirname(acPath, acPath, MAX_PATH) == NULL)
		return -2;
	snprintf(pcNewFilePath, nNewFileMax-1, "%s/%s", acPath, GetGUID().c_str());
	if(rename(pcFilePath, pcNewFilePath) == -1)
		return -3;
	return 0;
}

char* CSecureDeleteUtil::GetBuffer(INT32 nWipeType)
{
	char *pFillData = NULL;
	BOOL bSameByte = TRUE;
	INT32 nIndex = 0;
	if(nWipeType < 0 || nWipeType > WIPE_METHOD_MAX-1)
		return NULL;

	pFillData=g_acWipeMethods[m_nMethod][nWipeType];

	if (!_stricmp(pFillData, "rnd"))
	{
		FillRndMem(m_pBuffr, m_dwBufferSize, 0);
		return m_pBuffr;
	}

	for (nIndex = 1; nIndex < (INT32)strlen(pFillData); nIndex++)
	{
		if (pFillData[0]!=pFillData[nIndex])
		{
			bSameByte=FALSE;
			break;
		}
	}

	if(bSameByte)
	{
		FillMem(m_pBufff, m_dwBufferSize, pFillData[0]);
		return m_pBufff;
	}

	CustFill(m_pBuff7, m_dwBufferSize, pFillData);
	return m_pBuff7;
}

void CSecureDeleteUtil::FillRndMem(char *pBuf, unsigned long dwLength, char cFill)
{
	unsigned long *pMem = NULL;
	if(pBuf == NULL || dwLength < 1)
		return;
	for (pMem = (unsigned long *)pBuf; pMem < (unsigned long *)(pBuf + dwLength/4); pMem++)
	{
		srand((int)time(NULL));
		*pMem = (unsigned long) rand() % 0xffffffff;
	}
}

void CSecureDeleteUtil::FillMem(char *pBuf, unsigned long dwLength, char cFill)
{
	if(pBuf == 0 || dwLength < 1)
		return;
	memset(pBuf, cFill, dwLength);
}

void CSecureDeleteUtil::CustFill(char *pBuf, unsigned long dwLength, char *pFill)
{
	char *pBaux = NULL, *pFillData = NULL;
	DWORD i, dwLen=0;
	DWORD dwBauxLen=0;

	if(pBuf == NULL || dwLength < 1 || pFill == NULL)
		return;
	dwBauxLen = dwLength+16;
	pBaux=(char*)malloc(dwBauxLen);
	if (pBaux == NULL)
		return;
	memset(pBaux, 0, dwBauxLen);

	pFillData=pFill;
	dwLen=(DWORD)strlen(&pFillData[3])+3;

	for(i=0 ; i<dwBauxLen; i+=dwLen)
		memcpy((char *)(pBaux+i), pFillData, dwLen);

	memcpy(pBuf, pBaux, dwLength);
	
	safe_free(pBaux);
}
