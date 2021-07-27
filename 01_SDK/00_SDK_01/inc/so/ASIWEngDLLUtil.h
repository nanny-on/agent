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

// ASIWENGDLLUtil.h: interface for the CASIWEngDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASIWENGDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
#define AFX_ASIWENGDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_

class CASIWEngDLLUtil  
{
private:
	HINSTANCE	m_hDLL;
	PVOID		m_lpObject;
	
public:
	INT32		LoadLibraryExt(LPCTSTR szDLLPath);
	INT32		FreeLibraryExt();

public:	
	INT32	Init();
	INT32	Free();

	INT32	InitDB();
	INT32	LoadDB(LPCTSTR acEnginePath);
	INT32	CreateDB(LPCTSTR acWhitePath, LPCTSTR acEnginePath, LPCTSTR acDbName);
	INT32	AddFile(LPCTSTR szPath);
	INT32	DelFile(LPCTSTR szPath);
	INT32	ClearFile();
	INT32	MakeDB(LPCTSTR acEnginePath, LPCTSTR acDbName);
	INT32	UnInitDB();
	INT32	AddWL(LPCTSTR szPath, PVOID pWhiteList, DWORD dwListSize);
	INT32	ModWL(PVOID pWhiteList, DWORD dwListSize);
	INT32	DelWL(LPCTSTR szPath,  PVOID pWhiteList, DWORD dwListSize);
	INT32	GetWL(LPCTSTR szPath, PVOID pWhiteList, DWORD dwListSize, PDWORD pdwWFileType);
	INT32	GetWLCnt(PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount);
	INT32	GetAllWL(PVOID pPasrseData, DWORD dwDataSize, INT32 nTotalCount, PVOID pWhiteList, DWORD dwListSize);
	INT32	GetWLDBCnt(LPCTSTR szEnginePath, INT32 *pnFileCount);
	INT32	GetAllWLDBHdr(LPCTSTR szEnginePath, INT32 nFileCount, PVOID pWhiteHdr, DWORD dwHdrSize);
	INT32	GetWLDBHdr(LPCTSTR acEngineFile, PVOID pWhiteHdr, DWORD dwHdrSize);
	INT32	MakeWLDB(LPCTSTR szEngineFile, DWORD dwCategory, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount);
	INT32	LoadWL(LPCTSTR szEngineFile, PVOID pPasrseData, DWORD dwDataSize, INT32 *pnCount);

public:
	CHAR	*GetLastError()	{	return m_szErrMsg;	};
	CHAR	m_szErrMsg[CHAR_MAX_SIZE];

public:
	CASIWEngDLLUtil();
	virtual ~CASIWEngDLLUtil();

};

extern CASIWEngDLLUtil*	t_ASIWENGDLLUtil;

#endif // !defined(AFX_ASIWENGDLLUTIL_H__BBCD1EB7_1B5F_497A_87D5_BDEC7EBF1C3F__INCLUDED_)
