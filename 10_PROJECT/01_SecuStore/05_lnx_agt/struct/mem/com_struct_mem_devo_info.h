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


#ifndef MEM_DEVO_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_DEVO_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_devo_info
{
	_mem_devo_info()
	{
		nID					= 0;
		nPolType			= 0;
		strProtectInfo		= "";
	}

	UINT32			nID;	
	UINT32			nPolType;
	String			strProtectInfo;
}MEM_DEVO_INFO, *PMEM_DEVO_INFO;

typedef list<MEM_DEVO_INFO>				TListMemDevOInfo;
typedef TListMemDevOInfo::iterator		TListMemDevOInfoItor;

typedef map<UINT32, MEM_DEVO_INFO>		TMapMemDevOInfo;
typedef TMapMemDevOInfo::iterator		TMapMemDevOInfoItor;
typedef TMapMemDevOInfo::reverse_iterator	TMapMemDevOInfoRItor;

#endif //MEM_DEVO_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



