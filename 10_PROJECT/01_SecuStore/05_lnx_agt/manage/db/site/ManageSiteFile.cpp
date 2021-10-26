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
#include "ManageSiteFile.h"

//---------------------------------------------------------------------------

CManageSiteFile*	t_ManageSiteFile = NULL;

//---------------------------------------------------------------------------

CManageSiteFile::CManageSiteFile()
{
	t_DBMgrSiteFile	= new CDBMgrSiteFile();
}
//---------------------------------------------------------------------------

CManageSiteFile::~CManageSiteFile()
{
	if(t_DBMgrSiteFile)		{	delete t_DBMgrSiteFile;		t_DBMgrSiteFile = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageSiteFile::LoadDBMS()
{
	TListDBSiteFile tDBSiteFileList;
    TListDBSiteFileItor begin, end;
	if(SetER(t_DBMgrSiteFile->LoadExecute(&tDBSiteFileList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBSiteFileList.begin();	end = tDBSiteFileList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		if(begin->strFeKey.empty() == FALSE)		AddPosSKeyID(begin->strFeKey, begin->nID, SS_SITE_FILE_KEY_TYPE_FE);
		if(begin->strFileHash.empty() == FALSE)	AddPosSKeyID(begin->strFileHash, begin->nID, SS_SITE_FILE_KEY_TYPE_HASH);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFile::AddSiteFile(DB_SITE_FILE&	dsf)
{
	if(SetER(t_DBMgrSiteFile->InsertExecute(&dsf)))
    {
    	return g_nErrRtn;
    }

	AddItem(dsf.nID, dsf);    

	if(dsf.strFeKey.empty() == FALSE)			AddPosSKeyID(dsf.strFeKey, dsf.nID, SS_SITE_FILE_KEY_TYPE_FE);
	if(dsf.strFileHash.empty() == FALSE)		AddPosSKeyID(dsf.strFileHash, dsf.nID, SS_SITE_FILE_KEY_TYPE_HASH);
	
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFile::EditSiteFile(DB_SITE_FILE&	dsf)
{
    if(t_DBMgrSiteFile->UpdateExecute(&dsf))		return ERR_DBMS_UPDATE_FAIL;

	if(dsf.strFeKey.empty() == FALSE)			AddPosSKeyID(dsf.strFeKey, dsf.nID, SS_SITE_FILE_KEY_TYPE_FE);
	if(dsf.strFileHash.empty() == FALSE)		AddPosSKeyID(dsf.strFileHash, dsf.nID, SS_SITE_FILE_KEY_TYPE_HASH);

	EditItem(dsf.nID, dsf);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageSiteFile::DelSiteFile(UINT32 nID)
{
	PDB_SITE_FILE pdsf = FindItem(nID);
	if(!pdsf)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(pdsf->strFeKey.empty() == FALSE)		DelPosSKeyID(pdsf->strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
	if(pdsf->strFileHash.empty() == FALSE)	DelPosSKeyID(pdsf->strFileHash, SS_SITE_FILE_KEY_TYPE_HASH);
	
	if(SetER(t_DBMgrSiteFile->DeleteExecute(pdsf->nID)))
    {
    	return g_nErrRtn;
    }

	DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_SITE_FILE			CManageSiteFile::FindSiteFileKey(String strName)
{
	return FindItem(FindSKeyID(strName));
}
//---------------------------------------------------------------------------

INT32					CManageSiteFile::ApplySiteFile(DB_SITE_FILE& dsf)
{
	if(dsf.nID != 0 && FindItem(dsf.nID))
	{
		return EditSiteFile(dsf);
	}
	return AddSiteFile(dsf);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

INT32	CManageSiteFile::GetPkt(MemToken& RecvToken, DB_SITE_FILE& dsf)
{
	if (!RecvToken.TokenDel_32(dsf.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nUsedFlag))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nRegDate))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nUsedMode))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dsf.nRegSvrID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nSyncSvrStep))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(dsf.strFeKey) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFileHash) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFilePath) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFileName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFileDescr) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFileVersion) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nFileSize))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(dsf.strPubName) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strPubSN) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strCntName) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strCntSN) < 0)			goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(dsf.strPublisherName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strProductName) < 0)		goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dsf.nPeType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nPeBit))						goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32	CManageSiteFile::GetPktSync(MemToken& RecvToken, DB_SITE_FILE& dsf)
{
	if (!RecvToken.TokenDel_32(dsf.nRegSvrID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dsf.nSyncSvrStep))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFeKey) < 0)			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(dsf.strFileHash) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

INT32					CManageSiteFile::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());
	{
		TMapDBSiteFileItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			SetPkt(&(begin->second), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageSiteFile::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBSiteFileItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(!(begin->second.nSyncSvrStep & SS_PO_FE_PTN_OP_NEW_FILE_SEND_TYPE_INFO))			continue;

			tIDList.push_back(&(begin->second));
		}

		if(tIDList.empty())	return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageSiteFile::SetPktSync(MemToken& SendToken)
{
	TListPVOID tSendList;
	{
		SetPktSync(tSendList);

		if(tSendList.empty())	return -1;
	}

	SendToken.TokenAdd_32(tSendList.size());
	{
		TListPVOIDItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt((PDB_SITE_FILE)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFile::SetPkt(TListID& tIDList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tIDList.size());
	{
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt(*begin, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFile::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_SITE_FILE pdsf = FindItem(nID);
	if(!pdsf)	return 0;

	SetPkt(pdsf, SendToken);
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageSiteFile::SetPkt(PDB_SITE_FILE pdsf, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdsf->nID);
	SendToken.TokenAdd_32(pdsf->nUsedFlag);
	SendToken.TokenAdd_32(pdsf->nRegDate);
	SendToken.TokenAdd_32(pdsf->nUsedMode);

	SendToken.TokenAdd_32(pdsf->nRegSvrID);
	SendToken.TokenAdd_32(pdsf->nSyncSvrStep);

	SendToken.TokenAdd_String(pdsf->strFeKey);
	SendToken.TokenAdd_String(pdsf->strFileHash);
	SendToken.TokenAdd_String(pdsf->strFilePath);
	SendToken.TokenAdd_String(pdsf->strFileName);
	SendToken.TokenAdd_String(pdsf->strFileDescr);
	SendToken.TokenAdd_String(pdsf->strFileVersion);
	SendToken.TokenAdd_32(pdsf->nFileSize);

	SendToken.TokenAdd_String(pdsf->strPubName);
	SendToken.TokenAdd_String(pdsf->strPubSN);
	SendToken.TokenAdd_String(pdsf->strCntName);
	SendToken.TokenAdd_String(pdsf->strCntSN);

	SendToken.TokenAdd_String(pdsf->strPublisherName);
	SendToken.TokenAdd_String(pdsf->strProductName);

	SendToken.TokenAdd_32(pdsf->nPeType);
	SendToken.TokenAdd_32(pdsf->nPeBit);

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

void 		CManageSiteFile::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][site_file] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





