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

// FileOpUtil.cpp: implementation of the CFileOpUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "FileUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileUtil*	t_FileUtil = NULL;

//--------------------------------------------------------------------

CFileUtil::CFileUtil()
{
}

CFileUtil::~CFileUtil()
{

}

bool CFileUtil::DirectoryExists(LPCTSTR lpDir)
{
	struct stat sb;
	if(lpDir == NULL || lpDir[0] == 0)
	{
		return FALSE;
	}
	
	if(stat(lpDir, &sb) == -1)
		return FALSE;

	return S_ISDIR(sb.st_mode);
}

bool CFileUtil::FileExists(LPCTSTR lpFileName)
{
	struct stat sb;
	if(lpFileName == NULL || lpFileName[0] == 0)
	{
		return FALSE;
	}
	
	if(stat(lpFileName, &sb) == -1)
		return FALSE;
	return S_ISREG(sb.st_mode);
}

bool CFileUtil::FileExistsW(const wchar_t *pwFileName)
{
	char acFilePath[MAX_PATH] = {0,};
	if(pwFileName == NULL || pwFileName[0] == L'\0')
	{
		return FALSE;
	}

	if(wcstombs(acFilePath, pwFileName, MAX_PATH-1) < 0)
	{
		return FALSE;
	}
	acFilePath[MAX_PATH-1] = 0;
	return FileExists(acFilePath);
}

void CFileUtil::AddLasCHARLen(LPTSTR pszInput, UINT32 nBufLen, char cAddChar)
{
	UINT32 nLastIndex = strlen(pszInput) -1;
	if(nLastIndex < 0)
		return;
	
	if(pszInput[nLastIndex] != cAddChar)
	{		
		if(nBufLen > (nLastIndex + 2))
		{
			pszInput[nLastIndex + 1] = cAddChar; 	
			pszInput[nLastIndex + 2] = 0;
		}
	}
}

void CFileUtil::AddLasCHAR(LPTSTR pszInput, char cAddChar)
{
	if(!pszInput)	return;

	UINT32 nLastIndex = strlen(pszInput) -1;
	if(nLastIndex < 0)
	{
		pszInput[0] = cAddChar;
		return;
	}

	if(pszInput[nLastIndex] != cAddChar)
	{		
		pszInput[nLastIndex + 1] = cAddChar; 	
		pszInput[nLastIndex + 2] = 0;
	}
}

void CFileUtil::AddLasCHAR(String& strInput, char cAddChar)
{
	INT32 nLastIndex = strInput.length() -1;
	if(nLastIndex < 0)
		return;

	if(strInput.at(nLastIndex) != cAddChar)
	{
		strInput += SPrintf("%c", cAddChar);
	}
}

void	CFileUtil::AddLasCHAR(StringW& strInput, WCHAR cAddChar)
{
	INT32 nLastIndex = strInput.length() -1;
	if(nLastIndex < 0)
		return;

	if(strInput.at(nLastIndex) != cAddChar)
	{
		strInput += WPrintf(L"%c", cAddChar);
	}
}

LPCTSTR CFileUtil::DelLasCHAR(LPTSTR pszInput, char cAddChar)
{
	INT32 nLastIndex = strlen(pszInput) -1;
	if(nLastIndex < 0)
		return LPCTSTR(pszInput);
	
	if(pszInput[nLastIndex] == cAddChar)
	{
		pszInput[nLastIndex] = 0; 
	}
	return LPCTSTR(pszInput);
}

String CFileUtil::DelLasCHAR(String& strInput, char cAddChar)
{
	INT32 nLastIndex = strInput.length() -1;
	if(nLastIndex < 0)
		return strInput;

	if(strInput.at(nLastIndex) == cAddChar)
	{
		strInput.erase(strInput.begin()+nLastIndex);
	}
	return strInput;
}

String CFileUtil::GetShortPathName_Yss(String strFullPathName)
{

	return strFullPathName;
}

INT32 CFileUtil::RemoveDirectoryAll(String strDir)
{
	String strDelDir;
	String strDelFileName;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	strDelDir = strDir;
	if(DirectoryExists(strDelDir.c_str()) == FALSE)
	{
		return -1;
	}

	dp = opendir(strDelDir.c_str());
	if (dp == NULL)
	{
		return -2;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}

		strDelFileName = SPrintf("%s/%s", strDir.c_str(), dirp->d_name);
		if(DT_DIR == dirp->d_type)
		{
			RemoveDirectoryAll(strDelFileName);
		}
		else
		{
			unlink(strDelFileName.c_str());
		}
	}

	closedir(dp);
	rmdir(strDelDir.c_str());
	return 0;
		
}

