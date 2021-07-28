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
#include "stdafx.h"
#include "com_struct.h"
#include "ComExternObject.h"


TMapMemDBMgrMember m_tMDMMMap;

MEM_DB_MGR_MEMBER&		GetMDMM()
{
	UINT32 nThreadID = (UINT32)gettid();
	TMapMemDBMgrMemberItor find = m_tMDMMMap.find(nThreadID);
	if(find == m_tMDMMMap.end())
	{
		MEM_DB_MGR_MEMBER tMDMM;
		m_tMDMMMap[nThreadID] = tMDMM;
		find = m_tMDMMMap.find(nThreadID);
	}
	return find->second;
}
//-------------------------------------------------------------------------

void				InitMDMM()
{
	MEM_DB_MGR_MEMBER& tMDMM = GetMDMM();
	tMDMM._init();
	return;
}