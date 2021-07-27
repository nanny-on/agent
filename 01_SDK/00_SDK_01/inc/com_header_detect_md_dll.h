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


#ifndef COM_HEADER_DETECT_MD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_DETECT_MD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------
typedef struct _asidm_init
{
	_asidm_init()
	{
		nRemainLog = 1;
		ZeroMemory(szLogFilePath, CHAR_MAX_SIZE);
		ZeroMemory(szLogFileName, CHAR_MAX_SIZE);
	}
	UINT32	nRemainLog;
	CHAR	szLogFilePath[CHAR_MAX_SIZE];
	CHAR	szLogFileName[CHAR_MAX_SIZE];
}ASIDM_INIT, *PASIDM_INIT;

//------------------------------------------------------------------------------
typedef struct _asidm_local_drive_info
{
	_asidm_local_drive_info()
	{
		nDriveName = 0;
		nDetectType = 0;
		ZeroMemory(szPhysicalDiskName, MAX_PATH);
		ZeroMemory(szInterfaceType, MAX_PATH);
		ZeroMemory(szMediaType, MAX_PATH);
		ZeroMemory(szLocalSize, MAX_PATH);
		ZeroMemory(szSerialNumber, MAX_PATH);
		ZeroMemory(szInstanceID, MAX_PATH);
	}
	UINT32	nDriveName;
	UINT32	nDetectType;
	CHAR	szPhysicalDiskName[MAX_PATH];
	CHAR	szInterfaceType[MAX_PATH];
	CHAR	szMediaType[MAX_PATH];
	CHAR	szLocalSize[MAX_PATH];
	CHAR	szSerialNumber[MAX_PATH];
	CHAR	szInstanceID[MAX_PATH];
}ASIDM_LOCAL_DRIVE_INFO, *PASIDM_LOCAL_DRIVE_INFO;

typedef list<ASIDM_LOCAL_DRIVE_INFO>			TListMediaLocalDriveInfo;
typedef TListMediaLocalDriveInfo::iterator		TListMediaLocalDriveInfoItor;

typedef map<UINT32, ASIDM_LOCAL_DRIVE_INFO>		TMapMediaLocalDriveInfo;
typedef TMapMediaLocalDriveInfo::iterator		TMapMediaLocalDriveInfoItor;
//--------------------------------------------------------------------------

#define		ASIDM_INFO_FILE_PATH		"ASI\\MDINFO\\"
#define		ASIDM_INFO_FILE_NAME		"36D03F6C-5AF9-4874-9C32-B6BEF25C41A6"

typedef struct _asidm_user_info
{
	_asidm_user_info()
	{
		nVer		= 0;
		nInfoSize	= 0;
		nMdSize		= 0;

		ZeroMemory(szMdSerialNumber, MAX_PATH);
		ZeroMemory(szMdGuid, MAX_PATH);
		ZeroMemory(szUserID, MAX_PATH);
		ZeroMemory(szUserPW, MAX_PATH);
	}

	UINT32		nVer;
	UINT32		nInfoSize;
	UINT64		nMdSize;
	CHAR		szMdSerialNumber[MAX_PATH];
	CHAR		szMdGuid[MAX_PATH];
	CHAR		szUserID[MAX_PATH];
	CHAR		szUserPW[MAX_PATH];
}ASIDM_USER_INFO, *PASIDM_USER_INFO;
//--------------------------------------------------------------------------

typedef	void (*GetDriveListType)(PASIDM_LOCAL_DRIVE_INFO pamldi, INT32 nSize, PVOID lParam);
//--------------------------------------------------------------------------

typedef	INT32 (*ASIDM_InitType)(PVOID* lpObject, PASIDM_INIT pasidm_init, UINT32 nSize);
//--------------------------------------------------------------------------
typedef	INT32 (*ASIDM_ConnectSvrType)(PVOID lpObject);
typedef	INT32 (*ASIDM_DisconnectSvrType)(PVOID lpObject);
//--------------------------------------------------------------------------
typedef	INT32 (*ASIDM_GetDriveListType)(PVOID lpObject, PVOID lParam, GetDriveListType cbGetDriveList );
typedef	INT32 (*ASIDM_FreeType)(PVOID lpObject);
//--------------------------------------------------------------------------

typedef INT32 (*ASIDM_GetUserInfoType)(UINT32 nDriveName, PASIDM_USER_INFO psmui, UINT32 nSize);
typedef INT32 (*ASIDM_SetUserInfoType)(UINT32 nDriveName, PASIDM_USER_INFO psmui, UINT32 nSize);
//--------------------------------------------------------------------------

/*
typedef struct _media_info
{
	_media_info()
	{
		szDriveName		= 0;
		nDetectTime		= 0;
		nStatus			= 0;
		nSaveInfoRtn	= 0;
		nMdSize			= 0;
	}

	UINT32		nDetectTime;
	UINT32		nStatus;
	UINT32		nSaveInfoRtn;
	UINT64		nMdSize;	
	CHAR		szDriveName;		//C, D, E
	String		strDeviceName;		//\Device\HarddiskVolume2
	String		strMdGuid;
}MEDIA_INFO, *PMEDIA_INFO;

typedef map<String, MEDIA_INFO>	TMapMediaInfo;
typedef TMapMediaInfo::iterator		TMapMediaInfoItor;
//------------------------------------------------------------------------------

typedef struct _media_save_info
{
	_media_save_info()
	{
		ZeroMemory(szMdGuid, 60);
		ZeroMemory(szUserID, 64);
		ZeroMemory(szUserPw, 64);

		nMdSize = 0;
	}
	CHAR		szMdGuid[60];
	UINT64		nMdSize;
	CHAR		szUserID[64];
	CHAR		szUserPw[64];
}MEDIA_SAVE_INFO, *PMEDIA_SAVE_INFO;
//------------------------------------------------------------------------------
*/

#endif      //COM_HEADER_DETECT_MD_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
