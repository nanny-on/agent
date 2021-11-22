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

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "ManageEpsRegInfo.h"

//---------------------------------------------------------------------------

CManageEpsRegInfo*	t_ManageEpsRegInfo = NULL;

//---------------------------------------------------------------------------

CManageEpsRegInfo::CManageEpsRegInfo()
{

}
//---------------------------------------------------------------------------

CManageEpsRegInfo::~CManageEpsRegInfo()
{
	
}
//---------------------------------------------------------------------------
INT32		CManageEpsRegInfo::AddRegInfo(MEM_ASI_EPS_REG_INFO& data)
{
	if(data.nID == 0)	data.nID = GetMaxID();

	AddItem(data.nID, data);
	return 0;
}
//---------------------------------------------------------------------------

INT32				CManageEpsRegInfo::AddRegInfo(UINT32 nPoGID, String strRegInfo)
{
	MEM_ASI_EPS_REG_INFO data;
	data.nPolGID = nPoGID;
	data.strProtectInfo = strRegInfo;

	return AddRegInfo(data);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
UINT32		CManageEpsRegInfo::GetMaxID()
{
	TMapMemAsiEpsRegInfoRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	if(begin == end)	return 1;

	return (begin->first + 1);
}
//---------------------------------------------------------------------------

INT32		CManageEpsRegInfo::SetPkt(MemToken& SendToken)
{
	{
		SendToken.TokenAdd_32(m_tMap.size());

		TMapMemAsiEpsRegInfoItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			PMEM_ASI_EPS_REG_INFO pdata = &(begin->second);
			SetPkt(pdata, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageEpsRegInfo::SetPkt(PMEM_ASI_EPS_REG_INFO& pdata, MemToken& SendToken)
{

	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nPolGID);

	SendToken.TokenAdd_String(pdata->strProtectInfo);

	SendToken.TokenAdd_32(pdata->nAnyPerm);
	SendToken.TokenAdd_32(pdata->nLogAllow);
	SendToken.TokenAdd_32(pdata->nLogDeny);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageEpsRegInfo::GetPkt(MemToken& RecvToken, MEM_ASI_EPS_REG_INFO& data)
{
	if(!RecvToken.TokenDel_32(data.nID))						goto	INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nPolGID))					goto	INVALID_PKT;

	if( RecvToken.TokenDel_String(data.strProtectInfo) < 0)		goto	INVALID_PKT;	

	if(!RecvToken.TokenDel_32(data.nAnyPerm))					goto	INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nLogAllow))					goto	INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nLogDeny))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------