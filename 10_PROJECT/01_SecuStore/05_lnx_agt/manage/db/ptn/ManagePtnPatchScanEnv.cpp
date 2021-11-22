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
#include "ManagePtnPatchScanEnv.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatchScanEnv::CManagePtnPatchScanEnv()
{
	m_tDBMgrPtnPatchScanEnv	= new CDBMgrPtnPatchScanEnv();
}
//---------------------------------------------------------------------------

CManagePtnPatchScanEnv::~CManagePtnPatchScanEnv()
{
	if(m_tDBMgrPtnPatchScanEnv)		{	delete m_tDBMgrPtnPatchScanEnv;		m_tDBMgrPtnPatchScanEnv = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnv::LoadDBMS()
{
	TListDBPtnPatchScanEnv tDBPtnPatchScanEnvList;
    TListDBPtnPatchScanEnvItor begin, end;
	if(SetER(m_tDBMgrPtnPatchScanEnv->LoadExecute(&tDBPtnPatchScanEnvList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchScanEnvList.begin();	end = tDBPtnPatchScanEnvList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnv::AddPtnPatchScanEnv(DB_PTN_PATCH_SCAN_ENV&	data)
{
	if(SetER(m_tDBMgrPtnPatchScanEnv->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnv::EditPtnPatchScanEnv(DB_PTN_PATCH_SCAN_ENV&	data)
{
    if(m_tDBMgrPtnPatchScanEnv->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScanEnv::DelPtnPatchScanEnv(UINT32 nID)
{
	PDB_PTN_PATCH_SCAN_ENV pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatchScanEnv->DeleteExecute(pdata->nID)))
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

INT32	CManagePtnPatchScanEnv::GetPkt(MemToken& RecvToken, DB_PTN_PATCH_SCAN_ENV& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;	

	if ( RecvToken.TokenDel_String(data.strName) < 0)			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnv::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchScanEnvItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH_SCAN_ENV pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScanEnv::SetPkt(PDB_PTN_PATCH_SCAN_ENV pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_32(pdata->nRegDate);

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

void 		CManagePtnPatchScanEnv::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch_scan_env] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





