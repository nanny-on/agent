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
#include "ManagePtnPatchWsus.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatchWsus::CManagePtnPatchWsus()
{
	m_tDBMgrPtnPatchWsus	= new CDBMgrPtnPatchWsus();
}
//---------------------------------------------------------------------------

CManagePtnPatchWsus::~CManagePtnPatchWsus()
{
	if(m_tDBMgrPtnPatchWsus)		{	delete m_tDBMgrPtnPatchWsus;		m_tDBMgrPtnPatchWsus = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchWsus::LoadDBMS()
{
	TListDBPtnPatchWsus tDBPtnPatchWsusList;
    TListDBPtnPatchWsusItor begin, end;
	if(SetER(m_tDBMgrPtnPatchWsus->LoadExecute(&tDBPtnPatchWsusList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchWsusList.begin();	end = tDBPtnPatchWsusList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchWsus::AddPtnPatchWsus(DB_PTN_PATCH_WSUS&	data)
{
	if(SetER(m_tDBMgrPtnPatchWsus->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchWsus::EditPtnPatchWsus(DB_PTN_PATCH_WSUS&	data)
{
    if(m_tDBMgrPtnPatchWsus->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchWsus::DelPtnPatchWsus(UINT32 nID)
{
	PDB_PTN_PATCH_WSUS pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatchWsus->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_PTN_PATCH_WSUS			CManagePtnPatchWsus::GetPtnPatchWsus(UINT64 nSysID, UINT32 nSpID)
{
	PDB_PTN_PATCH_WSUS pdata = NULL;
	
	TMapDBPtnPatchWsusItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nSysID == 0)	
		{
			pdata = &(begin->second);
			continue;
		}

		if(begin->second.nSysID != nSysID || begin->second.nSysSPID != nSpID)	continue;

		pdata = &(begin->second);
		break;
	}	
	return pdata;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchWsus::IsValidPtnFile(PDB_PTN_PATCH_WSUS pdata, UINT32 nDnType)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	switch(nDnType)
	{
		case SS_DN_FILE_TYPE_PPTN_WSUS:
		{
			strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_PATCH_FILE_PATCH, pdata->strName.c_str());
			break;
		}
		case SS_DN_FILE_TYPE_VPTN_WSUS:
		{
			strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_VULN_FILE_PATCH, pdata->strName.c_str());
			break;
		}
	}

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE);
		if(_stricmp(pdata->strHash.c_str(), szHash))
		{
			WriteLogE("po ptn patch wsus invalid hash : [%s][%s]", szHash, pdata->strHash.c_str());
			return 0;
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnPatchWsus::GetPkt(MemToken& RecvToken, DB_PTN_PATCH_WSUS& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))					goto INVALID_PKT;	
	if ( RecvToken.TokenDel_String(data.strName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strHash) < 0)		goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPID))				goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatchWsus::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchWsusItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH_WSUS pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchWsus::SetPkt(PDB_PTN_PATCH_WSUS pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strHash);

	SendToken.TokenAdd_64(pdata->nSysID);
	SendToken.TokenAdd_32(pdata->nSysSPID);

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

void 		CManagePtnPatchWsus::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch_wsus] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





