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
#include "ManageLogDeployFile.h"

//---------------------------------------------------------------------------

CManageLogDeployFile*	t_ManageLogDeployFile = NULL;

//---------------------------------------------------------------------------

CManageLogDeployFile::CManageLogDeployFile()
{
	t_DBMgrLogDeployFile	= new CDBMgrLogDeployFile();
}
//---------------------------------------------------------------------------

CManageLogDeployFile::~CManageLogDeployFile()
{
	if(t_DBMgrLogDeployFile)	{	delete t_DBMgrLogDeployFile;	t_DBMgrLogDeployFile = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLogDeployFile::LoadDBMS()
{
	UINT32 nLogMode = 0, nLogNum = 0;
	{
		PDB_ENV_LOG pdel = t_ManageEnvLog->FindItem(DEFAULT_ID);
		if(pdel)
		{
			nLogMode = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadModeMap, SS_ENV_LOG_INDEX_DEPLOY_FILE);
			nLogNum = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadNumMap, SS_ENV_LOG_INDEX_DEPLOY_FILE);
			if(nLogMode == SS_ENV_LOG_LOAD_MODE_TYPE_DAY && nLogNum)
			{
				nLogNum = ((GetCurrentDateTimeInt() / 86400) - nLogNum) * 86400;
			}
		}
	}

	TListDBLogDeployFile tDBLogDeployFileList;
	TListDBLogDeployFileItor begin, end;
	if(SetER(t_DBMgrLogDeployFile->LoadDB(nLogMode, nLogNum, tDBLogDeployFileList)))
    {
    	return g_nErrRtn;
    }
    
	begin = tDBLogDeployFileList.begin();	end = tDBLogDeployFileList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(*begin);
		AddKeyID(begin->nRegSvrID, begin->nID);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::AddLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf)
{
	if(SetER(t_DBMgrLogDeployFile->InsertExecute(&dldf)))
    {
    	return g_nErrRtn;
    }

    AddItem(dldf);
	AddKeyID(dldf.nRegSvrID, dldf.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::EditLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf)
{
	PDB_LOG_DEPLOY_FILE pdldf = FindItem(dldf.nID);
	if(!pdldf)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogDeployFile->UpdateExecute(&dldf)))
    {
    	return g_nErrRtn;
    }

    *pdldf = dldf;
	AddKeyID(dldf.nRegSvrID, dldf.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::DelLogDeployFile(UINT32 nID)
{
	PDB_LOG_DEPLOY_FILE pdldf = FindItem(nID);
	if(!pdldf)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogDeployFile->DeleteExecute(pdldf->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdldf->nRegSvrID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::IsExistLogDeployFile(DB_LOG_DEPLOY_FILE&	dldf)
{
	TMapDBLogDeployFileRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nUnitID != dldf.nUnitID)		continue;
		if(begin->second.nPolicyID != dldf.nPolicyID)	continue;

		{
			return 1;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

PDB_LOG_DEPLOY_FILE		CManageLogDeployFile::FindItemByDFInfo(DB_LOG_DEPLOY_FILE&	dldf)
{
	TMapDBLogDeployFileRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nUnitID != dldf.nUnitID)		continue;
		if(begin->second.nPolicyID != dldf.nPolicyID)	continue;

		dldf = begin->second;
		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CManageLogDeployFile::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBLogDeployFileItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(ISSYNCSTEP(begin->second.nSyncSvrStep) || 
				(begin->second.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))	continue;

			tIDList.push_back(&(begin->second));
		}

		if(tIDList.empty())	return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::SetPkt(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBLogDeployFileItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(ISSYNCSTEP(begin->second.nSyncSvrStep) || 
				(begin->second.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))	continue;

			tSendList.push_back(&(begin->second));
		}

		if(tSendList.empty())	return -1;
	}

	SendToken.TokenAdd_32(tSendList.size());
	{
		TListPVOIDItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt((PDB_LOG_DEPLOY_FILE)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::SetPkt_Link(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	{
		TMapDBLogDeployFileItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			SetPkt(&(begin->second), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::SetPkt(PDB_LOG_DEPLOY_FILE pdldf, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdldf->nID);
	SendToken.TokenAdd_32(pdldf->nRegDate);
	SendToken.TokenAdd_32(pdldf->nEvtTime);
	SendToken.TokenAdd_32(pdldf->nEvtErrCode);

	SendToken.TokenAdd_32(pdldf->nNotifyType);
	SendToken.TokenAdd_32(pdldf->nNotifyID);

	SendToken.TokenAdd_32(pdldf->nHostID);
	SendToken.TokenAdd_32(pdldf->nPolicyID);
	SendToken.TokenAdd_32(pdldf->nUnitID);

	SendToken.TokenAdd_32(pdldf->nRegSvrID);
	SendToken.TokenAdd_32(pdldf->nSyncSvrStep);

	SendToken.TokenAdd_32(pdldf->nEndTime);
	SendToken.TokenAdd_32(pdldf->nEndReason);

	SendToken.TokenAdd_32(pdldf->nUserID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDeployFile::GetPkt(MemToken& RecvToken, DB_LOG_DEPLOY_FILE& dldf)
{
	if (!RecvToken.TokenDel_32(dldf.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dldf.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nNotifyID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dldf.nHostID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nPolicyID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nUnitID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dldf.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dldf.nEndTime))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dldf.nEndReason))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dldf.nUserID))					goto	INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