INT32 CFileUtil::RemoveFileAll(String strDir, String strExtFilter)
{
	String strDelDir;
	String strFileName;
	char *pcFileName = NULL;
	int nLen = 0;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	strDelDir = strDir;
	if(DirectoryExists(strDelDir.c_str()) == FALSE)
	{
		return -1;
	}

	dp = opendir(strDelDir.c_str());
	if (dp == NULL)
	{
		return -2;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		pcFileName = (char *)dirp->d_name;
		if(DT_REG == dirp->d_type)
		{
			nLen = (int)strlen(pcFileName);
			if(nLen > 4 && !_stricmp(&pcFileName[nLen-4], ".pkg"))
			{
				strFileName = SPrintf("%s/%s", strDelDir.c_str(), pcFileName);
				unlink(strFileName.c_str());
			}
		}
	}
	closedir(dp);
	return 0;
}

INT32	CFileUtil::GatherFileNameList(String strPrePath, String strAddPath, String strPassExtension, TListGatherFileInfo& tNamesList, INT32 nSubSearch)
{
	String strFindDir;
	String strFilePath;
	String strFileName;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if(strAddPath.empty())
		strFindDir = strPrePath;
	else
		strFindDir = SPrintf("%s/%s", strPrePath.c_str(), strAddPath.c_str());

	if(DirectoryExists(strFindDir.c_str()) == FALSE)
    {
        return -1;
    }

	dp = opendir(strFindDir.c_str());
	if (dp == NULL)
	{
		return -2;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}
		strFileName = SPrintf("%s", dirp->d_name);
		strFilePath = SPrintf("%s/%s", strFindDir.c_str(),  strFileName.c_str());
		if(DirectoryExists(strFilePath.c_str()))
		{	
			if(nSubSearch)
			{
				String strSubAddPath;
				if(strAddPath.empty())
					strSubAddPath = strFileName;
				else
					strSubAddPath = SPrintf("%s/%s", strAddPath.c_str(),  strFileName.c_str());
				GatherFileNameList(strPrePath, strSubAddPath, strPassExtension, tNamesList, nSubSearch);	
			}
		}
		else if(strPassExtension.empty())
		{
			FF_GATHER_FILE_INFO gather_file_info;
			gather_file_info.strPrePath		= strPrePath;
			gather_file_info.strAddPath		= strAddPath;
			gather_file_info.strFileName	= strFileName;
			gather_file_info.strFullPath	= strFilePath;
			tNamesList.push_back(gather_file_info);
		}
		else
		{
			String strFileExtension = FindExtension((LPSTR)strFileName.c_str());
//			strFileExtension = _strlwr(strFileExtension.c_str());
			if(strPassExtension.find(strFileExtension) == string::npos)
			{
				FF_GATHER_FILE_INFO gather_file_info;
				gather_file_info.strPrePath		= strPrePath;
				gather_file_info.strAddPath		= strAddPath;
				gather_file_info.strFileName	= strFileName;
				gather_file_info.strFullPath	= strFilePath;
				tNamesList.push_back(gather_file_info);
			}
		}
	}
    
    closedir(dp);
    
    return tNamesList.size();
}
//---------------------------------------------------------------------------

INT32	CFileUtil::GetFileNameList(String strPrePath, String strAddPath, String strSkipName, TListGatherFileInfo& tNamesList, INT32 nSubSearch)
{
	String strFindDir = strPrePath;
	String strFilePath;
	String strFileName;
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if(strAddPath.empty())
		strFindDir = strPrePath;
	else
		strFindDir = SPrintf("%s/%s", strPrePath.c_str(), strAddPath.c_str());

	if(DirectoryExists(strFindDir.c_str()) == FALSE)
	{
		return -1;
	}

//	strSkipName = _strlwr(strSkipName.c_str());

	dp = opendir(strFindDir.c_str());
	if (dp == NULL)
	{
		return -2;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}
	
		strFileName = SPrintf("%s", dirp->d_name);
		strFilePath = SPrintf("%s/%s", strFindDir.c_str(),  strFileName.c_str());
		if(DirectoryExists(strFilePath.c_str()))
		{	
			String strTempName = strFileName;
//			strDirName.MakeLower();
//			strTempName = _strlwr(strTempName.c_str());
			if(nSubSearch && strSkipName.find(strTempName) == string::npos)
			{
				String strSubAddPath;
				if(strAddPath.empty())
					strSubAddPath = strFileName;
				else
					strSubAddPath = SPrintf("%s/%s", strAddPath.c_str(), strFileName.c_str());

				GetFileNameList(strPrePath, strSubAddPath, strSkipName, tNamesList, nSubSearch);	
			}
		}
		else if(strSkipName.empty())
		{
			FF_GATHER_FILE_INFO gather_file_info;
			gather_file_info.strPrePath		= strPrePath;
			gather_file_info.strAddPath		= strAddPath;
			gather_file_info.strFileName	= strFileName;
			gather_file_info.strFullPath	= strFilePath;

			tNamesList.push_back(gather_file_info);
		}
		else
		{
			String strFileExtension = FindExtension((LPSTR)strFileName.c_str());
//			strFileExtension.MakeLower();
//			strFileExtension = _strlwr(strFileExtension.c_str());
			if(strSkipName.find(strFileExtension) == string::npos)
			{
				FF_GATHER_FILE_INFO gather_file_info;
				gather_file_info.strPrePath		= strPrePath;
				gather_file_info.strAddPath		= strAddPath;
				gather_file_info.strFileName	= strFileName;
				gather_file_info.strFullPath	= strFilePath;
				tNamesList.push_back(gather_file_info);
			}
		}
	}

    closedir(dp);
	return tNamesList.size();
}
//---------------------------------------------------------------------------

