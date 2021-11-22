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

// RegUtil.cpp: implementation of the CRegUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "as_registry.h"
#include "RegUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegUtil::CRegUtil()
{
	memset(m_lpRootPath, 0, CHAR_MAX_SIZE);
}

CRegUtil::~CRegUtil()
{

}

INT32 CRegUtil::RegReadInt( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT32& nValue )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;	
	UINT dwResult = 0;	
	DWORD dwSize = sizeof(DWORD);
	DWORD dwType = 0;
	INT32 nResult = 0;
	char acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);
	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)&nValue, sizeof(INT32), &dwSize ) != ERROR_SUCCESS )
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);

	if(hSubKey)
		RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegReadInt64( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT64& nValue )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;	
	DWORD dwSize = sizeof(UINT64);	
	INT32 nResult = 0;
	char acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)&nValue, sizeof(INT64), &dwSize ) != ERROR_SUCCESS )
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);

	if(hSubKey)
		RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegReadString( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPSTR pcData, DWORD dwSize )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwLen = 0;	 
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };
	
	if(hKey == NULL || pcData == NULL || dwSize < 1)
	{
		return -1;
	}

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		dwLen = dwSize;
		memset(pcData, 0, dwLen);
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)pcData, dwLen-1, &dwLen ) != ERROR_SUCCESS )
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);

	RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegNotOverWriteInt( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, DWORD dwData)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	UINT dwValue = 0;	
	DWORD dwSize = sizeof(DWORD);
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL)
	{
		return -1;
	}
	
	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);
	
	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}

	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)&dwValue, sizeof(DWORD), &dwSize) != ERROR_SUCCESS ) 
		{
			if( RegSetValueEx( hSubKey, lpValue, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(DWORD) ) != ERROR_SUCCESS )		
			{
				nResult = -3;
				break;
			}	
		}	
		nResult = 0;
	}while(FALSE);
		
	RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegWriteString( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwSize = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL || lpData == NULL)
	{
		return -1;
	}
	
	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		dwSize = (DWORD)strlen(lpData) + 1;  

		if( RegSetValueEx( hSubKey, lpValue, 0, REG_SZ, (LPBYTE)lpData, dwSize ) != ERROR_SUCCESS )		
		{
			nResult = -3;
			break;
		}	
		nResult = 0;
	}while(FALSE);
	
	RegCloseKey( hSubKey );	
	return nResult;
}

INT32 CRegUtil::RegWriteExtString( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwSize = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL || lpData == NULL)
	{
		return -1;
	}
	
	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		dwSize = (DWORD)strlen(lpData) + 1;  

		if( RegSetValueEx( hSubKey, lpValue, 0, REG_EXPAND_SZ, (LPBYTE)lpData, dwSize ) != ERROR_SUCCESS )		
		{
			nResult = -3;
			break;
		}	
		nResult = 0;
	}while(FALSE);

	RegCloseKey( hSubKey );	
	return nResult;
}

INT32 CRegUtil::RegWriteBinary( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData, INT32 nLen )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL || lpData == NULL || nLen < 1)
	{
		return -1;
	}
	
	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);
	
	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}

	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegSetValueEx( hSubKey, lpValue, 0, REG_BINARY, (LPBYTE)lpData, nLen ) != ERROR_SUCCESS )		
		{
			nResult = -3;
			break;
		}	
		nResult = 0;
	}while(FALSE);
	RegCloseKey( hSubKey );	
	return nResult;
}

INT32 CRegUtil::RegNotOverWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwSize = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };
	char acValue[MAX_PATH] = {0, };

	if(hKey == NULL || lpValue == NULL || lpData == NULL)
	{
		return -1;
	}
	
	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);
	
	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}

	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		dwSize = MAX_PATH-1;
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)acValue, MAX_PATH-1, &dwSize) != ERROR_SUCCESS ) 
		{
			dwSize = (DWORD)strlen(lpData) + 1;
			if( RegSetValueEx( hSubKey, lpValue, 0, REG_SZ, (LPBYTE)lpData, dwSize ) != ERROR_SUCCESS )		
			{
				nResult = -3;
				break;
			}	
		}	
		nResult = 0;
	}while(FALSE);
	
	RegCloseKey( hSubKey );	
	return nResult;
}

INT32 CRegUtil::RegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT32 nValue)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL)
	{
		return -1;
	}

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegSetValueEx( hSubKey, lpValue, 0, REG_DWORD, (LPBYTE)&nValue, (int)sizeof(INT32) ) != ERROR_SUCCESS )		
		{
			nResult = -3;
			break;
		}	
		nResult = 0;
	}while(FALSE);

	RegCloseKey( hSubKey );
	return nResult;
}

