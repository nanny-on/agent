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
#include "ManagePtnPatchScan.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatchScan::CManagePtnPatchScan()
{
	m_tDBMgrPtnPatchScan	= new CDBMgrPtnPatchScan();
}
//---------------------------------------------------------------------------

CManagePtnPatchScan::~CManagePtnPatchScan()
{
	if(m_tDBMgrPtnPatchScan)		{	delete m_tDBMgrPtnPatchScan;		m_tDBMgrPtnPatchScan = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScan::LoadDBMS()
{
	TListDBPtnPatchScan tDBPtnPatchScanList;
    TListDBPtnPatchScanItor begin, end;
	if(SetER(m_tDBMgrPtnPatchScan->LoadExecute(&tDBPtnPatchScanList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchScanList.begin();	end = tDBPtnPatchScanList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddSKeyID(begin->strFileHash, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScan::AddPtnPatchScan(DB_PTN_PATCH_SCAN&	data)
{
	if(SetER(m_tDBMgrPtnPatchScan->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
	AddSKeyID(data.strFileHash, data.nID);	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScan::EditPtnPatchScan(DB_PTN_PATCH_SCAN&	data)
{
    if(m_tDBMgrPtnPatchScan->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchScan::DelPtnPatchScan(UINT32 nID)
{
	PDB_PTN_PATCH_SCAN pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatchScan->DeleteExecute(pdata->nID)))
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

INT32	CManagePtnPatchScan::GetPkt(MemToken& RecvToken, DB_PTN_PATCH_SCAN& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;	

	if (!RecvToken.TokenDel_32(data.nPatchID))					goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nScanEnvID))				goto INVALID_PKT;	
	if (!RecvToken.TokenDel_32(data.nScanType))					goto INVALID_PKT;	

	if ( RecvToken.TokenDel_String(data.strTarPath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarVerBegin) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarVerEnd) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nTarCompType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nLanID))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strFileName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileHash) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileSize))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileExeSession))			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileCmd) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatchScan::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchScanItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH_SCAN pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchScan::SetPkt(PDB_PTN_PATCH_SCAN pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_32(pdata->nPatchID);
	SendToken.TokenAdd_32(pdata->nScanEnvID);
	SendToken.TokenAdd_32(pdata->nScanType);

	SendToken.TokenAdd_String(pdata->strTarPath);
	SendToken.TokenAdd_String(pdata->strTarName);
	SendToken.TokenAdd_String(pdata->strTarVerBegin);
	SendToken.TokenAdd_String(pdata->strTarVerEnd);
	SendToken.TokenAdd_32(pdata->nTarCompType);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

	SendToken.TokenAdd_32(pdata->nLanID);

	SendToken.TokenAdd_String(pdata->strFileName);
	SendToken.TokenAdd_String(pdata->strFileHash);
	SendToken.TokenAdd_32(pdata->nFileSize);
	SendToken.TokenAdd_32(pdata->nFileExeSession);
	SendToken.TokenAdd_String(pdata->strFileCmd);

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

void 		CManagePtnPatchScan::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch_scan] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





