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

#ifndef MEM_PROCESS_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_PROCESS_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_process_info
{
	_mem_process_info()
	{
		nPID		= 0;
		nPPID		= 0;
		nCpuValue	= 0;
		nMemValue	= 0;
		nReputation	= 0;
		nNumOfUser	= 0;

		nInPtnMatchType	= 0;
		nNcPtnMatchType	= 0;
	}


	union
	{
		UINT32			nID;
		UINT32			nPID;
	};

	UINT32			nPPID;
	String			strName;
	String			strPath;

	INT32			nCpuValue;
	INT32			nMemValue;
	String			strUserName;
	String			strProduct;
	String			strCompany;

	UINT32			nReputation;
	UINT32			nNumOfUser;

	UINT64			nInPtnMatchType;
	UINT64			nNcPtnMatchType;

}MEM_PROCESS_INFO, *PMEM_PROCESS_INFO;

typedef list<MEM_PROCESS_INFO>				TListMemProcessInfo;
typedef TListMemProcessInfo::iterator		TListMemProcessInfoItor;

typedef map<UINT32, MEM_PROCESS_INFO>		TMapMemProcessInfo;
typedef TMapMemProcessInfo::iterator		TMapMemProcessInfoItor;

#endif //MEM_PROCESS_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