INT32 CRegUtil::RegWriteInt64(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT64 nData)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL)
	{
		return -1;
	}

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if( RegCreateKeyEx( hKey, acFullKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegSetValueEx( hSubKey, lpValue, 0, REG_BINARY, (LPBYTE)&nData, (int)sizeof(INT64) ) != ERROR_SUCCESS )		
		{
			nResult = -3;
			break;
		}	
		nResult = 0;
	}while(FALSE);

	RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegDeleteValueExt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL || lpValue == NULL)
	{
		return -1;
	}

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	do{
		if(RegOpenKey(hKey, acFullKey, &hSubKey) != ERROR_SUCCESS)
		{
			nResult = -2;
			break;
		}

		if(RegDeleteValue(hSubKey, lpValue) != ERROR_SUCCESS)
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);

	RegCloseKey(hSubKey);
	return nResult;
}

INT32 CRegUtil::RegDeleteKeyExt(HKEY hKey, LPCTSTR lpKey)
{
	INT32 nResult = 0;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_SVC_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	if(RegDeleteKey(hKey, acFullKey) != ERROR_SUCCESS)
	{
		return -2;
	}
	return 0;
}

INT32 CRegUtil::RegDeleteKeySubAll(HKEY hKey, LPCTSTR lpKey)
{
	CHAR acSubKeyName[CHAR_MAX_SIZE] = {0, };

	if(hKey == NULL)
		return -1;

	TListStr tSubKeyNameList;
	GetEnumSubKeyName(hKey, lpKey, tSubKeyNameList, -1);
	TListStrItor begin, end;
	begin = tSubKeyNameList.begin();	end = tSubKeyNameList.end();
	for(begin; begin != end; begin++)
	{
		snprintf(acSubKeyName, CHAR_MAX_SIZE-1, "%s", (*begin).c_str());
		acSubKeyName[CHAR_MAX_SIZE-1] = 0;
		
		if(RegDeleteKeyExt(hKey, acSubKeyName) == -1)
			return -2;
	}

	return 0;
}

INT32 CRegUtil::RegDeleteValueSubAll(HKEY hKey, LPCTSTR lpKey)
{
	CHAR acSubKeyName[CHAR_MAX_SIZE] = {0, };
    TListRegValueInfo   tRegValueInfoList;
	TListStr            tSubKeyNameList;
	
	if(hKey == NULL)
		return -1;
	
	GetEnumSubKeyName(hKey,lpKey, tSubKeyNameList, -1);
	TListStrItor begin, end;
	begin = tSubKeyNameList.begin();	end = tSubKeyNameList.end();
	for(begin; begin != end; begin++)
	{
		snprintf(acSubKeyName, CHAR_MAX_SIZE-1, "%s", (*begin).c_str());
		acSubKeyName[CHAR_MAX_SIZE-1] = 0;

		GetEnumSubValueName(hKey, acSubKeyName, tRegValueInfoList);
	}

    TListRegValueInfoItor begin_v, end_v;
    begin_v = tRegValueInfoList.begin();    end_v = tRegValueInfoList.end();
    for(begin_v; begin_v != end_v; begin_v++)
    {
        RegDeleteValueExt(hKey, (*begin_v).strKey.c_str(), (*begin_v).strValue.c_str());
    }

	return 0;
}

INT32 CRegUtil::GetEnumSubValueName(HKEY hKey, LPCTSTR lpKey, TListRegValueInfo& tRegValueInfoList, INT32 nLevel)
{
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };
	CHAR acValueName[MAX_PATH] = {0, };

	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_PATH);
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

    DWORD   dwValueLen = MAX_PATH, dwType = 0;
	HKEY	hOpenKey = NULL;
	INT32	nIndex = 0;

	if(RegOpenKeyEx(hKey, acFullKey, 0, KEY_ALL_ACCESS, &hOpenKey) == ERROR_SUCCESS)
	{
		while(RegEnumValue(hOpenKey, nIndex, acValueName, &dwValueLen, NULL, &dwType, NULL, NULL ) != ERROR_NO_MORE_ITEMS)
		{
            REG_VALUE_INFO reg_value_info;

            reg_value_info.strKey   = lpKey,
            reg_value_info.strValue = acValueName;

            tRegValueInfoList.push_back(reg_value_info);
            memset(acValueName, 0, MAX_PATH);
            dwValueLen = MAX_PATH;
			nIndex++;
		}
		RegCloseKey(hOpenKey);
	}
	return tRegValueInfoList.size();
}

