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
#include "com_struct.h"
#include "ManageProcessInfo.h"

//-------------------------------------------------------------------------------------
CManageProcessInfo*	t_ManageProcessInfo;
//-------------------------------------------------------------------------------------

CManageProcessInfo::CManageProcessInfo(void)
{	
	
}
//-------------------------------------------------------------------------------------

CManageProcessInfo::~CManageProcessInfo(void)
{
	
}
//-------------------------------------------------------------------------------------

void	CManageProcessInfo::AddProcessInfo(MEM_PROCESS_INFO& data)
{
	AddItem(data);
	return;
}
//-------------------------------------------------------------------------------------

void	CManageProcessInfo::DelProcessInfo(UINT32 nID)
{
	DeleteItem(nID);
	return;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

INT32	CManageProcessInfo::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapMemProcessInfoItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PMEM_PROCESS_INFO pdata = (PMEM_PROCESS_INFO)&(begin->second);
		SetPkt(pdata, SendToken);
	}
	return 0;
}
//-------------------------------------------------------------------------------------

INT32	CManageProcessInfo::SetPkt(PMEM_PROCESS_INFO pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nPID);
	SendToken.TokenAdd_32(pdata->nPPID);
	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strPath);
	SendToken.TokenAdd_32(pdata->nCpuValue);
	SendToken.TokenAdd_32(pdata->nMemValue);
	SendToken.TokenAdd_String(pdata->strProduct);
	SendToken.TokenAdd_String(pdata->strCompany);

	SendToken.TokenAdd_32(pdata->nReputation);
	SendToken.TokenAdd_32(pdata->nNumOfUser);

	SendToken.TokenAdd_64(pdata->nInPtnMatchType);
	SendToken.TokenAdd_64(pdata->nNcPtnMatchType);

	SendToken.TokenSet_Block();
	return 0;
}
//-------------------------------------------------------------------------------------

INT32		CManageProcessInfo::GetPkt(MemToken& RecvToken, MEM_PROCESS_INFO& data)
{
	if(!RecvToken.TokenDel_32(data.nPID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nPPID))					goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strName) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strPath) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nCpuValue))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nMemValue))				goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strProduct) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strCompany) < 0)		goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nReputation))			goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nNumOfUser))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_64(data.nInPtnMatchType))		goto INVALID_PKT;
	if(!RecvToken.TokenDel_64(data.nNcPtnMatchType))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------