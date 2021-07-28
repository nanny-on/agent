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
#include "ManagePtnDeploy.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnDeploy::CManagePtnDeploy()
{
	m_tDBMgrPtnDeploy	= new CDBMgrPtnDeploy();
}
//---------------------------------------------------------------------------

CManagePtnDeploy::~CManagePtnDeploy()
{
	if(m_tDBMgrPtnDeploy)		{	delete m_tDBMgrPtnDeploy;		m_tDBMgrPtnDeploy = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnDeploy::LoadDBMS()
{
	TListDBPtnDeploy tDBPtnDeployList;
    TListDBPtnDeployItor begin, end;
	if(SetER(m_tDBMgrPtnDeploy->LoadExecute(&tDBPtnDeployList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnDeployList.begin();	end = tDBPtnDeployList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnDeploy::AddPtnDeploy(DB_PTN_DEPLOY&	data)
{
	if(SetER(m_tDBMgrPtnDeploy->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnDeploy::EditPtnDeploy(DB_PTN_DEPLOY&	data)
{
    if(m_tDBMgrPtnDeploy->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnDeploy::DelPtnDeploy(UINT32 nID)
{
	PDB_PTN_DEPLOY pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnDeploy->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_PTN_DEPLOY			CManagePtnDeploy::FindPtnDeployKey(String strKey)
{
	return FindItem(FindSKeyID(strKey));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnDeploy::GetPkt(MemToken& RecvToken, DB_PTN_DEPLOY& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nType))						goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nMatchPackage))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nMatchPolicy))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nMatchControl))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strSvFileName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSvFileHash) < 0)		goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nDnFileType))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strDnFilePath) < 0)		goto INVALID_PKT;	
	if ( RecvToken.TokenDel_String(data.strDnFileName) < 0)		goto INVALID_PKT;	
	if ( RecvToken.TokenDel_String(data.strDnFileHash) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnDeploy::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnDeployItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_DEPLOY pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnDeploy::SetPkt(PDB_PTN_DEPLOY pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_32(pdata->nType);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

	SendToken.TokenAdd_64(pdata->nMatchPackage);
	SendToken.TokenAdd_64(pdata->nMatchPolicy);
	SendToken.TokenAdd_64(pdata->nMatchControl);

	SendToken.TokenAdd_String(pdata->strSvFileName);
	SendToken.TokenAdd_String(pdata->strSvFileHash);

	SendToken.TokenAdd_32(pdata->nDnFileType);
	SendToken.TokenAdd_String(pdata->strDnFilePath);
	SendToken.TokenAdd_String(pdata->strDnFileName);
	SendToken.TokenAdd_String(pdata->strDnFileHash);

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

void 		CManagePtnDeploy::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_deploy] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





