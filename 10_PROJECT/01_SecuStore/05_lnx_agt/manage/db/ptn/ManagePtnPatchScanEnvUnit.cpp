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
#include "ManagePtnPatchScanEnvUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatchScanEnvUnit::CManagePtnPatchScanEnvUnit()
{
	m_tDBMgrPtnPatchScanEnvUnit	= new CDBMgrPtnPatchScanEnvUnit();
}
//---------------------------------------------------------------------------

CManagePtnPatchScanEnvUnit::~CManagePtnPatchScanEnvUnit()
{
	if(m_tDBMgrPtnPatchScanEnvUnit)		{	delete m_tDBMgrPtnPatchScanEnvUnit;		m_tDBMgrPtnPatchScanEnvUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnvUnit::LoadDBMS()
{
	TListDBPtnPatchScanEnvUnit tDBPtnPatchScanEnvUnitList;
    TListDBPtnPatchScanEnvUnitItor begin, end;
	if(SetER(m_tDBMgrPtnPatchScanEnvUnit->LoadExecute(&tDBPtnPatchScanEnvUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchScanEnvUnitList.begin();	end = tDBPtnPatchScanEnvUnitList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);		
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnvUnit::AddPtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT&	data)
{
	if(SetER(m_tDBMgrPtnPatchScanEnvUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnvUnit::EditPtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT&	data)
{
    if(m_tDBMgrPtnPatchScanEnvUnit->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnvUnit::DelPtnPatchScanEnvUnit(UINT32 nID)
{
	PDB_PTN_PATCH_SCAN_ENV_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatchScanEnvUnit->DeleteExecute(pdata->nID)))
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

INT32	CManagePtnPatchScanEnvUnit::GetPkt(MemToken& RecvToken, DB_PTN_PATCH_SCAN_ENV_UNIT& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;	

	if (!RecvToken.TokenDel_32(data.nPkgID))					goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nScanType))					goto INVALID_PKT;	

	if ( RecvToken.TokenDel_String(data.strTarPath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarVerBegin) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarVerEnd) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nTarCompType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnvUnit::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchScanEnvUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH_SCAN_ENV_UNIT pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnvUnit::SetPkt(PDB_PTN_PATCH_SCAN_ENV_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_32(pdata->nPkgID);
	SendToken.TokenAdd_32(pdata->nScanType);

	SendToken.TokenAdd_String(pdata->strTarPath);
	SendToken.TokenAdd_String(pdata->strTarName);
	SendToken.TokenAdd_String(pdata->strTarVerBegin);
	SendToken.TokenAdd_String(pdata->strTarVerEnd);
	SendToken.TokenAdd_32(pdata->nTarCompType);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

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

void 		CManagePtnPatchScanEnvUnit::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch_scan_env_unit] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





