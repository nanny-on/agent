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


#ifndef MEM_LINK_ENV_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_LINK_ENV_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_link_env
{
	_mem_link_env()
	{
		nID					= 0;		
	}

	UINT32			nID;	
}MEM_LINK_ENV, *PMEM_LINK_ENV;

typedef list<MEM_LINK_ENV>				TListMemLinkEnv;
typedef TListMemLinkEnv::iterator		TListMemLinkEnvItor;

typedef map<UINT32, MEM_LINK_ENV>		TMapMemLinkEnv;
typedef TMapMemLinkEnv::iterator		TMapMemLinkEnvItor;

#endif //MEM_LINK_ENV_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



