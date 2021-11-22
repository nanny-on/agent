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
#include "ManageLogPatch.h"

//---------------------------------------------------------------------------

CManageLogPatch*	t_ManageLogPatch = NULL;

//---------------------------------------------------------------------------

CManageLogPatch::CManageLogPatch()
{
	t_DBMgrLogPatch	= new CDBMgrLogPatch();
}
//---------------------------------------------------------------------------

CManageLogPatch::~CManageLogPatch()
{
	if(t_DBMgrLogPatch)	{	delete t_DBMgrLogPatch;	t_DBMgrLogPatch = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLogPatch::LoadDBMS()
{
	UINT32 nLogMode = 0, nLogNum = 0;

	TListDBLogPatch tDBLogPatchList;
	TListDBLogPatchItor begin, end;
	if(SetER(t_DBMgrLogPatch->LoadDB(tDBLogPatchList)))
    {
    	return g_nErrRtn;
    }

	begin = tDBLogPatchList.begin();	end = tDBLogPatchList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(*begin);
		AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_SID_MAP, begin->nRegSvrID, begin->nID);
		AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, begin->nPatchID, begin->nID);
		t_ManageLinkEnv->AddKeyIDMap(G_TYPE_PTN_PATCH, begin->nPatchID);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::AddLogPatch(DB_LOG_PATCH&	data)
{
	if(SetER(t_DBMgrLogPatch->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data);
	AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_SID_MAP, data.nRegSvrID, data.nID);
	AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, data.nPatchID, data.nID);

	t_ManageLinkEnv->AddKeyIDMap(G_TYPE_PTN_PATCH, data.nPatchID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::EditLogPatch(DB_LOG_PATCH&	data)
{
	PDB_LOG_PATCH pdata = FindItem(data.nID);
	if(!pdata)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogPatch->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    *pdata = data;

	AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_SID_MAP, data.nRegSvrID, data.nID);
	AddKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, data.nPatchID, data.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::DelLogPatch(UINT32 nID)
{
	PDB_LOG_PATCH pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogPatch->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }


	DelKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_SID_MAP, pdata->nRegSvrID);
	DelKeyTypeID(POS_KEY_ID_POSITION_LOG_PATCH_PID_MAP, pdata->nPatchID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CManageLogPatch::IsExistLocalPatch(TListDBLogPatch* tPatchInfoList, PDB_LOG_PATCH pdata)
{
	TListDBLogPatchItor begin, end;
	begin = tPatchInfoList->begin();	end = tPatchInfoList->end();
	for(begin; begin != end; begin++)
	{
		if(begin->nPatchID == pdata->nPatchID)		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageLogPatch::IsRestartSystem()
{
	TMapDBLogPatchItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nPatchStatus == SS_LOG_PATCH_STATUS_TYPE_END_RESTART)
			return 1;
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManageLogPatch::GetApplyNum(UINT32 nID)
{
	PDB_LOG_PATCH pdata = FindItem(nID);
	if(!pdata)	return 0;
	return pdata->nApplyNum;
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

INT32					CManageLogPatch::SetPkt(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBLogPatchItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(	ISSYNCSTEP(begin->second.nSyncSvrStep) || 
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
			SetPkt((PDB_LOG_PATCH)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::SetPkt(TListDBLogPatch& tLogList, MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TListDBLogPatchItor begin, end;
		begin = tLogList.begin();	end = tLogList.end();
		for(begin; begin != end; begin++)
		{
			if(	ISSYNCSTEP(begin->nSyncSvrStep) || 
				(begin->nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))	continue;

			tSendList.push_back(&(*begin));
		}
		if(tSendList.empty())	return -1;
	}

	SendToken.TokenAdd_32(tSendList.size());
	{
		TListPVOIDItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt((PDB_LOG_PATCH)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::SetPkt(PDB_LOG_PATCH pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nEvtTime);
	SendToken.TokenAdd_32(pdata->nEvtErrCode);

	SendToken.TokenAdd_32(pdata->nNotifyType);
	SendToken.TokenAdd_32(pdata->nNotifyID);
	SendToken.TokenAdd_32(pdata->nHostID);

	SendToken.TokenAdd_32(pdata->nRegSvrID);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_32(pdata->nPatchID);
	SendToken.TokenAdd_32(pdata->nPatchStatus);
	SendToken.TokenAdd_32(pdata->nPatchType);

	SendToken.TokenAdd_32(pdata->nUserID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::GetPkt(MemToken& RecvToken, DB_LOG_PATCH& data)
{
	if (!RecvToken.TokenDel_32(data.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nNotifyID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))					goto 	INVALID_PKT;
	
	if (!RecvToken.TokenDel_32(data.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPatchID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPatchStatus))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPatchType))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nUserID))					goto	INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

INT32					CManageLogPatch::SetPkt_Link(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());
	{
		TMapDBLogPatchItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			SetPkt_Link(&(begin->second), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::SetPkt_Link(TListDBLogPatch& tLogList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tLogList.size());
	{
		TListDBLogPatchItor begin, end;
		begin = tLogList.begin();	end = tLogList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt_Link(&(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::SetPkt_Link(PDB_LOG_PATCH pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nEvtTime);
	SendToken.TokenAdd_32(pdata->nEvtErrCode);

	SendToken.TokenAdd_32(pdata->nNotifyType);
	SendToken.TokenAdd_32(pdata->nNotifyID);
	SendToken.TokenAdd_32(pdata->nHostID);
	
	SendToken.TokenAdd_32(pdata->nRegSvrID);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_32(pdata->nPatchID);
	SendToken.TokenAdd_32(pdata->nPatchStatus);
	SendToken.TokenAdd_32(pdata->nPatchType);
	
	SendToken.TokenSet_Block();

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogPatch::GetPkt_Link(MemToken& RecvToken, DB_LOG_PATCH& data)
{
	if (!RecvToken.TokenDel_32(data.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nNotifyID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPatchID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPatchStatus))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPatchType))				goto 	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




