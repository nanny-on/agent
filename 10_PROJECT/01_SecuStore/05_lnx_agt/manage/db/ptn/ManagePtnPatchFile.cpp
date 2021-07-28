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
#include "ManagePtnPatchFile.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatchFile::CManagePtnPatchFile()
{
	m_tDBMgrPtnPatchFile	= new CDBMgrPtnPatchFile();
}
//---------------------------------------------------------------------------

CManagePtnPatchFile::~CManagePtnPatchFile()
{
	if(m_tDBMgrPtnPatchFile)		{	delete m_tDBMgrPtnPatchFile;		m_tDBMgrPtnPatchFile = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchFile::LoadDBMS()
{
	TListDBPtnPatchFile tDBPtnPatchFileList;
    TListDBPtnPatchFileItor begin, end;
	if(SetER(m_tDBMgrPtnPatchFile->LoadExecute(&tDBPtnPatchFileList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchFileList.begin();	end = tDBPtnPatchFileList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddSKeyID(begin->strFileHash, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchFile::AddPtnPatchFile(DB_PTN_PATCH_FILE&	data)
{
	if(SetER(m_tDBMgrPtnPatchFile->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    	
	AddSKeyID(data.strFileHash, data.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchFile::EditPtnPatchFile(DB_PTN_PATCH_FILE&	data)
{
    if(m_tDBMgrPtnPatchFile->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatchFile::DelPtnPatchFile(UINT32 nID)
{
	PDB_PTN_PATCH_FILE pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatchFile->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_PTN_PATCH_FILE			CManagePtnPatchFile::FindPtnPatchFileKey(String strKey)
{
	return FindItem(FindSKeyID(strKey));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnPatchFile::GetPkt(MemToken& RecvToken, DB_PTN_PATCH_FILE& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRevID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileRevID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nLanID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPatchType))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strFileName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strFileHash) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileSize))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strDownURL) < 0)		goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatchFile::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchFileItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH_FILE pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatchFile::SetPkt(PDB_PTN_PATCH_FILE pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);
	SendToken.TokenAdd_32(pdata->nRevID);
	SendToken.TokenAdd_32(pdata->nFileRevID);
	SendToken.TokenAdd_32(pdata->nLanID);
	SendToken.TokenAdd_32(pdata->nPatchType);

	SendToken.TokenAdd_String(pdata->strFileName);
	SendToken.TokenAdd_String(pdata->strFileHash);
	SendToken.TokenAdd_32(pdata->nFileSize);

	SendToken.TokenAdd_String(pdata->strDownURL);

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

void 		CManagePtnPatchFile::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch_dict] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





