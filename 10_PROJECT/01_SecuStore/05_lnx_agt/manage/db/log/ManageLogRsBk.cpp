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
#include "ManageLogRsBk.h"

//---------------------------------------------------------------------------

CManageLogRsBk*	t_ManageLogRsBk = NULL;

//---------------------------------------------------------------------------

CManageLogRsBk::CManageLogRsBk()
{
	t_DBMgrLogRsBk	= new CDBMgrLogRsBk();
	m_nTBkSize		= 0;
}
//---------------------------------------------------------------------------

CManageLogRsBk::~CManageLogRsBk()
{
	if(t_DBMgrLogRsBk)	{	delete t_DBMgrLogRsBk;	t_DBMgrLogRsBk = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLogRsBk::LoadDBMS()
{
	UINT32 nLogMode = 0, nLogNum = 0;
// 	{
// 		PDB_ENV_LOG pdel = t_ManageEnvLog->FindItem(DEFAULT_ID);
// 		if(pdel)
// 		{
// 			nLogMode = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadModeMap, SS_ENV_LOG_INDEX_DOC);
// 			nLogNum = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadNumMap, SS_ENV_LOG_INDEX_DOC);
// 			if(nLogMode == SS_ENV_LOG_LOAD_MODE_TYPE_DAY && nLogNum)
// 			{
// 				nLogNum = ((GetCurrentDateTimeInt() / 86400) - nLogNum) * 86400;
// 			}
// 		}
// 	}

	TListDBLogRsBk tDBLogRsBkList;
	TListDBLogRsBkItor begin, end;
	if(SetER(t_DBMgrLogRsBk->LoadDB(nLogMode, nLogNum, tDBLogRsBkList)))
    {
    	return g_nErrRtn;
    }
	
    
	begin = tDBLogRsBkList.begin();	end = tDBLogRsBkList.end();
	for(begin; begin != end; begin++)
	{
		m_nTBkSize += begin->nBackupSize;
		
		AddItem(*begin);
		AddKeyList(begin->nID);
		AddSKeyIDList(SPrintf("%s/%s", begin->strObjectPath.c_str(), begin->strObjectName.c_str()), begin->nID);
		{
			UINT32 nEvtDay = begin->nEvtTime / TIMER_INTERVAL_TIME_DAY;
			AddPosKeyIDMap(nEvtDay, begin->nID, POS_KEY_ID_POSITION_LOG_EVT_DAY);
		}
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::AddLogRsBk(DB_LOG_RS_BK&	data)
{
	if(SetER(t_DBMgrLogRsBk->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	m_nTBkSize += data.nBackupSize;

    AddItem(data);
	AddKeyList(data.nID); 

	AddSKeyIDList(SPrintf("%s/%s", data.strObjectPath.c_str(), data.strObjectName.c_str()), data.nID);

	{
		UINT32 nEvtDay = data.nEvtTime / TIMER_INTERVAL_TIME_DAY;
		AddPosKeyIDMap(nEvtDay, data.nID, POS_KEY_ID_POSITION_LOG_EVT_DAY);
	}

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::EditLogRsBk(DB_LOG_RS_BK&	data)
{
	PDB_LOG_RS_BK pdata = FindItem(data.nID);
	if(!pdata)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogRsBk->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

	m_nTBkSize -= pdata->nBackupSize;
	m_nTBkSize += data.nBackupSize;

    *pdata = data;

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::DelLogRsBk(UINT32 nID)
{
	PDB_LOG_RS_BK pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogRsBk->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	m_nTBkSize -= pdata->nBackupSize;

	{
		UINT32 nEvtDay = pdata->nEvtTime / TIMER_INTERVAL_TIME_DAY;
		DelPosKeyIDMap(nEvtDay, pdata->nID, POS_KEY_ID_POSITION_LOG_EVT_DAY);
	}


	DelSKeyIDList(SPrintf("%s/%s", pdata->strObjectPath.c_str(), pdata->strObjectName.c_str()), pdata->nID);	
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::ApplyLogRsBk(DB_LOG_RS_BK&	data)
{
	if(FindItem(data.nID))
		return EditLogRsBk(data);
	
	return AddLogRsBk(data);
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetItemIDListCnt(TListID& tIDList, UINT32 nCnt, UINT32 nDirection)
{
	if(nDirection)
	{
		TMapDBLogRsBkRItor begin, end;
		begin = m_tMap.rbegin(); end = m_tMap.rend();
		for(begin; begin != end && nCnt; begin++)
		{
			tIDList.push_back(begin->first);
			nCnt -= 1;
		}
	}
	else
	{
		TMapDBLogRsBkItor begin, end;
		begin = m_tMap.begin(); end = m_tMap.end();
		for(begin; begin != end && nCnt; begin++)
		{
			tIDList.push_back(begin->first);
			nCnt -= 1;
		}
	}
	return tIDList.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetKeyListDay(TListID& tIDList, UINT32 nDay, UINT32 nDirection)
{
	TMapID tKeyMap;		
	GetPosKeyIDMap_KeyMap(tKeyMap, POS_KEY_ID_POSITION_LOG_EVT_DAY);

	if(nDirection)
	{
		UINT32 nCurDayNum = GetCurrentDateTimeInt() / TIMER_INTERVAL_TIME_DAY;
		
		while(nDay--)
		{			
			TMapIDItor find = tKeyMap.find(nCurDayNum);
			if(find != tKeyMap.end())
			{
				tIDList.push_back(nCurDayNum);
			}
			nCurDayNum -= 1;			
		}
	}
	else
	{
		UINT32 nCurDayNum = GetCurrentDateTimeInt() / TIMER_INTERVAL_TIME_DAY;
		nCurDayNum -= (nDay);

		while(nDay--)
		{
			nCurDayNum += 1;

			TMapIDItor find = tKeyMap.find(nCurDayNum);
			if(find == tKeyMap.end())	continue;

			tIDList.push_back(nCurDayNum);
		}
	}
	return tIDList.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetItemIDMapDay(TMapID& tIDMap, UINT32 nDay, UINT32 nDirection)
{
	TListID tKeyList;
	GetKeyListDay(tKeyList, nDay, nDirection);

	TListIDItor begin, end;
	begin = tKeyList.begin();	end = tKeyList.end();
	for(begin; begin != end; begin++)
	{
		GetPosKeyIDMap(*begin, tIDMap, POS_KEY_ID_POSITION_LOG_EVT_DAY);
	}

	return tIDMap.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::CompareLogRsBkLast(DB_LOG_RS_BK& data)
{
	TListID tIDList;
	String strFullPath = SPrintf("%s/%s", data.strObjectPath.c_str(), data.strObjectName.c_str());
	GetSKeyIDList(strFullPath, tIDList);
	
	TListIDRItor begin, end;
	begin = tIDList.rbegin();	end = tIDList.rend();
	if(begin == end)	return -1;

	PDB_LOG_RS_BK pdata = FindItem(*begin);
	if(!pdata)		return -2;

	if(pdata->strOrFileHash != data.strOrFileHash)	return 1;
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetTodayLogRsBkID(DB_LOG_RS_BK&	data)
{
	UINT32 nCurDay = GetCurrentYearMonDay();

	TListID tIDList;
	String strFullPath = SPrintf("%s/%s", data.strObjectPath.c_str(), data.strObjectName.c_str());
	GetSKeyIDList(strFullPath, tIDList);

	{
		TListIDRItor begin, end;
		begin = tIDList.rbegin();	end = tIDList.rend();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_RS_BK pdata = FindItem(*begin);
			if(!pdata)		continue;

			UINT32 nChkDay = GetCurrentYearMonDay(pdata->nBackupTime, 0);
			if(nChkDay != nCurDay)			continue;

			return *begin;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

PDB_LOG_RS_BK				CManageLogRsBk::GetTodayLogRsBk(DB_LOG_RS_BK& data)
{
	UINT32 nExistID = GetTodayLogRsBkID(data);
	if(!nExistID)	return NULL;

	return FindItem(nExistID);
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetOldRsBk(PDB_LOG_RS_BK pdata, TListID& tIDList, UINT32 nRemainNum)
{
	return GetOldRsBk(SPrintf("%s/%s", pdata->strObjectPath.c_str(), pdata->strObjectName.c_str()), tIDList, nRemainNum);
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetOldRsBk(String strKey, TListID& tIDList, UINT32 nRemainNum)
{
	if(!nRemainNum)		return 0;

	TListID tCurIDList;
	GetSKeyIDList(strKey, tCurIDList);
	if(tCurIDList.size() <= nRemainNum)		return 0;
	
	{
		TListIDRItor begin, end;
		begin = tCurIDList.rbegin();	end = tCurIDList.rend();

		while(nRemainNum--)
		{
			begin++;
		}

		for(begin; begin != end; begin++)
		{
			tIDList.push_back(*begin);
		}	
	}

	return tIDList.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetFirstRsBk(String strKey, TListID& tIDList, UINT32 nRemainNum)
{
	TListID tCurIDList;
	GetSKeyIDList(strKey, tCurIDList);
	if(tCurIDList.size() <= nRemainNum)		return 0;

	if(!nRemainNum)
	{
		if(tCurIDList.size() == 1)		return 0;

		TListIDItor begin, end;
		begin = tCurIDList.begin();	end = tCurIDList.end();
		if(begin != end)
		{
			tIDList.push_back(*begin);
		}		
	}
	else
	{
		TListIDRItor begin, end;
		begin = tCurIDList.rbegin();	end = tCurIDList.rend();

		while(nRemainNum--)
		{
			begin++;
		}

		for(begin; begin != end; begin++)
		{
			tIDList.push_back(*begin);
		}	
	}

	return tIDList.size();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetInitPktList(UINT64 nKey, TListDBLogRsBk& tInitPktList, UINT32 nOnceNum)
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

		PDB_LOG_RS_BK pdata = FindItem(nID);
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

INT32					CManageLogRsBk::SetPkt(MemToken& SendToken, UINT64 nKey, UINT32 nOnceNum)
{
	TListDBLogRsBk tInitPktList;
	GetInitPktList(nKey, tInitPktList, nOnceNum);

	SendToken.TokenAdd_32(tInitPktList.size());
	TListDBLogRsBkItor begin, end;
	begin = tInitPktList.begin();	end = tInitPktList.end();
	for(begin; begin != end; begin++)
	{
		PDB_LOG_RS_BK pdata = (PDB_LOG_RS_BK)&(*begin);

		SetPkt(pdata, SendToken);
	}
	return tInitPktList.size();
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::SetPkt(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBLogRsBkItor begin, end;
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
			SetPkt((PDB_LOG_RS_BK)(*begin), SendToken);
		}
	}	
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManageLogRsBk::SetPkt(PDB_LOG_RS_BK pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nEvtTime);
	SendToken.TokenAdd_32(pdata->nEvtErrCode);

	SendToken.TokenAdd_32(pdata->nNotifyType);
	SendToken.TokenAdd_32(pdata->nNotifyID);

	SendToken.TokenAdd_32(pdata->nHostID);
	SendToken.TokenAdd_32(pdata->nPolicyType);
	SendToken.TokenAdd_32(pdata->nOpType);

	SendToken.TokenAdd_32(pdata->nRegSvrID);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_32(pdata->nBackupSize);
	SendToken.TokenAdd_32(pdata->nBackupType);
	SendToken.TokenAdd_32(pdata->nBackupTime);

	SendToken.TokenAdd_String(pdata->strSubjectPath);
	SendToken.TokenAdd_String(pdata->strSubjectName);
	SendToken.TokenAdd_String(pdata->strObjectPath);
	SendToken.TokenAdd_String(pdata->strObjectName);
	SendToken.TokenAdd_StringW(pdata->strObjectPathW);

	SendToken.TokenAdd_String(pdata->strBkFileName);
	SendToken.TokenAdd_String(pdata->strOrFileHash);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::SetPkt_Link(MemToken& SendToken, INT32 nMaxDay)
{
	TMapID tIDMap;
	GetItemIDMapDay(tIDMap, nMaxDay, 1);

	{
		SendToken.TokenAdd_32(tIDMap.size());

		TMapIDItor begin, end;
		begin = tIDMap.begin();	end = tIDMap.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_RS_BK pdata = FindItem(begin->first);
			if(!pdata)	continue;

			SetPkt(pdata, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogRsBk::GetPkt(MemToken& RecvToken, DB_LOG_RS_BK& data)
{
	if (!RecvToken.TokenDel_32(data.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nNotifyID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nHostID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPolicyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nOpType))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nBackupSize))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nBackupType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nBackupTime))				goto 	INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strSubjectPath) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSubjectName) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strObjectPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strObjectName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_StringW(data.strObjectPathW) < 0)	goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strBkFileName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strOrFileHash) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




