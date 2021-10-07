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

//---------------------------------------------------------------------------

#include "stdafx.h"
#include <sys/utsname.h>
#include "com_struct.h"
#include "SystemInfo.h"

//---------------------------------------------------------------------------

CSystemInfo*		t_SystemInfo = NULL;

//---------------------------------------------------------------------------

CSystemInfo::CSystemInfo()
{
	m_nSystemType	= ASI_SYSTEM_TYPE_WORKSTATION;
	m_nSystemPaType	= PROCESSOR_ARCHITECTURE_INTEL;
	m_nSystemID = 0;
	m_nSystemType = 0;
	m_nCpuCoreNum = 0;
	memset(m_acOSName, 0, MAX_QHBUFF);
	memset(m_acRelease, 0, MAX_QHBUFF);
	memset(m_acSystem, 0, MAX_QHBUFF);
	memset(m_acIpAddr, 0, MAX_QHBUFF);
	memset(m_acHostName, 0, MAX_QHBUFF);
	memset(m_acMachine, 0, MAX_TYPE_LEN);
	pthread_mutex_init(&m_mutex, NULL);
	GetCurrentSystemInfo();
}
//---------------------------------------------------------------------------

CSystemInfo::~CSystemInfo()
{
	pthread_mutex_destroy(&m_mutex);
}
//---------------------------------------------------------------------------

String CSystemInfo::GetSysName()
{
    String strResult = "";
    strResult = GetSystemConvertName();
    return strResult;
}
//---------------------------------------------------------------------------

String CSystemInfo::GetCompName()
{
    String strResult = "";
	pthread_mutex_lock (&m_mutex);
	if(m_acHostName[0] != 0)
	{
		strResult = m_acHostName;
	}
	else
	{
	    if(gethostname(m_acHostName, MAX_QHBUFF-1) == 0)
	    {
			strResult = m_acHostName;
	    }
	}
	pthread_mutex_unlock (&m_mutex);
    return strResult;
}
//---------------------------------------------------------------------------

String CSystemInfo::GetIpAddress()
{
    String strIpAddr;
	INT32 i = 0;
    struct hostent *pHostEnt = NULL;
	struct in_addr **ppAddrList = NULL;
	pthread_mutex_lock (&m_mutex);
	do{
		if(m_acIpAddr[0] != 0)
		{
			strIpAddr = m_acIpAddr;
			break;
		}
		if(m_acHostName[0] == 0)
		{
			GetCompName();
		}
		pHostEnt = gethostbyname(m_acHostName);
		if(pHostEnt == NULL || pHostEnt->h_addr_list[0] == NULL)
		{
			strIpAddr.empty();
			break;
		}
		ppAddrList = (struct in_addr **)pHostEnt->h_addr_list;
		for(i = 0; ppAddrList[i] != NULL; i++)
		{
			strncpy(m_acIpAddr, (char *)inet_ntoa(*ppAddrList[i]), MAX_QHBUFF-1);
			break;
		}
		strIpAddr = m_acIpAddr;
	}while(FALSE);
	pthread_mutex_unlock (&m_mutex);
    return strIpAddr;

}
//---------------------------------------------------------------------------

BOOL CSystemInfo::InitHostInfo()
{
	struct utsname uname_data;
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_mutex);

	do{
		if(m_acMachine[0] != 0 && m_acHostName[0] != 0 && m_acRelease[0] != 0 && m_nSystemPaType != 0)
		{
			bRetVal = TRUE;
			break;
		}
		memset(&uname_data, 0, sizeof(struct utsname));
		if(uname(&uname_data) == -1)
		{
			bRetVal = FALSE;
			break;
		}
		strncpy(m_acMachine, uname_data.machine, MAX_TYPE_LEN-1);
		if(!_stricmp(m_acMachine, "x86_64"))
		{
			m_nSystemPaType = PROCESSOR_ARCHITECTURE_AMD64;
		}
		else if(!_stricmp(m_acMachine, "i386"))
		{
			m_nSystemPaType = PROCESSOR_ARCHITECTURE_INTEL;
		}
		else
		{
			m_nSystemPaType = PROCESSOR_ARCHITECTURE_ARM;
		}
		strncpy(m_acHostName, uname_data.nodename, MAX_TYPE_LEN-1);
		strncpy(m_acRelease, uname_data.release, MAX_QHBUFF-1);
		bRetVal = TRUE;
	}while(FALSE);
	pthread_mutex_unlock (&m_mutex);
	return bRetVal;
}

