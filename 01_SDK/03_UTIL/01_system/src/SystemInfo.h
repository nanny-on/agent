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

#ifndef CSYSTEMINFO_H_8FE901B4_CCFE_48d1_9E83_DB7466A7313B
#define CSYSTEMINFO_H_8FE901B4_CCFE_48d1_9E83_DB7466A7313B
//---------------------------------------------------------------------------

class CSystemInfo
{
private :
//  BOOL	GetCurrentServicePack(String strServicePack);
    BOOL	GetCurrentSystemInfo();
    String 	GetSystemConvertName();

private:
	UINT32	m_nOsMajorVer;
	UINT32	m_nOsMinorVer;
	UINT32	m_nOsBuildVer;

	char 	m_acOSName[MAX_QHBUFF];
	char 	m_acRelease[MAX_QHBUFF];
	char 	m_acSystem[MAX_QHBUFF];
	char 	m_acIpAddr[MAX_QHBUFF];
	char 	m_acHostName[MAX_QHBUFF];
	char 	m_acMachine[MAX_TYPE_LEN];

	UINT32  m_nSystemType;
    UINT64  m_nSystemID;
	UINT32  m_nSystemPdType;
	UINT32	m_nSystemPaType;
    UINT32  m_nSPID;    

public :
    String	GetSysName();
    String	GetCompName();
    String	GetIpAddress();	
	
	UINT32	GetSystemType()			{	return m_nSystemType;		};
	UINT32	GetSystemPdType()		{	return m_nSystemPdType;		};
	UINT32	GetSystemPaType()		{	return m_nSystemPaType;		};
	UINT64  GetSystemID();
    UINT32	GetSPID();
	UINT32	GetSysProcArchitecture();
	UINT64	GetASIProcArchitecture();
	UINT32	GetSysProcessNumber();
	INT32	IsExistLoginSession();
	INT32	GetLoginSessionIDList(TListID& tSIDList);

private:
	void	GetSystemProductName(String& strName);
	void	GetSystemProcArchName(String& strName);

public:
    CSystemInfo();
    ~CSystemInfo();
} ;

extern CSystemInfo*		t_SystemInfo;

#endif
