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

#ifndef MEM_ASI_EPS_REG_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_ASI_EPS_REG_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_asi_eps_reg_info
{
	_mem_asi_eps_reg_info()
	{
		nID					= 0;
		nPolGID				= 0;
		strProtectInfo		= "";

		nAnyPerm			= 0;
		nLogAllow			= 0;
		nLogDeny			= 0;

	}

	UINT32			nID;
	UINT32			nPolGID;
	String			strProtectInfo;

	UINT32			nAnyPerm;
	UINT32			nLogAllow;
	UINT32			nLogDeny;

}MEM_ASI_EPS_REG_INFO, *PMEM_ASI_EPS_REG_INFO;

typedef list<MEM_ASI_EPS_REG_INFO>			TListMemAsiEpsRegInfo;
typedef TListMemAsiEpsRegInfo::iterator		TListMemAsiEpsRegInfoItor;

typedef map<UINT32, MEM_ASI_EPS_REG_INFO>		TMapMemAsiEpsRegInfo;
typedef TMapMemAsiEpsRegInfo::iterator			TMapMemAsiEpsRegInfoItor;
typedef TMapMemAsiEpsRegInfo::reverse_iterator	TMapMemAsiEpsRegInfoRItor;

#endif //MEM_ASI_EPS_REG_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