LPCTSTR	CFileUtil::FindExtension(LPTSTR lpFileName)
{
	INT32 nLen = 0;

	if(lpFileName == NULL || lpFileName[0] == 0)
		return NULL;
	
	nLen = strlen(lpFileName);

	for(INT32 i = nLen -1; i > -1; i--)
	{
		if(lpFileName[i] == '.')
			return (LPCTSTR)&(lpFileName[i]);
	}
	return NULL;
}
//---------------------------------------------------------------------------

String		CFileUtil::FindFileName(String strFullName)
{
	LPCTSTR szFullName = strFullName.c_str();
	CHAR szFileName[MAX_FILE_NAME] = {0, };
	String strFileName;
	INT32 nLen = strFullName.length();
	INT32 nRtnLen = 0;

	for(INT32 i = nLen - 1; i > -1; i--)
	{
		if(szFullName[i] == '/')
		{
			if(nRtnLen)
			{
				strncpy(szFileName, &szFullName[i+1], MAX_FILE_NAME-1);
				strFileName = szFileName;
				return strFileName;
			}
			return "";
		}
		nRtnLen += 1;
	}
	return "";
}
//---------------------------------------------------------------------------

String		CFileUtil::FindFilePath(String strFullName)
{
	String strPath;
	INT32 nLen = strFullName.length();
	CHAR szFullName[CHAR_MAX_SIZE] = {0, };
	if(nLen > CHAR_MAX_SIZE-1)
		nLen = CHAR_MAX_SIZE-1;
	memcpy(szFullName, strFullName.c_str(), nLen);

	for(INT32 i = nLen - 1; i > -1; i--)
	{
		if(szFullName[i] == '/')
		{
			szFullName[i] = 0;
			break;
		}
	}
	strPath = szFullName;
	return strPath;
}

INT32 CFileUtil::GetFileDateTime(LPCSTR lpFileName, UINT32 *pnCDT, UINT32 *pnMDT, UINT32 *pnADT)
{
	UINT32 nFileDT = 0;
	UINT32 nCurCDT = 0, nCurADT = 0, nCurMDT = 0;

	if(lpFileName == NULL || pnCDT == NULL || pnMDT == NULL || pnADT == NULL)
		return -1;

	if(GetFileTimeInfo(lpFileName, &nCurCDT, &nCurMDT, &nCurADT) != 0)
	{
		return -2;	
	}
	
	*pnCDT = nCurCDT;
	*pnMDT = nCurMDT;
	*pnADT = nCurADT;
	
	return 0;
}


//---------------------------------------------------------------------------

DWORD	CFileUtil::GetFileSizeExt(LPCTSTR lpFileName)
{
	struct stat stStat;
	DWORD dwFileSize = 0;
	if(lpFileName == NULL)
	{
		return 0;
	}
	if(stat(lpFileName, &stStat) < 0)
	{
		return 0;
	}

	if (!S_ISREG(stStat.st_mode))
	{
		return 0;
	}
	dwFileSize = (DWORD)stStat.st_size;
	return dwFileSize;
}
//---------------------------------------------------------------------------

DWORD	CFileUtil::GetFileSizeExtW(const wchar_t *pwFileName)
{
	char acFilePath[MAX_PATH] = {0,};
	if(pwFileName == NULL || pwFileName[0] == L'\0')
	{
		return 0;
	}

	if(wcstombs(acFilePath, pwFileName, MAX_PATH-1) < 0)
	{
		return 0;
	}
	acFilePath[MAX_PATH-1] = 0;
	return GetFileSizeExt(acFilePath);
}
//---------------------------------------------------------------------------

UINT64	CFileUtil::GetDirSizeExt(LPCTSTR lpPath, TMapStrID* tNameSizeMap)
{
	UINT64 nRtn = 0;
	TListGatherFileInfo tNameList;
	GatherFileNameList(lpPath, "", "", tNameList, -1);

	{
		TListGatherFileInfoItor begin, end;
		begin = tNameList.begin();	end = tNameList.end();
		for(begin; begin != end; begin++)
		{
			String strFilePath = begin->strFullPath;
			
			if(tNameSizeMap)
			{
				TMapStrIDItor find = tNameSizeMap->find(strFilePath);
				if(find != tNameSizeMap->end())
				{
					nRtn += find->second;
					continue;
				}
			}

			DWORD nFileSize = GetFileSizeExt(strFilePath.c_str());
			if(tNameSizeMap)
			{
				(*tNameSizeMap)[strFilePath.c_str()] = nFileSize;
			}
			nRtn += nFileSize;
		}
	}
	return nRtn;
}
//---------------------------------------------------------------------------


