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
#include "ManagePtnVulnMP.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnVulnMP::CManagePtnVulnMP()
{
	m_tDBMgrPtnVulnMP	= new CDBMgrPtnVulnMP();
}
//---------------------------------------------------------------------------

CManagePtnVulnMP::~CManagePtnVulnMP()
{
	if(m_tDBMgrPtnVulnMP)		{	delete m_tDBMgrPtnVulnMP;		m_tDBMgrPtnVulnMP = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnVulnMP::LoadDBMS()
{
	TListDBPtnVulnMP tDBPtnVulnMPList;
    TListDBPtnVulnMPItor begin, end;
	if(SetER(m_tDBMgrPtnVulnMP->LoadExecute(&tDBPtnVulnMPList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnVulnMPList.begin();	end = tDBPtnVulnMPList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnMP::AddPtnVulnMP(DB_PTN_VULN_MP&	data)
{
	if(SetER(m_tDBMgrPtnVulnMP->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnMP::EditPtnVulnMP(DB_PTN_VULN_MP&	data)
{
    if(m_tDBMgrPtnVulnMP->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnMP::DelPtnVulnMP(UINT32 nID)
{
	PDB_PTN_VULN_MP pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnVulnMP->DeleteExecute(pdata->nID)))
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

INT32	CManagePtnVulnMP::GetPkt(MemToken& RecvToken, DB_PTN_VULN_MP& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;	

	if (!RecvToken.TokenDel_32(data.nType))						goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nCode))						goto INVALID_PKT;	

	if ( RecvToken.TokenDel_String(data.strCompanyName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFamilyName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strProductName) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nMatchPackage))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nMatchPolicy))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nMatchControl))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nChkType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nChkID))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnVulnMP::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnVulnMPItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VULN_MP pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnVulnMP::SetPkt(PDB_PTN_VULN_MP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_32(pdata->nType);
	SendToken.TokenAdd_32(pdata->nCode);

	SendToken.TokenAdd_String(pdata->strCompanyName);
	SendToken.TokenAdd_String(pdata->strFamilyName);
	SendToken.TokenAdd_String(pdata->strProductName);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

	SendToken.TokenAdd_64(pdata->nMatchPackage);
	SendToken.TokenAdd_64(pdata->nMatchPolicy);
	SendToken.TokenAdd_64(pdata->nMatchControl);

	SendToken.TokenAdd_32(pdata->nChkType);
	SendToken.TokenAdd_32(pdata->nChkID);

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

void 		CManagePtnVulnMP::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_vuln_mp] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





