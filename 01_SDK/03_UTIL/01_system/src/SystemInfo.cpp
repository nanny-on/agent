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
	memset(m_acOSName, 0, MAX_QHBUFF);
	memset(m_acRelease, 0, MAX_QHBUFF);
	memset(m_acSystem, 0, MAX_QHBUFF);
	memset(m_acIpAddr, 0, MAX_QHBUFF);
	memset(m_acHostName, 0, MAX_QHBUFF);
	memset(m_acMachine, 0, MAX_TYPE_LEN);
	GetCurrentSystemInfo();
}
//---------------------------------------------------------------------------

CSystemInfo::~CSystemInfo()
{
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
	if(m_acHostName[0] != 0)
	{
//		strResult = String(m_acHostName);
		strResult = m_acHostName;
	}
	else
	{
	    if(gethostname(m_acHostName, MAX_QHBUFF-1) == 0)
	    {
//	        strResult = String(m_acHostName);
			strResult = m_acHostName;
	    }
	}
    return strResult;
}
//---------------------------------------------------------------------------

String CSystemInfo::GetIpAddress()
{
    String strIpAddr;
    struct hostent *pHostEnt = NULL;
	if(m_acIpAddr[0] != 0)
	{
		strIpAddr = m_acIpAddr;
	    return strIpAddr;

	}
	if(m_acHostName[0] == 0)
	{
		GetCompName();
	}
	pHostEnt = gethostbyname(m_acHostName);
    if(pHostEnt == NULL)
    {
        return "" ;
    }
    strncpy(m_acIpAddr, (char *)inet_ntoa(*(struct in_addr *)*pHostEnt->h_addr_list), MAX_QHBUFF-1);
//	strIpAddr = String(m_acIpAddr);
	strIpAddr = m_acIpAddr;
    return strIpAddr;

}
//---------------------------------------------------------------------------

BOOL CSystemInfo::GetCurrentSystemInfo()
{
	struct utsname uname_data;

	if(m_acMachine[0] == 0 || m_acHostName[0] == 0 || m_acRelease[0] == 0)
	{
		memset(&uname_data, 0, sizeof(struct utsname));
		if(uname(&uname_data) == -1)
			return FALSE;

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
	}
	if(m_acOSName[0] == 0 || m_nSystemID == 0 || m_nSystemType == 0)
		get_os_info(m_acOSName, MAX_QHBUFF, &m_nSystemID, &m_nSystemType);
	GetCompName();
	GetIpAddress();
	if(m_acSystem[0] == 0)
	{
		if(get_system_name(m_acSystem, MAX_QHBUFF) != 0)
			return FALSE;
	}
    return TRUE;
}
//---------------------------------------------------------------------------


String CSystemInfo::GetSystemConvertName()
{
	String strResult = m_acOSName;
	GetSystemProcArchName(strResult);
	return strResult;
}

//---------------------------------------------------------------------------

UINT64    CSystemInfo::GetSystemID()
{
    return m_nSystemID;
}
//---------------------------------------------------------------------------

UINT32    CSystemInfo::GetSPID()
{
    return m_nSPID;
}
//---------------------------------------------------------------------------

UINT32	CSystemInfo::GetSysProcArchitecture()
{
	return m_nSystemPaType;
}
//---------------------------------------------------------------------------

UINT64	CSystemInfo::GetASIProcArchitecture()
{
	if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_INTEL)
	{
		return ASI_SYSTEM_ARCH_X86;
	}
	else if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_AMD64)
	{
		return ASI_SYSTEM_ARCH_X64;
	}
	else if(m_nSystemPaType == PROCESSOR_ARCHITECTURE_ARM)
	{
		return ASI_SYSTEM_ARCH_ARM;
	}
	
	return 0;
}
//---------------------------------------------------------------------------

UINT32	CSystemInfo::GetSysProcessNumber()
{
	UINT32 dwCount = 0;
	dwCount = (UINT32)get_cpu_core_num();
	return dwCount;
}
//---------------------------------------------------------------------------

void	CSystemInfo::GetSystemProcArchName(String& strName)
{
	switch(m_nSystemPaType)
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
	CProcUtil tProcUtil;
	TListID tPIDList;
	{
		tProcUtil.GetEnumProcessID("explorer.exe", tPIDList);
	}
	return tPIDList.size();
}
//---------------------------------------------------------------------------

INT32	CSystemInfo::GetLoginSessionIDList(TListID& tSIDList)
{
	CProcUtil tProcUtil;
	
	TMapID tSIDMap;
	TListID tPIDList;
	{
		tProcUtil.GetEnumProcessID("explorer.exe", tPIDList);
	}

	TListIDItor begin, end;
	begin = tPIDList.begin();	end = tPIDList.end();
	for(begin; begin != end; begin++)
	{
		UINT32 nPSID = tProcUtil.GetProcessSessionID(*begin);
		
		tSIDMap[nPSID] = 0;					
	}
	
	ConvertMapToList(tSIDMap, tSIDList);
	return 0;
}
//---------------------------------------------------------------------------

