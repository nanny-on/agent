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

#include "stdafx.h"

#include "as_file.h"
#include "as_string.h"
#include "as_registry.h"
#include "com_define_string.h"

static vector<String> listRegKey;

long RegCreateKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD dwReserved, LPTSTR lpClass, DWORD dwOption, DWORD dwSam, void* lpSecutiry, HKEY* phkResult, DWORD* lpdwDisposition, char *acLogMsg)
{
	INT32 nLen = 0;
	char acKeyPath[CHAR_MAX_SIZE] = {0,};
	if(hKey == NULL || lpSubKey == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegCreateKeyEx] invalid input data");
		return -1;
	}

	snprintf(acKeyPath, CHAR_MAX_SIZE-1, "%s/%s/%s", INSTALL_REG_PATH, (char*)hKey, lpSubKey);
	acKeyPath[CHAR_MAX_SIZE-1] = 0;

	if(DirectoryExists(acKeyPath) == FALSE)
	{
		if(ForceDirectories(acKeyPath, acLogMsg) == FALSE)
		{
			return -1;
		}
	}

	nLen = (INT32)strlen(acKeyPath);
	if(nLen < 1 || nLen > CHAR_MAX_SIZE-1)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegCreateKeyEx] invalid key path (%d)", nLen);
		return -1;
	}

	char *pTemp = (char *)malloc(CHAR_MAX_SIZE);
	if(pTemp == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegCreateKeyEx] fail to allocate memory (len:%d) (%d)", nLen, errno);
		return -1;
	}
	strncpy(pTemp, acKeyPath, CHAR_MAX_SIZE-1);
	pTemp[CHAR_MAX_SIZE-1] = 0;
	*phkResult = (HKEY)pTemp;
	return 0;
}

long RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName, DWORD* lpReserved, DWORD* lpType, BYTE* lpData, DWORD dwMaxLen, DWORD* lpcbData, char *acLogMsg)
{
	long lRet = -1;
	DWORD dwRead = 0;
	char acFilePath[CHAR_MAX_SIZE] = {0,};

	if(hKey == NULL || dwMaxLen == 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegQueryValueEx] invalid input data");
		return -1;
	}

	String strLower = _strlwr(lpValueName);
	if(strLower.length() == 0)
		strLower = "default";

	snprintf(acFilePath, CHAR_MAX_SIZE-1, "%s/%s", (char*)hKey, strLower.c_str());
	acFilePath[CHAR_MAX_SIZE-1] = 0;

	FILE* fp = NULL;
	fp = fopen(acFilePath, "r");
	if(fp == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegQueryValueEx] fail to open %s (%d)", acFilePath, errno);
		return -1;
	}

	unsigned char szBuf[1024] = {0,};
	DWORD dwType = 0;

	do{
		if(fread(&dwType, 1, sizeof(DWORD), fp) != sizeof(DWORD))
		{
			lRet = -1;
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "[RegQueryValueEx] fail to read %s (%d)", acFilePath, errno);
			break;
		}
		dwRead = (DWORD)fread(szBuf, 1, sizeof(szBuf)-1, fp);
		if(dwRead == 0)
		{
			lRet = -1;
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "[RegQueryValueEx] fail to read %s (%d)", acFilePath, errno);
			break;
		}
		if(dwMaxLen < dwRead)
		{
			dwRead = dwMaxLen;
		}
		if(lpType)
			*lpType = dwType;
		if(lpcbData)
			*lpcbData = dwRead;
		if(dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ)
		{
			if(lpcbData)
				(*lpcbData)++;
		}
		if(lpData)
			memcpy(lpData, szBuf, dwRead);
		lRet = 0;
	}while(FALSE);

	fclose(fp);

	return lRet;
}

