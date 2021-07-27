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



#ifndef     COM_HEADER_RI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_RI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------

#define MEGABYTE_SIZE			1048576	

typedef struct _asires_info_process_basic_info
{
	INT32	nPID;
	INT32	nPPID;
	INT32	nBasePri;	
	CHAR	szProcessName[MAX_PROC_PATH];
	CHAR	szProcessFullName[MAX_PROC_PATH];
	_asires_info_process_basic_info()
	{
		nPID			= 0;
		nPPID			= 0;
		nBasePri		= 0;
		memset(szProcessName, 0, MAX_PROC_PATH);
		memset(szProcessFullName, 0, MAX_PROC_PATH);
	}
}ASIRES_INFO_PROCESS_BASIC_INFO, *PASIRES_INFO_PROCESS_BASIC_INFO;
//------------------------------------------------------------------------------------

typedef struct _asires_info_process_status
{
	INT32	nThreadCount;
	INT32	nHandleCount;
	INT32	nCpuValue;
	INT32	nMemValue;
	_asires_info_process_status()
	{
		nThreadCount		= 0;
		nHandleCount		= 0;
		nCpuValue			= 0;
		nMemValue			= 0;
	}
}ASIRES_INFO_PROCESS_STATUS_INFO, *PASIRES_INFO_PROCESS_STATUS_INFO;
//------------------------------------------------------------------------------------

typedef struct _asires_info_process_time_info
{
	UINT32	nStartTime;
	UINT32	nCPUTime;
	CHAR	szStartTime[MAX_TIME_STR];
	CHAR	szCPUTime[MAX_TIME_STR];
	_asires_info_process_time_info()
	{
		nStartTime = 0;
		nCPUTime = 0;
		memset(szStartTime, 0, MAX_TIME_STR);
		memset(szCPUTime, 0, MAX_TIME_STR);
	}
}ASIRES_INFO_PROCESS_TIME_INFO, *PASIRES_INFO_PROCESS_TIME_INFO;
//------------------------------------------------------------------------------------

typedef struct  _asires_info_process_add_info
{
	CHAR szUserName[MAX_QBUFF];
	CHAR szFileVersion[24];
	CHAR szProductName[256];
	CHAR szCompanyName[256];
	_asires_info_process_add_info()
	{
		memset(szUserName, 0, MAX_QBUFF);
		memset(szFileVersion, 0, 24);
		memset(szProductName, 0, 256);
		memset(szCompanyName, 0, 256);
	}
}ASIRES_INFO_PROCESS_ADD_INFO, *PASIRES_INFO_PROCESS_ADD_INFO;
//------------------------------------------------------------------------------------

typedef struct _asires_info_process_info
{
	ASIRES_INFO_PROCESS_BASIC_INFO			pbi;
	ASIRES_INFO_PROCESS_STATUS_INFO		psi;	
	ASIRES_INFO_PROCESS_TIME_INFO			pti;
	ASIRES_INFO_PROCESS_ADD_INFO			pai;

	CHAR szExeProcessName[MAX_PROC_PATH];
	INT32 nCount;
	_asires_info_process_info()
	{
		memset(&pbi, 0, sizeof(ASIRES_INFO_PROCESS_BASIC_INFO));
		memset(&psi, 0, sizeof(ASIRES_INFO_PROCESS_STATUS_INFO));
		memset(&pti, 0, sizeof(ASIRES_INFO_PROCESS_TIME_INFO));
		memset(&pai, 0, sizeof(ASIRES_INFO_PROCESS_ADD_INFO));

		memset(&szExeProcessName, 0, MAX_PROC_PATH);
		nCount = 0;
	}
}ASIRES_INFO_PROCESS_INFO, *PASIRES_INFO_PROCESS_INFO;
//------------------------------------------------------------------------------------
typedef list<String>		TListStr;
typedef TListStr::iterator	TListStrItor;

typedef list<ASIRES_INFO_PROCESS_INFO>			TListResInfoProcInfo;
typedef TListResInfoProcInfo::iterator		TListResInfoProcInfoItor;

typedef map<String, String>	TMapStr;
typedef TMapStr::iterator		TMapStrItor;

typedef map<String, UINT32>    		TMapStrID;
typedef TMapStrID::iterator     		TMapStrIDItor;
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


#define	RESDLL_TCP		0
#define RESDLL_UDP		1

typedef struct _asires_info_net_stat
{
	INT32 nProtocol;
	INT32 nLocalPort;
	INT32 nRemotePort;
	INT32 nStatus;			
	CHAR szLocalAddr[MAX_NETADDR_LEN];
	CHAR szRemoteAddr[MAX_NETADDR_LEN];

}ASIRES_INFO_NET_STAT, *PASIRES_INFO_NET_STAT;
//------------------------------------------------------------------------------------

typedef list<ASIRES_INFO_NET_STAT>		TListResInfoNetStat;
typedef TListResInfoNetStat::iterator	TListResInfoNetStatItor;
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


typedef INT32 (*CallBack_ProcessInfo)(PVOID lParam, ASIRES_INFO_PROCESS_INFO);
typedef INT32 (*CallBack_NetWorkInfo)(PVOID lParam, ASIRES_INFO_NET_STAT);

typedef	INT32 (*InitProcInfoType)(PVOID* lpObject);
typedef	INT32 (*FreeProcInfoType)(PVOID lpObject);
//--------------------------------------------------------------------------

typedef INT32 (*GetProcessResInfoType)(PVOID lpObject, void * lParam, void * fnCallBack);
typedef INT32 (*GetNetWorkResInfoType)(PVOID lpObject, void * lParam, void * fnCallBack);

//--------------------------------------------------------------------------

#endif      //COM_HEADER_RI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