INT32	CFileUtil::ForceDirectory(LPCTSTR lpPath)
{
	INT32 nLen = 0;
	INT32 nIndex = 0;
	char pszPath[MAX_PATH] = {0, };

	if(lpPath == NULL || lpPath[0] == 0)
		return -1;

	nLen = strlen(lpPath);
	
	while(nIndex < nLen)
	{
		if(lpPath[nIndex] == '/' || lpPath[nIndex] == '\\')
		{
			if(nIndex)
				mkdir(pszPath, 0755);
			pszPath[nIndex] = '/';
		}
		else
		{
			pszPath[nIndex] = lpPath[nIndex];
		}
		nIndex++;

	}
	
	if(nLen)
		mkdir(pszPath, 0755);
	return 0;
}
//---------------------------------------------------------------------------

INT32	CFileUtil::ForceDirectoryW(const wchar_t *pwPath)
{
	char acFilePath[MAX_PATH] = {0,};
	if(pwPath == NULL || pwPath[0] == L'\0')
		return -1;

	if(wcstombs(acFilePath, pwPath, MAX_PATH-1) < 0)
	{
		return -2;
	}
	acFilePath[MAX_PATH-1] = 0;
	return ForceDirectory(acFilePath);
}
//---------------------------------------------------------------------------

HANDLE	CFileUtil::OpenFileHandle(String strFileName)
{
	HANDLE hFile = NULL;
	unlink(strFileName.c_str());
	
	hFile = fopen(strFileName.c_str(), "w+");
	if(hFile == NULL)
		return NULL;
	
	return hFile;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::ReadFileBuffer(String strFileName, String& strBuffer)
{
    DWORD dwFileSize = 0;
    DWORD dwReadSize = 0;
    int nOnceReadSize = 0;
	char pszBuff[CHAR_MAX_SIZE + 1] = {0, };
	FILE* hFile = NULL;
	
    dwFileSize = GetFileSizeExt(strFileName.c_str());
	if(dwFileSize == 0)
		return 0;
	
	hFile = fopen(strFileName.c_str(), "r");
	if(hFile == NULL)
        return 0;

    while(dwReadSize < dwFileSize)
    {
		nOnceReadSize = (int)fread(pszBuff, 1, CHAR_MAX_SIZE, hFile);
		if(nOnceReadSize < 1)
			break;

		pszBuff[CHAR_MAX_SIZE] = 0;
		
		dwReadSize += (DWORD)nOnceReadSize;
		strBuffer += pszBuff;
    }
    fclose(hFile);
    return dwReadSize;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::ReadFileBuffer(String strFileName, PBYTE* lpBuffer, UINT32& dwBufLen)
{
    FILE* hFile = NULL;
    DWORD dwReadSize = 0;
    DWORD dwOnceReadSize = CHAR_MAX_SIZE;
    DWORD dwSize = 0;

	if(lpBuffer == NULL)
		return 0;

	dwBufLen = GetFileSizeExt(strFileName.c_str());
	if(dwBufLen == 0)
		return 0;
	
	hFile = fopen(strFileName.c_str(), "r");
    if(hFile == NULL)
        return 0;

	*lpBuffer = (PBYTE)malloc(dwBufLen + 1);
	if(*lpBuffer == NULL)
	{
		fclose(hFile);
		return 0;
	}
	memset(*lpBuffer, 0, dwBufLen + 1);

	if(dwOnceReadSize > dwBufLen)
		dwOnceReadSize = dwBufLen;

	dwReadSize = 0;
    while(dwReadSize < dwBufLen)
    {
		dwSize = (int)fread(*lpBuffer + dwReadSize, 1, dwOnceReadSize, hFile);
		if(dwSize < 1)
			break;
   
        dwReadSize += dwSize;		
    }
	
    fclose(hFile);
    return dwReadSize;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::ReadFileBuffer(String strFileName, PVOID pBuffer, UINT32 dwBuffSize)
{
    FILE* hFile = NULL;
    DWORD dwFileSize = 0;
	DWORD dwSize = 0;
    DWORD dwReadSize = 0;
	DWORD dwOnceReadSize = CHAR_MAX_SIZE;
	char *pSaveBuff = NULL;
	
	if(pBuffer == NULL || dwBuffSize == 0)
		return 0;

	dwFileSize = GetFileSizeExt(strFileName.c_str());
	if(dwFileSize == 0)
		return 0;

	hFile = fopen(strFileName.c_str(), "r");
    if(hFile == NULL)
        return 0;

	if(dwFileSize > dwBuffSize)
		dwFileSize = dwBuffSize;

	if(dwOnceReadSize > dwFileSize)
		dwOnceReadSize = dwFileSize;
	
	pSaveBuff = (char *)pBuffer;

	dwReadSize = 0;
	while(dwReadSize < dwBuffSize)
	{
		dwSize = (int)fread(pSaveBuff + dwReadSize, 1, dwOnceReadSize, hFile);
		if(dwSize < 1)
			break;
        dwReadSize += dwSize;		
	}

	fclose(hFile);
	return dwReadSize;
}
//---------------------------------------------------------------------------

HANDLE	CFileUtil::FirstReadFileBuff(String strFileName, PBYTE pFileBuff, INT32 nBufLen)
{
	FILE* hFile = NULL;
	DWORD dwReadSize = 0;
	DWORD dwOnceReadSize = 0;
	hFile = fopen(strFileName.c_str(), "r");
    if(hFile == NULL)
        return 0;

    memset(pFileBuff, 0, nBufLen);
    
    while(dwReadSize < (DWORD)nBufLen)
    {
		dwOnceReadSize = (DWORD)fread(pFileBuff + dwReadSize, 1, (DWORD)nBufLen - dwReadSize, hFile);
		if((int)dwOnceReadSize < 1)
			break;
		dwReadSize += dwOnceReadSize;
    }
	return (HANDLE)hFile;
}
//---------------------------------------------------------------------------

INT32	CFileUtil::NextReadFileBuff(HANDLE hFile, PBYTE pFileBuf, INT32 nBufLen)
{
	DWORD dwOnceReadSize = 0;
	INT32 nTryCnt = 10;
	DWORD dwReadSize = 0;
	FILE* fp = (FILE*)hFile;
	if(fp == NULL)
		return 0;

	memset(pFileBuf, 0, nBufLen);

	while(dwReadSize < (DWORD)nBufLen && nTryCnt)
	{
		nTryCnt--;

		dwOnceReadSize = (DWORD)fread(pFileBuf + dwReadSize, 1, (DWORD)nBufLen - dwReadSize, fp);
		if((int)dwOnceReadSize < 1)
			break;
		dwReadSize += dwOnceReadSize;
	}
	return dwReadSize;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::WriteFileBuffer(String strFileName, String strBuffer)
{
	FILE* hFile = NULL;
	char* pszBuffer = NULL;
	DWORD dwWritedByte = 0;
    INT32 dwWritedSize = 0;
	DWORD dwStrLength = 0;
	
	unlink(strFileName.c_str());

    hFile = fopen(strFileName.c_str(), "w+");
    if(hFile == NULL)
    {
		return -1;
    }

    pszBuffer = (char*)strBuffer.c_str();
	dwStrLength = (DWORD)strBuffer.length();

    while(dwWritedSize < dwStrLength)
    {
		dwWritedByte = (DWORD)fwrite(&(pszBuffer[dwWritedSize]), 1, dwStrLength - dwWritedSize, hFile);
		if((int)dwWritedByte < 1)
			break;
		dwWritedSize += dwWritedByte;
    }
	fclose(hFile);
    return 0;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::WriteFileBuffer(String strFileName, PBYTE lpBuffer, UINT32 dwBufLen)
{
	FILE* hFile = NULL;
	DWORD dwWritedByte = 0;
	UINT32 dwWritedSize = 0;

	if(lpBuffer == NULL || dwBufLen < 1)
		return -1;
	
	unlink(strFileName.c_str());
    hFile = fopen(strFileName.c_str(), "w+");
    if(hFile == NULL)
    {
		return -2;
    }

	while(dwWritedSize < dwBufLen)
	{
		dwWritedByte = (DWORD)fwrite(&(lpBuffer[dwWritedSize]), 1, dwBufLen - dwWritedSize, hFile);
		if((int)dwWritedByte < 1)
			break;
		dwWritedSize += dwWritedByte;
	}
	fclose(hFile);
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CFileUtil::WriteFileBuffer(HANDLE hFile, PBYTE lpBuffer, UINT32 dwBufLen)
{
	FILE* fp = (FILE*)hFile;
	DWORD dwWritedByte = 0;
	UINT32 dwWritedSize = 0;

	if(fp == NULL || lpBuffer == NULL || dwBufLen < 1)
	{
		return -1;
	}

	while(dwWritedSize < dwBufLen)
	{
		dwWritedByte = (DWORD)fwrite(&(((PCHAR)lpBuffer)[dwWritedSize]), 1, dwBufLen - dwWritedSize, fp);
		if((int)dwWritedByte < 1)
			break;
		dwWritedSize += dwWritedByte;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CFileUtil::GetherUsersDirNameList(String strPrePath, TListGatherFileInfo& tNamesList)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath_UsersEnvByDirPath(TListStr& tPathList, String strPreKey, UINT32 nRemainLog)
{

	return 0;
}

//---------------------------------------------------------------------------

String		CFileUtil::GetEnvPathToLocalPathSingle(String strEnvPath, UINT32 nRemainLog)
{
	TListStr tLocalPathList;
	GetEnvPathToLocalPath(strEnvPath, &tLocalPathList, nRemainLog);
	
	if(tLocalPathList.size() == 0)
	{
		return strEnvPath;
	}

	return *(tLocalPathList.begin());
}
//---------------------------------------------------------------------------

void		CFileUtil::SetEnvPathRegKey(String strRegKey)
{
	m_strEnvPathKey = strRegKey;
	return;
}
//---------------------------------------------------------------------------

void		CFileUtil::SetEnvPathKey(String strName, String strData)
{
}
//---------------------------------------------------------------------------

String		CFileUtil::GetEnvPathKey(String strName)
{
	return "";
}
//---------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath(String strEnvPath, TListStr* pLocalPathList, UINT32 nRemainLog, UINT32 nEnvWaitSec)
{
	TListStr tPathList;
	TListStrItor begin, end;
	String strTempFP;
	char *ppKeyword[6] = {"<USERS_HOME>", "<USERS_HOME_EX>", "<DESKTOP>", "<DOWNLOAD>", "<DOCUMENTS>", ""};
	if(pLocalPathList == NULL)
		return -1;

	for(int i=0; i<5; i++)
	{
		if(ppKeyword[i][0] == 0)
			break;

		if(strEnvPath.find(ppKeyword[i]) != string::npos)
		{
			GetEnvPathToLocalPath_UsersEnv(tPathList, ppKeyword[i], nRemainLog, nEnvWaitSec);

			begin = tPathList.begin();	end = tPathList.end();
			for(begin; begin != end; begin++)
			{
				strTempFP = strEnvPath;
				if(i==0)
					strTempFP.replace(0,12,*begin);
				else if(i==1)
					strTempFP.replace(0,15,*begin);
				else if(i==2)
					strTempFP.replace(0,9,*begin);
				else if(i==3)
					strTempFP.replace(0,10,*begin);
				else if(i==4)
					strTempFP.replace(0,11,*begin);
				else
					break;

				if(nRemainLog)
					WriteLogN("convert keyword path : [%s]->[%s]", strEnvPath.c_str(), strTempFP.c_str());
				pLocalPathList->push_back(strTempFP);
			}
			return 0;
		}
	}
	pLocalPathList->push_back(strEnvPath);
	return 0;
}
//---------------------------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath_KorPath(char *pcUseDir, char *pcPreKey, String& strUserSubKorPath)
{
	DIR *dp = NULL;
	INT32 nRetVal = 0;
	struct dirent *dirp = NULL;
	char ppDirName[2][64] = {"문서", "다운로드"};
	char *pUtf8 = NULL;
	if(pcUseDir == NULL || pcUseDir[0] == 0 || pcPreKey == NULL || pcPreKey[0] == 0)
		return -1;

	if(!_stricmp(pcPreKey, "<DOCUMENTS>"))
		nRetVal = conver_utf8(ppDirName[0], strlen(ppDirName[0])+2, &pUtf8);
	else if(!_stricmp(pcPreKey, "<DOWNLOAD>"))
		nRetVal = conver_utf8(ppDirName[1], strlen(ppDirName[1])+2, &pUtf8);

	if(nRetVal != 0 || pUtf8 == NULL)
	{
		WriteLogN("fail to convert utf8 : [%d]", nRetVal);
		return -1;
	}

	if(DirectoryExists(pcUseDir) == FALSE)
	{
		safe_free(pUtf8);
		return -2;
	}

	dp = opendir(pcUseDir);
	if (dp == NULL)
	{
		safe_free(pUtf8);
		return -3;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}

		if(DT_DIR == dirp->d_type)
		{
			if(!_stricmp(dirp->d_name, pUtf8))
			{
				strUserSubKorPath = SPrintf("%s/%s", pcUseDir, dirp->d_name);
				closedir(dp);
				safe_free(pUtf8);
				return 0;
			}
		}
	}

	closedir(dp);
	safe_free(pUtf8);

	return -4;
}
//---------------------------------------------------------------------------------------------

INT32	CFileUtil::IsCurrentLoginUser(char *pUserName)
{
	TListStrItor begin, end;
	if(pUserName == NULL || pUserName[0] == 0)
		return -1;

	if(m_tUserList.size() == 0)
		return -2;

	begin = m_tUserList.begin();	end = m_tUserList.end();
	for(begin; begin != end; begin++)
	{
		if(!_stricmp(begin->c_str(), pUserName))
			return 0;
	}
	return -3;
}

VOID	CFileUtil::GetCurrentLoginUser()
{
	FILE    *fp = NULL;
	int     i, nLen = 0;
	char    acBuf[MAX_QHBUFF] = {0,};

	m_tUserList.clear();

	fp = popen(CMD_GET_LOGIN_USER, "r");
	if (fp == NULL)
	{
		WriteLogE("fail to open %s (%d)", CMD_GET_LOGIN_USER, errno);
		return;
	}

	while (fgets(acBuf, MAX_QHBUFF-1, fp))
	{
		nLen = strlen(acBuf);
		if(nLen > MAX_QHBUFF-1 || nLen < 1)
		{
			memset(acBuf, 0, MAX_QHBUFF);
			continue;
		}

		for(i=0; i<nLen; i++)
		{
			if(acBuf[i] == 0x20)
			{
				acBuf[i] = 0;
				break;
			}
		}

		if(acBuf[0] == 0)
		{
			memset(acBuf, 0, MAX_QHBUFF);
			continue;
		}

		if(IsCurrentLoginUser(acBuf) != 0)
		{
			m_tUserList.push_back(acBuf);
		}

		memset(acBuf, 0, MAX_QHBUFF);
	}
	pclose(fp);
}


INT32		CFileUtil::GetEnvPathToLocalPath_UsersEnv(TListStr& tPathList, char *pcPreKey, UINT32 nRemainLog, UINT32 nEnvWaitSec)
{
	TListStr tKeyNameList;
	DIR *dp = NULL;
	char acHome[MAX_QQBUFF] = "/home";
	struct dirent *dirp = NULL;
	String strUserPath, strUserSubEngPath, strUserSubKorPath;
	
	if(pcPreKey == NULL || pcPreKey[0] == 0)
	{
		return -1;
	}
	if(DirectoryExists(acHome) == FALSE)
	{
		return -2;
	}

	dp = opendir(acHome);
	if (dp == NULL)
	{
		return -3;
	}

	if(!_strnicmp(pcPreKey, "<USERS_HOME>", 12))
		GetCurrentLoginUser();

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}

		if(DT_DIR == dirp->d_type)
		{
			strUserPath = SPrintf("%s/%s", acHome, dirp->d_name);
			if(!_strnicmp(pcPreKey, "<USERS_HOME_EX>", 15))
			{
				tPathList.push_back(strUserPath);
			}
			else if(!_strnicmp(pcPreKey, "<USERS_HOME>", 12))
			{
				if(IsCurrentLoginUser((char *)dirp->d_name) == 0)
					tPathList.push_back(strUserPath);
			}
			else if(!_strnicmp(pcPreKey, "<DOWNLOAD>", 10))
			{
				strUserSubEngPath = SPrintf("%s/%s/Downloads", acHome, dirp->d_name);
				if(DirectoryExists(strUserSubEngPath.c_str()) == TRUE)
				{
					tPathList.push_back(strUserSubEngPath);
				}
				else
				{
					if(GetEnvPathToLocalPath_KorPath((char *)strUserPath.c_str(), pcPreKey, strUserSubKorPath) == 0)
					{
						tPathList.push_back(strUserSubKorPath);
					}
				}
			}
			else if(!_strnicmp(pcPreKey, "<DOCUMENTS>", 11))
			{
				strUserSubEngPath = SPrintf("%s/%s/Documents", acHome, dirp->d_name);
				if(DirectoryExists(strUserSubEngPath.c_str()) == TRUE)
				{
					tPathList.push_back(strUserSubEngPath);
				}
				else
				{
					if(GetEnvPathToLocalPath_KorPath((char *)strUserPath.c_str(), pcPreKey, strUserSubKorPath) == 0)
					{
						tPathList.push_back(strUserSubKorPath);
					}
				}
			}
			else if(!_strnicmp(pcPreKey, "<DESKTOP>", 9))
			{
				strUserSubEngPath = SPrintf("%s/%s/Desktop", acHome, dirp->d_name);
				if(DirectoryExists(strUserSubEngPath.c_str()) == TRUE)
				{
					tPathList.push_back(strUserSubEngPath);
				}
			}
		}
	}

	closedir(dp);

	return 0;
}
//---------------------------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath_EnvPath(HKEY nRegKey, String strRegRoot, String strEnvValue, String& strPathValue)
{
	return 0;
}
//---------------------------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath_TokInfo(String strValue, String& strEnvKey, String& strRemainValue)
{
	CHAR chFindChar = 37;
	strEnvKey = "", strRemainValue = "";

	if(strValue.find(&chFindChar) == string::npos)	return 0;

	INT32 nFindStart = 0;
	INT32 nIdx = 0;
	while(nIdx < strValue.length())
	{
		CHAR szChar = strValue.at(nIdx++);
		if(szChar == chFindChar)
		{
			nFindStart += 1;
			continue;
		}

		switch(nFindStart)
		{
			case 1:		
				strEnvKey += SPrintf("%c", szChar);
				break;
			case 2:
				strRemainValue += SPrintf("%c", szChar);
				break;
		}
	}
	return 1;
}
//---------------------------------------------------------------------------------------------

INT32		CFileUtil::GetEnvPathToLocalPath_WindowsUpdate(TListStr& tPathList, String strPreKey, UINT32 nRemainLog)
{
	INT32 nRtn = 0;
	return nRtn;
}
//---------------------------------------------------------------------------------------------

INT32		CFileUtil::IsExistFileDateTime(UINT32 nChkFDTType, UINT32 nChkFDValue, String strFilePath, String strFileName)
{
	UINT32 nCurCDT = 0, nCurADT = 0, nCurMDT = 0;
	UINT32 nCurDT = GetCurrentDateTimeInt();
	INT32 nRetVal = 0;
	UINT32 nExistInfo = 0;
	String strChkFileName;

	if(strFilePath.length() != 0)
	{
		if(strFileName.length() != 0)
			strChkFileName = SPrintf("%s/%s", strFilePath.c_str(), strFileName.c_str());
		else
			strChkFileName = strFilePath;

		nRetVal = GetFileTimeInfo(strChkFileName.c_str(), &nCurCDT, &nCurMDT, &nCurADT);
		if(nRetVal != 0)
		{
			nCurCDT = 0;
			nCurADT = 0;
			nCurMDT = 0;
		}
	}	
	switch(nChkFDTType)
	{
		case ASI_FF_FILE_DT_CHK_TYPE_CREATE:
		{
			if(nChkFDValue == 0 || (nCurCDT && nCurCDT < (nCurDT - nChkFDValue)))
			{
				nExistInfo = 1;
			}
			break;
		}
		case ASI_FF_FILE_DT_CHK_TYPE_ACCESS:
		{
			if(nChkFDValue == 0 || (nCurADT && nCurADT < (nCurDT - nChkFDValue)))
			{
				nExistInfo = 1;
			}
			break;
		}
		case ASI_FF_FILE_DT_CHK_TYPE_WRITE:
		{
			if(nChkFDValue == 0 || (nCurMDT && nCurMDT < (nCurDT - nChkFDValue)))
			{
				nExistInfo = 1;
			}
			break;
		}
	}

	return nExistInfo;
}

//---------------------------------------------------------------------------------------------

String		CFileUtil::GetNewFileName(String strFileFullName, INT32 nOrUsed, String strAppand)
{
	String strFilePath, strFileName;
	CHAR szFilePath[CHAR_MAX_SIZE] = {0, };
	char szFileName[MAX_HBUFF] = {0,};

	strncpy(szFilePath, strFileFullName.c_str(), CHAR_MAX_SIZE-1);
	szFilePath[CHAR_MAX_SIZE-1] = 0;

	if(get_basename(szFilePath, szFileName, MAX_HBUFF-1) != NULL)
	{
		szFileName[MAX_HBUFF-1] = 0;
		strFileName = szFileName;
	}
	if(get_dirname(szFilePath, szFilePath, CHAR_MAX_SIZE-1) != NULL)
	{
		strFilePath = szFilePath;
	}
	
	return GetNewFileName(strFilePath, strFileName, nOrUsed, strAppand);
}
//---------------------------------------------------------------------------------------------

String		CFileUtil::GetNewFileName(String strFilePath, String strFileName, INT32 nOrUsed, String strAppand)
{
	String strChkFullName = "";
	String strChkFName, strChkFExt, strNewFileName;

	{
		CHAR szFile[MAX_FILE_NAME] = {0, };
		if(remove_extname((char *)strFileName.c_str(), szFile, MAX_FILE_NAME) != NULL)
			strChkFName = szFile;
		else
			strChkFName = strFileName;
	}
	
	{
		CHAR szExt[MAX_FILE_NAME] = {0, };
		if(get_extname((char *)strFileName.c_str(), szExt, MAX_FILE_NAME) != NULL)
			strChkFExt = &szExt[1];
		else
			strChkFExt = strFileName;
	}

	INT32 nAutoInc = 0;
	do 
	{
		if(!nAutoInc && nOrUsed)
			strNewFileName = SPrintf("%s.%s", strChkFName.c_str(), strChkFExt.c_str());
		else
			strNewFileName = SPrintf("%s(%s%d).%s", strChkFName.c_str(), strAppand.c_str(), nAutoInc, strChkFExt.c_str());

		strChkFullName = SPrintf("%s/%s", strFilePath.c_str(), strNewFileName.c_str());
		nAutoInc += 1;

	} while (FileExists(strChkFullName.c_str()));

	return strNewFileName;
}

UINT32 CFileUtil::FindFileList(TListStr& tListFiles, String strPath, BOOL bIncludeSubPath, String strExtFilter)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	String strFilePath;
	tListFiles.clear();

	if (strPath.empty())
		return tListFiles.size();

//	PathAddBackslash(strPath.GetBuffer(BUFSIZ));
//	strPath.ReleaseBuffer();
//	strPath.append(strExtFilter);

	if(DirectoryExists(strPath.c_str()) == FALSE)
	{
		return 0;
	}

	dp = opendir(strPath.c_str());
	if (dp == NULL)
	{
		return 0;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		if(!_stricmp(dirp->d_name, ".") || !_stricmp(dirp->d_name, ".."))
		{
			continue;
		}
		strFilePath = SPrintf("%s/%s", strPath.c_str(), dirp->d_name);
		if (bIncludeSubPath && DT_DIR == dirp->d_type)
		{
			FindFileList(tListFiles, strFilePath, bIncludeSubPath, strExtFilter);
		}
		else if(DT_REG == dirp->d_type)
		{
			tListFiles.push_back(strFilePath);
		}
	}
	closedir(dp);
	return tListFiles.size();
}
//---------------------------------------------------------------------------------------------