long RegSetValueEx(HKEY hKey, LPCTSTR lpValueName, DWORD dwReserved, DWORD dwType, const BYTE* lpData, DWORD cdData, char *acLogMsg)
{
	long lRet = -1;
	char acFilePath[CHAR_MAX_SIZE] = {0,};

	if(hKey == NULL || lpValueName == NULL || lpData == NULL || cdData == 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegSetValueEx] invalid input data");
		return -1;
	}

	String strLower = _strlwr(lpValueName);
	if(strLower.length() == 0)
		strLower = "default";

	snprintf(acFilePath, CHAR_MAX_SIZE-1, "%s/%s", (char*)hKey, strLower.c_str());
	acFilePath[CHAR_MAX_SIZE-1] = 0;

	FILE* fp = NULL;
	fp = fopen(acFilePath, "w");
	if(fp == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegSetValueEx] fail to open %s (%d)", acFilePath, errno);
		return -1;
	}

	do{
		if(fwrite(&dwType, 1, sizeof(DWORD), fp) != sizeof(DWORD))
		{
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "[RegSetValueEx] fail to write %s (%d)", acFilePath, errno);
			lRet = -1;
			break;
		}

		if(fwrite(lpData, 1, cdData, fp) == 0)
		{
			if(acLogMsg != NULL)
				snprintf(acLogMsg, MAX_LOGMSG, "[RegSetValueEx] fail to write %s (len : %d) (%d)", acFilePath, cdData, errno);
			lRet = -1;
			break;
		}
		lRet = 0;
	}while(FALSE);

	fclose(fp);

	return lRet;
}

VOID RegCloseKey(HKEY hKey)
{
	if(hKey != NULL)
	{
		free(hKey);
	}

	if(!listRegKey.empty())
		listRegKey.clear();
}

long RegEnumKey(HKEY hKey, DWORD dwIndex, LPTSTR lpName, DWORD ccName)
{
	if(listRegKey.empty())
	{
		DIR *dp;
		struct dirent *dirp;
		dp = opendir((char*)hKey);
		if (dp == NULL)
		{
			return -1 ;
		}
		while((dirp = readdir(dp)) != NULL)
		{
			if(DT_REG == dirp->d_type && strcmp(dirp->d_name, "default"))
			{
				listRegKey.push_back(dirp->d_name);
			}
		}
		closedir(dp);
	}

	if(listRegKey.empty())
		return -1;
	if(listRegKey.size() <= dwIndex)
		return ERROR_NO_MORE_ITEMS;

	if(ccName <= listRegKey[dwIndex].length())
		return ERROR_MORE_DATA;

	strncpy(lpName, listRegKey[dwIndex].c_str(), ccName-1);

	return 0;
}

long RegOpenKey(HKEY hKey, LPCTSTR lpSubKey, HKEY* phkResult, char *acLogMsg)
{
	char acPath[CHAR_MAX_SIZE] = {0,};
	DWORD dwLen = 0;
	if(hKey == NULL || lpSubKey == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegOpenKey] invalid input data");
		return -1;
	}
	snprintf(acPath, CHAR_MAX_SIZE-1, "%s/%s/%s", INSTALL_REG_PATH, (char*)hKey, lpSubKey);
	acPath[CHAR_MAX_SIZE-1] = 0;

	if(is_file(acPath) < 0)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegOpenKey] invalid directory %s (%d)", acPath, errno);
		return -1;
	}

	dwLen = (DWORD)strlen(acPath);
	if(dwLen == 0 || dwLen > CHAR_MAX_SIZE-1)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegOpenKey] invalid directory length (%d)", dwLen);
		return -1;
	}

    char *pTemp = (char *)malloc(CHAR_MAX_SIZE);
	if(pTemp == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegOpenKey] invalid directory %s (%d)", acPath, errno);
		return -1;
	}

	strncpy(pTemp, acPath, CHAR_MAX_SIZE-1);
	pTemp[CHAR_MAX_SIZE-1] = 0;

    *phkResult = (HKEY)pTemp;

	return 0;
}

long RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, DWORD samDesired, HKEY* phkResult, char *acLogMsg)
{
	return RegOpenKey(hKey, lpSubKey, phkResult, acLogMsg);
}

long RegDeleteKey(HKEY hKey, LPCTSTR lpSubKey, char *acLogMsg)
{
	char acCmd[CHAR_MAX_SIZE] = {0,};
	if(hKey == NULL || lpSubKey == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegDeleteKey] invalid input data");
		return -1;
	}

	snprintf(acCmd, CHAR_MAX_SIZE-1, "%s/%s/%s", INSTALL_REG_PATH, (char*)hKey, lpSubKey);
	acCmd[CHAR_MAX_SIZE-1] = 0;

	if(is_file(acCmd) < 0)
	{
		return 0;
	}

	snprintf(acCmd, CHAR_MAX_SIZE-1, "rm -rf %s/%s/%s", INSTALL_REG_PATH, (char*)hKey, lpSubKey);
	acCmd[CHAR_MAX_SIZE-1] = 0;

	system(acCmd);

	return 0;
}

