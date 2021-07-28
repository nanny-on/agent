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
#include "ManagePtnProcFile.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnProcFile::CManagePtnProcFile()
{
	m_tDBMgrPtnProcFile	= new CDBMgrPtnProcFile();
}
//---------------------------------------------------------------------------

CManagePtnProcFile::~CManagePtnProcFile()
{
	if(m_tDBMgrPtnProcFile)		{	delete m_tDBMgrPtnProcFile;		m_tDBMgrPtnProcFile = NULL;	}
}
//---------------------------------------------------------------------------

void		CManagePtnProcFile::SetDBFQUtil(CASIDBFQDLLUtil*	tASIDBFQDLLUtil)
{
	m_tDBMgrPtnProcFile->SetDBFQUtil(tASIDBFQDLLUtil);
}

INT32		CManagePtnProcFile::LoadDBMS()
{
	TListDBPtnProcFile tDBPtnProcFileList;
    TListDBPtnProcFileItor begin, end;
	if(SetER(m_tDBMgrPtnProcFile->LoadExecute(&tDBPtnProcFileList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnProcFileList.begin();	end = tDBPtnProcFileList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddKeyList(begin->nID);
		AddSKeyID(begin->strFeKey, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnProcFile::AddPtnProcFile(DB_PTN_PROC_FILE&	data)
{
	if(SetER(m_tDBMgrPtnProcFile->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data); 
	AddKeyList(data.nID);
	AddSKeyID(data.strFeKey, data.nID);   
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnProcFile::EditPtnProcFile(DB_PTN_PROC_FILE&	data)
{
    if(m_tDBMgrPtnProcFile->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnProcFile::DelPtnProcFile(UINT32 nID)
{
	PDB_PTN_PROC_FILE pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnProcFile->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyList(pdata->nID);
	DelSKeyID(pdata->strFeKey);
    DeleteItem(nID);
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

INT32					CManagePtnProcFile::GetInitPktList(UINT64 nKey, TListDBPtnProcFile& tInitPktList, UINT32 nOnceNum)
{
	if(!IsInitGlobalKeyListIter(nKey))
	{
		InitGlobalKeyListIter(nKey);
	}

	UINT32 nFindLastID = FALSE;
	while(IsNextGlobalKeyListIter(nKey) && nOnceNum && !nFindLastID)
	{
		UINT32 nID = NextGlobalKeyListIter(nKey);		
		if(!nID)	
		{
			nFindLastID = TRUE;
			continue;
		}

		PDB_PTN_PROC_FILE pdata = FindItem(nID);
		if(pdata)	tInitPktList.push_back(*pdata);

		nOnceNum -= 1;
	}

	if(nFindLastID)
	{
		FreeGlobalKeyListIter(nKey);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnProcFile::SetPkt(MemToken& SendToken, UINT64 nKey, UINT32 nOnceNum)
{
	TListDBPtnProcFile tInitPktList;
	GetInitPktList(nKey, tInitPktList, nOnceNum);

	SendToken.TokenAdd_32(tInitPktList.size());
	TListDBPtnProcFileItor begin, end;
	begin = tInitPktList.begin();	end = tInitPktList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PROC_FILE pdata = (PDB_PTN_PROC_FILE)&(*begin);

		SetPkt(pdata, SendToken);
	}
	return tInitPktList.size();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnProcFile::GetPkt(MemToken& RecvToken, DB_PTN_PROC_FILE& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_CString(data.strFeKey) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strFileHash) < 0)		goto INVALID_PKT;

	if ( RecvToken.TokenDel_CString(data.strFilePath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strFileName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strFileDescr) < 0)		goto INVALID_PKT;

	if ( RecvToken.TokenDel_CString(data.strFileVersion) < 0)	goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nFileSize))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_CString(data.strPubName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strPubSN) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strCntName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strCntSN) < 0)			goto INVALID_PKT;

	if ( RecvToken.TokenDel_CString(data.strPublisherName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_CString(data.strPublisherName) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nOsID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nBuildSN))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPeType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPeBit))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPtnSrcType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnRisk))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnProcFile::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnProcFileItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PROC_FILE pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnProcFile::SetPkt(TListID& tIDList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PROC_FILE pdata = FindItem(*begin);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return tIDList.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnProcFile::SetPkt(PDB_PTN_PROC_FILE pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nUsedMode);

	SendToken.TokenAdd_CString(pdata->strFeKey);
	SendToken.TokenAdd_CString(pdata->strFileHash);

	SendToken.TokenAdd_CString(pdata->strFilePath);
	SendToken.TokenAdd_CString(pdata->strFileName);
	SendToken.TokenAdd_CString(pdata->strFileDescr);

	SendToken.TokenAdd_CString(pdata->strFileVersion);
	SendToken.TokenAdd_32(pdata->nFileSize);

	SendToken.TokenAdd_CString(pdata->strPubName);
	SendToken.TokenAdd_CString(pdata->strPubSN);
	SendToken.TokenAdd_CString(pdata->strCntName);
	SendToken.TokenAdd_CString(pdata->strCntSN);

	SendToken.TokenAdd_CString(pdata->strPublisherName);
	SendToken.TokenAdd_CString(pdata->strProductName);

	SendToken.TokenAdd_64(pdata->nOsID);
	SendToken.TokenAdd_32(pdata->nBuildSN);
	SendToken.TokenAdd_32(pdata->nPeType);
	SendToken.TokenAdd_32(pdata->nPeBit);

	SendToken.TokenAdd_32(pdata->nPtnSrcType);
	SendToken.TokenAdd_32(pdata->nPtnType);
	SendToken.TokenAdd_32(pdata->nPtnRisk);

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

void 		CManagePtnProcFile::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_proc_file] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





