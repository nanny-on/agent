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

// FileOpUtil.h: interface for the CFileOpUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEUTIL_H__91236E9F_4F99_4768_99D9_967DB67663F3__INCLUDED_)
#define AFX_FILEUTIL_H__91236E9F_4F99_4768_99D9_967DB67663F3__INCLUDED_

#include "stdafx.h"

typedef struct _ff_gather_file_info
{
	String		strPrePath;
	String		strAddPath;
	String		strFullPath;
	String		strFileName;
}FF_GATHER_FILE_INFO, *PFF_GATHER_FILE_INFO;

typedef list<FF_GATHER_FILE_INFO>			TListGatherFileInfo;
typedef TListGatherFileInfo::iterator		TListGatherFileInfoItor;

#define FILEUTIL_FILE_DT_TYPE_CREATE	0
#define FILEUTIL_FILE_DT_TYPE_MODIFY	1
#define FILEUTIL_FILE_DT_TYPE_ACCESS	2

#define CMD_GET_LOGIN_USER				"/usr/bin/who"

class CFileUtil  
{
public:
	void	AddLasCHARLen(LPTSTR pszInput, UINT32 nBufLen, char cAddChar = '/');
	void	AddLasCHAR(LPTSTR pszInput, char cAddChar = '/');
	void	AddLasCHAR(String& strInput, char cAddChar = '/'); 
	void	AddLasCHAR(StringW& strInput, WCHAR cAddChar = L'/');
	LPCTSTR	DelLasCHAR(LPTSTR pszInput, char cAddChar = '/');
	String	DelLasCHAR(String& strInput, char cAddChar = '/');
	bool	DirectoryExists(LPCTSTR lpDir);
	bool	FileExists(LPCTSTR lpFileName);
	bool	FileExistsW(const wchar_t *pwFileName);
	INT32	RemoveDirectoryAll(String strDir);	
	String	GetShortPathName_Yss(String strFullPathName);
	INT32 	GetFileDateTime(LPCSTR lpFileName, UINT32 *pnCDT, UINT32 *pnMDT, UINT32 *pnADT);
	DWORD	GetFileSizeExt(LPCTSTR lpFileName);
	DWORD	GetFileSizeExtW(const wchar_t *pwFileName);
	UINT64	GetDirSizeExt(LPCTSTR lpPath, TMapStrID* tNameSizeMap = NULL);
	INT32	GatherFileNameList(String strPrePath, String strAddPath, String strPassExtension, TListGatherFileInfo& tNamesList, INT32 nSubSearch = 1);
	INT32	GetFileNameList(String strPrePath, String strAddPath, String strSkipName, TListGatherFileInfo& tNamesList, INT32 nSubSearch = 1);
	INT32	ForceDirectory(LPCTSTR lpPath);
	INT32	ForceDirectoryW(const wchar_t * lpPath);
	HANDLE	OpenFileHandle(String strFileName);
    UINT32	ReadFileBuffer(String strFileName, String& strBuffer);
	UINT32	ReadFileBuffer(String strFileName, PBYTE* lpBuffer, UINT32& nBufLen);
	UINT32	ReadFileBuffer(String strFileName, PVOID lpBuffer, UINT32 nBufLen);
	HANDLE	FirstReadFileBuff(String strFileName, PBYTE pFileBuff, INT32 nBufLen);
	INT32	NextReadFileBuff(HANDLE hFile, PBYTE pFileBuf, INT32 nBufLen);
	UINT32	WriteFileBuffer(String strFileName, String strBuffer);
	UINT32	WriteFileBuffer(String strFileName, PBYTE lpBuffer, UINT32 nBufLen);
	UINT32	WriteFileBuffer(HANDLE hFile, PBYTE lpBuffer, UINT32 nBufLen);
	LPCTSTR	FindExtension(LPTSTR lpFileName);
	String	FindFileName(String strFullName);
	String	FindFilePath(String strFullName);

	String	GetNewFileName(String strFileFullName, INT32 nOrUsed = 1, String strAppand = "");
	String	GetNewFileName(String strFilePath, String strFileName, INT32 nOrUsed = 1, String strAppand = "");
	INT32	GetherUsersDirNameList(String strPrePath, TListGatherFileInfo& tNamesList);
	INT32	GetEnvPathToLocalPath_UsersEnvByDirPath(TListStr& tPathList, String strPreKey, UINT32 nRemainLog = 1);

public:
	INT32	RemoveFileAll(String strDir, String strExtFilter="*.pkg");
	UINT32	FindFileList(OUT TListStr& tListFiles, IN String strPath, IN BOOL bIncludeSubPath = TRUE, IN String strExtFilter="*.*");

private:
	String		m_strEnvPathKey;
	TListStr	m_tUserList;
	void		SetEnvPathKey(String strName, String strData);
	String		GetEnvPathKey(String strName);
	VOID		GetCurrentLoginUser();
	INT32		IsCurrentLoginUser(char *pUserName);

private:
	INT32		GetEnvPathToLocalPath_KorPath(char *pcUseDir, char *pcKey, String& strUserSubKorPath);
	INT32		GetEnvPathToLocalPath_UsersEnv(TListStr& tPathList, char *pcPreKey, UINT32 nRemainLog = 1, UINT32 nEnvWaitSec = 60);
	INT32		GetEnvPathToLocalPath_EnvPath(HKEY nRegKey, String strRegRoot, String strEnvValue, String& strPathValue);
	INT32		GetEnvPathToLocalPath_TokInfo(String strValue, String& strEnvKey, String& strRemainValue);

	INT32		GetEnvPathToLocalPath_WindowsUpdate(TListStr& tPathList, String strPreKey, UINT32 nRemainLog = 1);
	
public:
	void		SetEnvPathRegKey(String strRegKey);
	INT32		GetEnvPathToLocalPath(String strEnvPath, TListStr* pLocalPathList, UINT32 nRemainLog = 1, UINT32 nEnvWaitSec = 60);
	String		GetEnvPathToLocalPathSingle(String strEnvPath, UINT32 nRemainLog = 1);
	INT32		IsExistFileDateTime(UINT32 nChkFDTType, UINT32 nChkFDValue, String strFilePath, String strFileName);

public:
	CFileUtil();
	virtual ~CFileUtil();
};

extern CFileUtil*	t_FileUtil;

#endif // !defined(AFX_FILEUTIL_H__91236E9F_4F99_4768_99D9_967DB67663F3__INCLUDED_)