long RegDeleteValue(HKEY hKey, LPCTSTR lpValueName, char *acLogMsg)
{
	char acPath[CHAR_MAX_SIZE] = {0,};
	if(hKey == NULL || lpValueName == NULL)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegDeleteValue] invalid input data");
		return -1;
	}

	snprintf(acPath, CHAR_MAX_SIZE-1, "%s/%s", (char*)hKey, lpValueName);
	acPath[CHAR_MAX_SIZE-1] = 0;

	if(is_file(acPath) < 0)
	{
		return 0;
	}

	if(unlink(acPath) == -1)
	{
		if(acLogMsg != NULL)
			snprintf(acLogMsg, MAX_LOGMSG, "[RegDeleteValue] fail to unlink %s (%d)", acPath, errno);
		return -1;
	}
	return 0;
}

long RegEnumValue(HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, DWORD* lpcchValueName, DWORD* lpReserved, DWORD* lpType, LPBYTE lpData, DWORD* lpcbData)
{
	DWORD dwValueLen = 0;
	DIR *dp = NULL;
	struct dirent *dirp;
	if(listRegKey.empty())
	{
		dp = opendir((char*)hKey);
		if (dp == NULL)
		{
			return -1 ;
		}
		while((dirp = readdir(dp)) != NULL)
		{
			if(DT_REG == dirp->d_type && strcmp(dirp->d_name, "default"))
			{
				listRegKey.push_back(dirp->d_name);
			}
		}
		closedir(dp);
	}
	if(listRegKey.empty())
		return -1;
	
	if(listRegKey.size() <= dwIndex)
		return ERROR_NO_MORE_ITEMS;

	dwValueLen = *lpcchValueName;
	if(dwValueLen <= listRegKey[dwIndex].length())
		return ERROR_MORE_DATA;

	strncpy(lpValueName, listRegKey[dwIndex].c_str(), dwValueLen);

	if(lpData == NULL && lpType == NULL && lpcbData == NULL)
		return 0;

	String strLower = _strlwr(lpValueName);
	String strFilePath = SPrintf("%s/%s", (char*)hKey, strLower.c_str());
	FILE* fp = NULL;
	DWORD dwType = 0;
	DWORD dwLen = 0;
	unsigned char *pBuf = NULL;

	pBuf = (unsigned char *)malloc(CHAR_MAX_SIZE);
	if(pBuf == NULL)
		return -1;

	memset(pBuf, 0, CHAR_MAX_SIZE);
	fp = fopen(strFilePath.c_str(), "r");
	if(fp == NULL)
	{
		safe_free(pBuf);
		return -1;
	}

	if(fread(&dwType, 1, sizeof(DWORD), fp) == sizeof(DWORD))
	{
		dwLen = (DWORD)fread(pBuf, 1, CHAR_MAX_SIZE-1, fp);
		if(dwLen > 0)
		{
			if(lpType)
				*lpType = dwType;
			
			if(lpData)
				memcpy(lpData, pBuf, dwLen);

			if(dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ)
			{
				dwLen += 1;
			}

			if(lpcbData)
				*lpcbData = dwLen;
			
		}
	}
	fclose(fp);
	safe_free(pBuf);

	return 0;
}

int get_nanny_agent_root(char *pRootPath, int nRootMax)
{
	HKEY hSubKey = NULL;
	char szPath[MAX_PATH] = {0,};
	DWORD dwDisp = 0;
	DWORD dwType = 0;
	DWORD cbData = MAX_PATH - 1;

	if(pRootPath == NULL || nRootMax < 1)
		return -1;

	if(RegCreateKeyEx((HKEY)HKEY_LOCAL_MACHINE, STR_REG_DEFAULT_SVC_LOCAL_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp) != 0)
	{
		return -2;
	}
	if(RegQueryValueEx(hSubKey, "root_path", 0, &dwType, (PBYTE)szPath, MAX_PATH-1, &cbData) != 0)
	{
		RegCloseKey(hSubKey);
		return -3;
	}
	RegCloseKey(hSubKey);
	strncpy(pRootPath, szPath, nRootMax-1);
	return 0;
}