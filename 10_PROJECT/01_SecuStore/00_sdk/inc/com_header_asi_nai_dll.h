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


#ifndef COM_HEADER_NAI_APP_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_NAI_APP_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#define		ASI_NAI_APP_VERSION						"20170213.01"
//------------------------------------------------------------------------

typedef struct _asi_nai_app_init
{
	_asi_nai_app_init()
	{
		ZeroMemory(pszAddr, 16);
		ZeroMemory(pszCltSockName, MAX_PATH);
		ZeroMemory(pszLogDirectory, MAX_PATH);
		ZeroMemory(pszLogFile, MAX_PATH);
		ZeroMemory(pszLogFile_Socket, MAX_PATH);
		ZeroMemory(pszLicenseKey, MAX_PATH);		

		nPort = 55001;
		nRemainLog = 1;
		hNotifyHandle = NULL;
		nNotifyID = 0;
		hOwner	= NULL;
	}

	CHAR	pszAddr[16];
	UINT16	nPort;
	CHAR	pszCltSockName[MAX_PATH];
	UINT8	nRemainLog;
	CHAR	pszLogDirectory[MAX_PATH];
	CHAR	pszLogFile[MAX_PATH];
	CHAR	pszLogFile_Socket[MAX_PATH];
	PVOID	hNotifyHandle;
	UINT32	nNotifyID;
	PVOID	hOwner;
	CHAR	pszLicenseKey[MAX_PATH];
	
}ASI_NAI_APP_INIT, *PASI_NAI_APP_INIT;

//------------------------------------------------------------------------

typedef struct _asi_nai_app_data
{
	_asi_nai_app_data()
	{
		_init();	
	}
	_asi_nai_app_data(UINT32 _nType, UINT32 _nCode)
	{
		_init();

		nType = _nType;
		nCode = _nCode;
	}

	void _init()
	{
		nType = 0;
		nCode = 0;
		nDataLen = 0;
		nSize = 0;
		lpData = NULL;	
	}

	UINT32	nType;
	UINT32	nCode;
	UINT32	nDataLen;
	UINT32 	nSize;
	PVOID	lpData;

}ASI_NAI_APP_DATA, *PASI_NAI_APP_DATA;

//------------------------------------------------------------------------

typedef struct _nai_notify_info
{
	_nai_notify_info()
	{
		_init();
	}
	_nai_notify_info(UINT32 _nType, UINT32 _nCode)
	{
		_init();

		nType = _nType;
		nCode = _nCode;
	}

	void _init()
	{
		nType = 0;
		nCode = 0;
		nID   = 0;
		nValue= 0;
		nSubValue = 0;
		lpData  = NULL;
	}

	UINT32	nType;
	UINT32	nCode;
	UINT32	nID;
	UINT32	nValue;
	UINT32  nSubValue;
	PVOID	lpData;
}NAI_NOTIFY_INFO, *PNAI_NOTIFY_INFO;
//-------------------------------------------------------------------------


typedef INT32	(*ASINAIAPP_InitType)(PVOID* lpObject, PASI_NAI_APP_INIT pAEAI, UINT32 nSize);
typedef INT32	(*ASINAIAPP_FreeType)(PVOID* lpObject);

typedef INT32	(*ASINAIAPP_GetPolicyType)(PVOID lpObject, PASI_NAI_APP_DATA pANAD);
typedef INT32	(*ASINAIAPP_SetPolicyType)(PVOID lpObject, PASI_NAI_APP_DATA pANAD);
typedef INT32	(*ASINAIAPP_FreePolicyType)(PVOID lpObject, PASI_NAI_APP_DATA pANAD);


typedef INT32	(*ASINAIAPP_SendPktType)(UINT16 nType, UINT16 nCode, PVOID lpData, UINT32 nDataLen);

typedef INT32	(*ASINAIAPP_IsInstalledType)(PVOID lpObject);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

#endif