INT32 CRegUtil::GetEnumSubKeyName(HKEY hKey, LPCTSTR lpKey, TListStr& tSubKeyNameList, INT32 nLevel)
{
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };
	CHAR acKeyName[CHAR_MAX_SIZE] = {0, };
	CHAR acSubKeyName[CHAR_MAX_SIZE] = {0, };
	HKEY hOpenKey = NULL;
	INT32 nResult = 0;
	INT32 nIndex = 0;

	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_PATH);
	
	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}

	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	if(RegOpenKey(hKey, acFullKey, &hOpenKey) == ERROR_SUCCESS)
	{
		while(RegEnumKey(hOpenKey, nIndex, acKeyName, CHAR_MAX_SIZE-1) != ERROR_NO_MORE_ITEMS)
		{
			if(lpKey != NULL && lpKey[0] != 0)
				snprintf(acSubKeyName, CHAR_MAX_SIZE-1, "%s/%s", lpKey, acKeyName);
			else
				snprintf(acSubKeyName, CHAR_MAX_SIZE-1, "/%s", acKeyName);
			if(nLevel)
				GetEnumSubKeyName(hKey, acSubKeyName, tSubKeyNameList, (nLevel - 1));
			else
				tSubKeyNameList.push_back(acSubKeyName);

			nIndex++;
			memset(acKeyName, 0, CHAR_MAX_SIZE);
		}
		if(lpKey != NULL && lpKey[0] != 0)
			tSubKeyNameList.push_back(lpKey);
		RegCloseKey(hOpenKey);
	}
	return tSubKeyNameList.size();
}

INT32 CRegUtil::RegIsExistsKey(HKEY hKey, LPCTSTR lpKey)
{
	HKEY hSubKey = NULL;
	CHAR acFullKey[CHAR_MAX_SIZE] = {0, };
	INT32 nResult = 0;
	if(hKey == NULL)
		return -1;

	if(m_lpRootPath[0] == 0)
		SetRootPath(STR_REG_DEFAULT_PATH);

	nResult = (INT32)strlen(m_lpRootPath);
	if(nResult < 1)
		return -11;
	if(nResult > 2 && m_lpRootPath[nResult-1] == '/')
	{
		m_lpRootPath[nResult-1] = 0;
	}
	
	if(lpKey)
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s/%s", m_lpRootPath, lpKey);
	else
		snprintf(acFullKey, CHAR_MAX_SIZE-1, "%s", m_lpRootPath);

	if(RegOpenKey(hKey, acFullKey, &hSubKey) != ERROR_SUCCESS)
		return -2;
	
	RegCloseKey(hSubKey);
	return 0;
}

INT32 CRegUtil::SetRootPath(LPCSTR lpRootPath)
{
	memset(m_lpRootPath, 0, CHAR_MAX_SIZE);
	strncpy(m_lpRootPath, (LPCTSTR)lpRootPath, CHAR_MAX_SIZE-1);
	m_lpRootPath[CHAR_MAX_SIZE-1] = 0;
	return 0;
}

INT32 CRegUtil::RegFullReadInt( HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT32& nValue )
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwSize = sizeof(INT32);	
	INT32 nResult = 0;

	if(hKey == NULL || lpKey == NULL || lpValue == NULL)
		return -1;
	
	do{
		if( RegCreateKeyEx( hKey, lpKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)&nValue, sizeof(INT32), &dwSize ) != ERROR_SUCCESS )
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);

	RegCloseKey( hSubKey );
	
	return nResult;
}

INT32 CRegUtil::RegFullReadString( HKEY hKey, LPCSTR lpKey, LPCSTR lpValue, LPSTR lpData, DWORD dwDataLen)
{
	HKEY hSubKey = NULL;
	DWORD dwDisp = 0;
	DWORD dwSize = 0;	 
	INT32 nResult = 0;	

	if(	hKey == NULL || lpKey == NULL || lpData == NULL || dwDataLen < 1)
	{
		return -1;
	}
	dwSize = dwDataLen;
	
	memset(lpData, 0, dwSize);
	do{
		if( RegCreateKeyEx( hKey, lpKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hSubKey, &dwDisp ) != ERROR_SUCCESS )
		{
			nResult = -2;
			break;
		}
		if( RegQueryValueEx( hSubKey, lpValue, 0, NULL, (LPBYTE)lpData, dwSize-1, &dwSize ) != ERROR_SUCCESS ) 
		{
			nResult = -3;
			break;
		}
		nResult = 0;
	}while(FALSE);
	
	RegCloseKey( hSubKey );
	
	return nResult;
}
