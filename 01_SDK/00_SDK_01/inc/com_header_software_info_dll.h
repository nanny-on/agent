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

#ifndef     COM_HEADER_SOFTWARE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_SOFTWARE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
typedef struct _asisi_init
{
	_asisi_init()
	{
		nRemainLog = 1;
		ZeroMemory(szLogFilePath, CHAR_MAX_SIZE);
		ZeroMemory(szLogFileName, CHAR_MAX_SIZE);
	}
	UINT32	nRemainLog;
	CHAR	szLogFilePath[CHAR_MAX_SIZE];
	CHAR	szLogFileName[CHAR_MAX_SIZE];
}ASISI_INIT, *PASISI_INIT;
//------------------------------------------------------------------------------

typedef struct _asisi_sw_info
{
	_asisi_sw_info()
	{
		nDetectMode		= 0;
		nProcArchitect	= 0;
		ZeroMemory(szChkKey, MAX_PATH);
		ZeroMemory(szName, MAX_PATH);
		ZeroMemory(szPublisher, MAX_PATH);
		ZeroMemory(szInsDate, MAX_PATH);
		ZeroMemory(szVersion, MAX_PATH);
	}
	UINT32	nDetectMode;
	UINT32	nProcArchitect;

	CHAR	szChkKey[MAX_PATH];
	CHAR	szName[MAX_PATH];
	CHAR	szPublisher[MAX_PATH];
	CHAR	szInsDate[MAX_PATH];
	CHAR	szVersion[MAX_PATH];
}ASISI_SW_INFO, *PASISI_SW_INFO;

typedef list<ASISI_SW_INFO>						TListSwInfo;
typedef TListSwInfo::iterator					TListSwInfoItor;

typedef map<UINT32, ASISI_SW_INFO>				TMapSwInfo;
typedef TMapSwInfo::iterator					TMapSwInfoItor;
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define		ASISI_REQUEST_SW_INFO_APPWIZ			0x00000001
#define		ASISI_REQUEST_SW_INFO_NON_APPWIZ		0x00000002
#define		ASISI_REQUEST_SW_INFO_ALL				(ASISI_REQUEST_SW_INFO_APPWIZ | ASISI_REQUEST_SW_INFO_NON_APPWIZ)
//--------------------------------------------------------------------------

typedef	INT32 (*ASISI_InitType)(PVOID* lpObject, PASISI_INIT pasisi_init, UINT32 nSize);
typedef	INT32 (*ASISI_FreeType)(PVOID* lpObject);
//--------------------------------------------------------------------------

typedef	VOID (*GetSwInfoType)(PASISI_SW_INFO, INT32, PVOID);
typedef	INT32 (*ASISI_GetSwInfoType)(PVOID lpObject, UINT32 nReqMode, GetSwInfoType cbGetSwInfo, PVOID lParam);
//--------------------------------------------------------------------------

#endif      //COM_HEADER_SOFTWARE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