BOOL CSystemInfo::InitSystemInfo()
{
	struct utsname uname_data;
	BOOL bRetVal = TRUE;
	pthread_mutex_lock (&m_mutex);

	do{
		if(m_acSystem[0] != 0)
		{
			bRetVal = TRUE;
			break;
		}
		if(get_system_name(m_acSystem, MAX_QHBUFF) != 0)
		{
			bRetVal = FALSE;
			break;
		}
		bRetVal = TRUE;
	}while(FALSE);
	pthread_mutex_unlock (&m_mutex);
	return bRetVal;
}

VOID CSystemInfo::InitOSInfo()
{
	pthread_mutex_lock (&m_mutex);
	do{
		if(m_acOSName[0] != 0 && m_nSystemID != 0 && m_nSystemType != 0)
		{
			break;
		}
		get_os_info(m_acOSName, MAX_QHBUFF-1, &m_nSystemID, &m_nSystemType);
	}while(FALSE);
	pthread_mutex_unlock (&m_mutex);
}

BOOL CSystemInfo::GetCurrentSystemInfo()
{
	if(InitHostInfo() == FALSE)
		return FALSE;

	InitOSInfo();

	GetCompName();
	GetIpAddress();

	if(InitSystemInfo() == FALSE)
	{
		return FALSE;
	}
    return TRUE;
}
//---------------------------------------------------------------------------


String CSystemInfo::GetSystemConvertName()
{
	String strResult;
	pthread_mutex_lock (&m_mutex);
	strResult = m_acOSName;
	pthread_mutex_unlock (&m_mutex);
	GetSystemProcArchName(strResult);
	return strResult;
}

//---------------------------------------------------------------------------

UINT64    CSystemInfo::GetSystemID()
{
	UINT64 nSystemID = 0;
	pthread_mutex_lock (&m_mutex);
	nSystemID = m_nSystemID;
	pthread_mutex_unlock (&m_mutex);
    return nSystemID;
}
//---------------------------------------------------------------------------

UINT32    CSystemInfo::GetSPID()
{
	UINT64 nSPID = 0;
	pthread_mutex_lock (&m_mutex);
	nSPID = m_nSPID;
	pthread_mutex_unlock (&m_mutex);
	return nSPID;
}
//---------------------------------------------------------------------------

UINT32	CSystemInfo::GetSysProcArchitecture()
{
	UINT64 nSystemPaType = 0;
	pthread_mutex_lock (&m_mutex);
	nSystemPaType = m_nSystemPaType;
	pthread_mutex_unlock (&m_mutex);

	return nSystemPaType;
}
//---------------------------------------------------------------------------

UINT64	CSystemInfo::GetASIProcArchitecture()
{
	UINT64 nProcArch = 0;
	pthread_mutex_lock (&m_mutex);
	if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_INTEL)
	{
		nProcArch = ASI_SYSTEM_ARCH_X86;
	}
	else if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_AMD64)
	{
		nProcArch = ASI_SYSTEM_ARCH_X64;
	}
	else if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_ARM)
	{
		nProcArch = ASI_SYSTEM_ARCH_ARM;
	}
	pthread_mutex_unlock (&m_mutex);
	return nProcArch;
}
//---------------------------------------------------------------------------

UINT32	CSystemInfo::GetSysProcessNumber()
{
	UINT32 dwCount = 0;
	pthread_mutex_lock (&m_mutex);
	if(m_nCpuCoreNum == 0)
		m_nCpuCoreNum = (UINT32)get_cpu_core_num();
	dwCount = m_nCpuCoreNum;
	pthread_mutex_unlock (&m_mutex);
	return dwCount;
}
//---------------------------------------------------------------------------

void	CSystemInfo::GetSystemProcArchName(String& strName)
{
	UINT64 nSystemPaType = 0;
	pthread_mutex_lock (&m_mutex);
	nSystemPaType = m_nSystemPaType;
	pthread_mutex_unlock (&m_mutex);

	switch(nSystemPaType)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		strName += ", 64-bit";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		strName += ", 32-bit";
		break;
	}
}
//---------------------------------------------------------------------------

INT32	CSystemInfo::IsExistLoginSession()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32	CSystemInfo::GetLoginSessionIDList(TListID& tSIDList)
{
	return 0;
}
//---------------------------------------------------------------------------
