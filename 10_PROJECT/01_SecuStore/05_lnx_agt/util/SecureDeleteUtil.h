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

#ifndef _SECURE_DELETE_UTIL_H__
#define _SECURE_DELETE_UTIL_H__

#define NMETHODS			12
#define WIPE_METHOD_MAX		35
#define DEFAULT_BUFF_SIZE	1024
#define DOD5220				7

class CSecureDeleteUtil : public CUtilBase
{
public:
	CSecureDeleteUtil(void);
	~CSecureDeleteUtil(void);

public:
	INT32				WipeFilesA(char* pcPath, INT32 nWipingCount);
	void				SetBufferSize(unsigned long dwBytes = DEFAULT_BUFF_SIZE);

private:
	INT32				WipeObjectA(char *pszPath, INT32 nWipingCount);
	INT32				WipeFileA(char *pszFile, INT32 nWipingCount);
	INT32				WipeFileStreamA(FILE *fp, DWORD dwFileLen, INT32 nWipeType);
	INT32				WipeFileNameA(char *pcFilePath, char *pcNewFilePath, INT32 nNewFileMax);
	char*				GetBuffer(INT32 nType);
	void				CustFill(char *pBuf, unsigned long dwLength, char *pFill);
	void				FillMem(char *pBuf, unsigned long dwLength, char cFill);
	void				FillRndMem(char *pBuf, unsigned long dwLength, char cFill);
	INT32				AllocateMemory();
	VOID				FreeMemory();
	
private:
	DWORD				m_dwBufferSize;
	INT32				m_nMethod;
	char				*m_pBufff;
	char				*m_pBuffr;
	char				*m_pBuff7;
};

#endif /*_SECURE_DELETE_UTIL_H__*/