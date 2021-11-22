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
#include "ManageLogEvent.h"

//---------------------------------------------------------------------------

CManageLogEvent*	t_ManageLogEvent = NULL;

//---------------------------------------------------------------------------

CManageLogEvent::CManageLogEvent()
{
	t_DBMgrLogEvent	= new CDBMgrLogEvent();
}
//---------------------------------------------------------------------------

CManageLogEvent::~CManageLogEvent()
{
	if(t_DBMgrLogEvent)	{	delete t_DBMgrLogEvent;	t_DBMgrLogEvent = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLogEvent::LoadDBMS()
{
	UINT32 nLogMode = 0, nLogNum = 0;
	{
		PDB_ENV_LOG pdel = t_ManageEnvLog->FindItem(DEFAULT_ID);
		if(pdel)
		{
			nLogMode = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadModeMap, SS_ENV_LOG_INDEX_EVENT);
			nLogNum = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadNumMap, SS_ENV_LOG_INDEX_EVENT);
			if(nLogMode == SS_ENV_LOG_LOAD_MODE_TYPE_DAY && nLogNum)
			{
				nLogNum = ((GetCurrentDateTimeInt() / 86400) - nLogNum) * 86400;
			}
		}
	}

	TListDBLogEvent tDBLogEventList;
	TListDBLogEventItor begin, end;
	if(SetER(t_DBMgrLogEvent->LoadDB(nLogMode, nLogNum, tDBLogEventList)))
    {
    	return g_nErrRtn;
    }

	begin = tDBLogEventList.begin();	end = tDBLogEventList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(*begin);
	}
    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::AddLogEvent(DB_LOG_EVENT&	dle)
{
	if(SetER(t_DBMgrLogEvent->InsertExecute(&dle)))
    {
    	return g_nErrRtn;
    }

    AddItem(dle);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::EditLogEvent(DB_LOG_EVENT&	dle)
{
	PDB_LOG_EVENT pdle = FindLogEvent(dle.nID);
	if(!pdle)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogEvent->UpdateExecute(&dle)))
    {
    	return g_nErrRtn;
    }

    EditItem(dle);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::DelLogEvent(UINT32 nID)
{
	PDB_LOG_EVENT pdle = FindLogEvent(nID);
	if(!pdle)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogEvent->DeleteExecute(pdle->nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_LOG_EVENT	CManageLogEvent::FindLogEvent(UINT32 nID)
{
	return FindItem(nID);
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::GetInitPktList(UINT32 nAdminID, TListPVOID& tInitPktList)
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CManageLogEvent::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBLogEventItor begin, end;
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

INT32					CManageLogEvent::SetPktSyncMode(MemToken& SendToken, UINT32 nSyncMode)
{
	TListPVOID tSendList;

	{
		TMapDBLogEventItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if((begin->second.nSyncSvrMode && !(begin->second.nSyncSvrMode & nSyncMode)) ||
				(begin->second.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))	continue;

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
			SetPkt((PDB_LOG_EVENT)(*begin), SendToken);
		}
	}	
	return tSendList.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::SetPktSync(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBLogEventItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(	ISSYNCSTEP(begin->second.nSyncSvrStep) || 
				(begin->second.nSyncSvrMode & SS_LOG_EVENT_HOST_SYNC_MODE_SVR) == 0 ||
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
			SetPkt((PDB_LOG_EVENT)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::SetPkt(MemToken& SendToken, UINT32 nAdminID)
{
	TListPVOID tInitPktList;
	GetInitPktList(nAdminID, tInitPktList);

	SendToken.TokenAdd_32(tInitPktList.size());

	TListPVOIDItor begin, end;
	begin = tInitPktList.begin();	end = tInitPktList.end();
	for(begin; begin != end; begin++)
	{
		PDB_LOG_EVENT pdle = (PDB_LOG_EVENT)*begin;

		SetPkt(pdle, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBLogEventItor begin, end;	
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::SetPkt(PDB_LOG_EVENT pdle, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdle->nID);
	SendToken.TokenAdd_32(pdle->nRegDate);
	SendToken.TokenAdd_32(pdle->nEvtTime);
	SendToken.TokenAdd_32(pdle->nEvtErrCode);

	SendToken.TokenAdd_32(pdle->nNotifyType);
	SendToken.TokenAdd_32(pdle->nNotifyID);

	SendToken.TokenAdd_32(pdle->nHostID);
	SendToken.TokenAdd_32(pdle->nRegSvrID);
	SendToken.TokenAdd_32(pdle->nSyncSvrStep);

	SendToken.TokenAdd_32(pdle->nSubjectType);
	SendToken.TokenAdd_32(pdle->nSubjectID);

	SendToken.TokenAdd_String(pdle->strSubjectInfo);

	SendToken.TokenAdd_32(pdle->nTargetType);
	SendToken.TokenAdd_32(pdle->nTargetID);

	SendToken.TokenAdd_String(pdle->strTargetInfo);

	SendToken.TokenAdd_32(pdle->nObjectType);
	SendToken.TokenAdd_32(pdle->nObjectCode);
	SendToken.TokenAdd_32(pdle->nObjectID);
	SendToken.TokenAdd_String(pdle->strObjectInfo);

	SendToken.TokenAdd_32(pdle->nOperationType);

	SendToken.TokenAdd_String(pdle->strEventDescr);

	SendToken.TokenAdd_32(pdle->nUserID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogEvent::GetPkt(MemToken& RecvToken, DB_LOG_EVENT& dle)
{
	if (!RecvToken.TokenDel_32(dle.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dle.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nNotifyID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dle.nHostID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dle.nSubjectType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nSubjectID))					goto 	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dle.strSubjectInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nTargetType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nTargetID))					goto 	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dle.strTargetInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nObjectType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nObjectCode))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nObjectID))					goto 	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dle.strObjectInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nOperationType))				goto 	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dle.strEventDescr) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dle.nUserID))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



