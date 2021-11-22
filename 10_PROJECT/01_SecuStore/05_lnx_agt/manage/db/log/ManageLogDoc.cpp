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
#include "ManageLogDoc.h"

//---------------------------------------------------------------------------

CManageLogDoc*	t_ManageLogDoc = NULL;

//---------------------------------------------------------------------------

CManageLogDoc::CManageLogDoc()
{
	t_DBMgrLogDoc	= new CDBMgrLogDoc();
}
//---------------------------------------------------------------------------

CManageLogDoc::~CManageLogDoc()
{
	if(t_DBMgrLogDoc)	{	delete t_DBMgrLogDoc;	t_DBMgrLogDoc = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLogDoc::LoadDBMS()
{
	UINT32 nLogMode = 0, nLogNum = 0;
	{
		PDB_ENV_LOG pdel = t_ManageEnvLog->FindItem(DEFAULT_ID);
		if(pdel)
		{
			nLogMode = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadModeMap, SS_ENV_LOG_INDEX_DOC);
			nLogNum = t_ManageEnvLog->GetLogValue(pdel->tAgtLoadNumMap, SS_ENV_LOG_INDEX_DOC);
			if(nLogMode == SS_ENV_LOG_LOAD_MODE_TYPE_DAY && nLogNum)
			{
				nLogNum = ((GetCurrentDateTimeInt() / 86400) - nLogNum) * 86400;
			}
		}
	}

	TListDBLogDoc tDBLogDocList;
	TListDBLogDocItor begin, end;
	if(SetER(t_DBMgrLogDoc->LoadDB(nLogMode, nLogNum, tDBLogDocList)))
    {
    	return g_nErrRtn;
    }
    
	begin = tDBLogDocList.begin();	end = tDBLogDocList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(*begin);
		AddKeyID(begin->nRegSvrID, begin->nID);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::AddLogDoc(DB_LOG_DOC&	dld)
{
	if(SetER(t_DBMgrLogDoc->InsertExecute(&dld)))
    {
    	return g_nErrRtn;
    }

    AddItem(dld);
	AddKeyID(dld.nRegSvrID, dld.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::EditLogDoc(DB_LOG_DOC&	dld)
{
	PDB_LOG_DOC pdld = FindItem(dld.nID);
	if(!pdld)
		return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogDoc->UpdateExecute(&dld)))
    {
    	return g_nErrRtn;
    }

    *pdld = dld;
	AddKeyID(dld.nRegSvrID, dld.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::DelLogDoc(UINT32 nID)
{
	PDB_LOG_DOC pdld = FindItem(nID);
	if(!pdld)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrLogDoc->DeleteExecute(pdld->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdld->nRegSvrID);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::IsExistLogDoc(DB_LOG_DOC&	dld)
{
	TMapDBLogDocRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	for(begin; begin != end; begin++)
	{
		if(begin->second.nRemoveTime)	continue;
		if(begin->second.nBackupTime)	continue;

		//if(difftime(GetCurrentDateTimeInt(), begin->second.nRegDate) > (TIMER_INTERVAL_TIME_DAY * 30))	break;

//		if(begin->second.strObjectPath.CompareNoCase(dld.strObjectPath) == 0 && begin->second.strObjectName.CompareNoCase(dld.strObjectName) == 0)
//		if(!_stricmp(begin->second.strObjectPath.c_str(), dld.strObjectPath.c_str()) && !_stricmp(begin->second.strObjectName.c_str(), dld.strObjectName.c_str()))
		if(!strcmp(begin->second.strObjectPath.c_str(), dld.strObjectPath.c_str()) && !strcmp(begin->second.strObjectName.c_str(), dld.strObjectName.c_str()))
		{
			return begin->second.nID;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

PDB_LOG_DOC				CManageLogDoc::GetExistLogDoc(DB_LOG_DOC& dld)
{
	UINT32 nExistID = IsExistLogDoc(dld);
	if(!nExistID)	return NULL;

	return FindItem(nExistID);
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::IsExistLogDocByBkName(String strBkName, UINT32 nChkRmTime, UINT32 nChkBkTime)
{
	if(strBkName.empty())
		return 0;

	TMapDBLogDocRItor begin, end;
	begin = m_tMap.rbegin();	end = m_tMap.rend();
	for(begin; begin != end; begin++)
	{
		if(nChkRmTime && begin->second.nRemoveTime)		continue;
		if(nChkBkTime && !begin->second.nBackupTime)	continue;

//		if(strBkName.CompareNoCase(begin->second.strBkFileName) == 0)
//		if(!_stricmp(strBkName.c_str(), begin->second.strBkFileName.c_str()))
		if(!strcmp(strBkName.c_str(), begin->second.strBkFileName.c_str()))
		{
			return begin->second.nID;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageLogDoc::AddLogDocWithHost(DB_LOG_DOC&	dld)
{
	if(SetER(AddLogDoc(dld)))
	{
		return g_nErrRtn;
	}

	DB_LOG_DOC_HOST data;
	data.nEvtTime	= GetCurrentDateTimeInt();
	data.nLogID		= dld.nID;
	data.nUsedFlag	= STATUS_USED_MODE_ON;

	return t_ManageLogDocHost->AddLogDocHost(data);
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::EditLogDocWithHost(DB_LOG_DOC&	dld)
{
	if(SetER(EditLogDoc(dld)))
	{
		return g_nErrRtn;
	}

	{
		PDB_LOG_DOC_HOST pdata = t_ManageLogDocHost->FindKeyItem(dld.nID);
		if(!pdata)		return 0;

		pdata->nEvtTime = GetCurrentDateTimeInt();
		SetER(t_ManageLogDocHost->EditLogDocHost(*pdata));
	}
	return g_nErrRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CManageLogDoc::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBLogDocItor begin, end;
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

INT32			CManageLogDoc::ResetDocLog(BOOL bDelOnlyDB)
{
	if(SetER(t_DBMgrLogDoc->ResetTable()))
	{
		return g_nErrRtn;
	}

	if(!bDelOnlyDB)
	{
		ClearKeyID();
		ClearItem();
	}

	return 0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageLogDoc::SetPkt(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBLogDocItor begin, end;
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
			SetPkt((PDB_LOG_DOC)(*begin), SendToken);
		}
	}	
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManageLogDoc::SetPkt(PDB_LOG_DOC pdld, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdld->nID);
	SendToken.TokenAdd_32(pdld->nRegDate);
	SendToken.TokenAdd_32(pdld->nEvtTime);
	SendToken.TokenAdd_32(pdld->nEvtErrCode);

	SendToken.TokenAdd_32(pdld->nNotifyType);
	SendToken.TokenAdd_32(pdld->nNotifyID);

	SendToken.TokenAdd_32(pdld->nHostID);
	SendToken.TokenAdd_32(pdld->nPolicyType);
	SendToken.TokenAdd_32(pdld->nOpType);

	SendToken.TokenAdd_32(pdld->nRegSvrID);
	SendToken.TokenAdd_32(pdld->nSyncSvrStep);

	SendToken.TokenAdd_32(pdld->nRemoveTime);
	SendToken.TokenAdd_32(pdld->nBackupType);
	SendToken.TokenAdd_32(pdld->nBackupTime);

	SendToken.TokenAdd_String(pdld->strSubjectPath);
	SendToken.TokenAdd_String(pdld->strSubjectName);
	SendToken.TokenAdd_String(pdld->strObjectPath);
	SendToken.TokenAdd_String(pdld->strObjectName);
	SendToken.TokenAdd_String(pdld->strBkFileName);
	SendToken.TokenAdd_StringW(pdld->strObjectPathW);
	SendToken.TokenAdd_32(pdld->nUserID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::GetPkt(MemToken& RecvToken, DB_LOG_DOC& dld)
{
	if (!RecvToken.TokenDel_32(dld.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nNotifyID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nHostID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nPolicyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nOpType))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nRemoveTime))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nBackupType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nBackupTime))				goto 	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dld.strSubjectPath) < 0)		goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dld.strSubjectName) < 0)		goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dld.strObjectPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strObjectName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strBkFileName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_StringW(dld.strObjectPathW) < 0)	goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nUserID))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

INT32					CManageLogDoc::SetPkt_Link(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());
	{
		TMapDBLogDocItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			SetPkt_Link(&(begin->second), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::SetPkt_Link(PDB_LOG_DOC pdld, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdld->nID);
	SendToken.TokenAdd_32(pdld->nRegDate);
	SendToken.TokenAdd_32(pdld->nEvtTime);
	SendToken.TokenAdd_32(pdld->nEvtErrCode);

	SendToken.TokenAdd_32(pdld->nNotifyType);
	SendToken.TokenAdd_32(pdld->nNotifyID);

	SendToken.TokenAdd_32(pdld->nHostID);
	SendToken.TokenAdd_32(pdld->nPolicyType);
	SendToken.TokenAdd_32(pdld->nOpType);

	SendToken.TokenAdd_32(pdld->nRegSvrID);
	SendToken.TokenAdd_32(pdld->nSyncSvrStep);

	SendToken.TokenAdd_32(pdld->nRemoveTime);
	SendToken.TokenAdd_32(pdld->nBackupType);
	SendToken.TokenAdd_32(pdld->nBackupTime);

	SendToken.TokenAdd_String(pdld->strSubjectPath);
	SendToken.TokenAdd_String(pdld->strSubjectName);
	SendToken.TokenAdd_String(pdld->strObjectPath);
	SendToken.TokenAdd_String(pdld->strObjectName);
	SendToken.TokenAdd_String(pdld->strBkFileName);
	SendToken.TokenAdd_StringW(pdld->strObjectPathW);

	SendToken.TokenAdd_32(pdld->nFileCrTime);
	SendToken.TokenAdd_32(pdld->nFileMdTime);
	SendToken.TokenAdd_32(pdld->nFileAcTime);

	SendToken.TokenAdd_32(pdld->nUserID);

	SendToken.TokenSet_Block();

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLogDoc::GetPkt_Link(MemToken& RecvToken, DB_LOG_DOC& dld)
{
	if (!RecvToken.TokenDel_32(dld.nID))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nRegDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nEvtTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nEvtErrCode))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nNotifyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nNotifyID))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nHostID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nPolicyType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nOpType))					goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nRegSvrID))					goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nSyncSvrStep))				goto 	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nRemoveTime))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nBackupType))				goto 	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nBackupTime))				goto 	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dld.strSubjectPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strSubjectName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strObjectPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strObjectName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dld.strBkFileName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_StringW(dld.strObjectPathW) < 0)	goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nFileCrTime))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nFileMdTime))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dld.nFileAcTime))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dld.nUserID))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




