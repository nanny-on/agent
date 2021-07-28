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
#include "ManagePtnVuln.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnVuln::CManagePtnVuln()
{
	m_tDBMgrPtnVuln	= new CDBMgrPtnVuln();
}
//---------------------------------------------------------------------------

CManagePtnVuln::~CManagePtnVuln()
{
	if(m_tDBMgrPtnVuln)		{	delete m_tDBMgrPtnVuln;		m_tDBMgrPtnVuln = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnVuln::LoadDBMS()
{
	TListDBPtnVuln tDBPtnVulnList;
    TListDBPtnVulnItor begin, end;
	if(SetER(m_tDBMgrPtnVuln->LoadExecute(&tDBPtnVulnList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnVulnList.begin();	end = tDBPtnVulnList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVuln::AddPtnVuln(DB_PTN_VULN&	data)
{
	if(SetER(m_tDBMgrPtnVuln->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVuln::EditPtnVuln(DB_PTN_VULN&	data)
{
    if(m_tDBMgrPtnVuln->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVuln::DelPtnVuln(UINT32 nID)
{
	PDB_PTN_VULN pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnVuln->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnVuln::GetPkt(MemToken& RecvToken, DB_PTN_VULN& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nClass))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strName) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strDescr) < 0)			goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nSupportOption))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRiskLevel))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nDefScore))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnVuln::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnVulnItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VULN pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnVuln::SetPkt(PDB_PTN_VULN pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);
	SendToken.TokenAdd_32(pdata->nRegDate);

	SendToken.TokenAdd_32(pdata->nClass);

	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strDescr);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

	SendToken.TokenAdd_32(pdata->nSupportOption);
	SendToken.TokenAdd_32(pdata->nRiskLevel);
	SendToken.TokenAdd_32(pdata->nDefScore);

	SendToken.TokenSet_Block();
 	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------- 

void 		CManagePtnVuln::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_vuln] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





