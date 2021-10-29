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
#include "LogicMgrLogDoc.h"

//---------------------------------------------------------------------------

CLogicMgrLogDoc*		t_LogicMgrLogDoc = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogDoc::CLogicMgrLogDoc()
{		
	t_ManageLogDoc	= new CManageLogDoc();
//	t_LogicLogDoc	= new CLogicLogDoc();

	t_ManageLogDocHost	= new CManageLogDocHost();

	t_ManageLogDoc->LoadDBMS();
	t_ManageLogDocHost->LoadDBMS();

	m_strLogicName		= "mgr log doc";

	m_nControlType		= SS_CONTROL_TYPE_LOG_DOC_ROLLBACK;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogDoc::~CLogicMgrLogDoc()
{
	SAFE_DELETE(t_ManageLogDocHost);

//	SAFE_DELETE(t_LogicLogDoc);
	SAFE_DELETE(t_ManageLogDoc);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_FIND:				nRtn = AnalyzePkt_FromMgr_Ext_Find();				break;
		case G_CODE_COMMON_SCAN:				nRtn = AnalyzePkt_FromMgr_Ext_Scan();				break;
		case G_CODE_COMMON_SYNC:				nRtn = AnalyzePkt_FromMgr_Ext_Sync();				break;
		case G_CODE_COMMON_ROLLBACK:			nRtn = AnalyzePkt_FromMgr_Ext_RollBack();			break;
		case G_CODE_LOG_DOC_ROLLBACK_EACH_HOST:	nRtn = AnalyzePkt_FromMgr_Ext_RollBack_Each_Host();	break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_LOG_DOC dld;
	if(t_ManageLogDoc->GetPkt_Link(RecvToken, dld))		return AZPKT_CB_RTN_PKT_INVALID;

	dld.nEvtTime	= t_ValidTimeUtil->GetValidTime();
	HISYNCSTEPUP(dld.nSyncSvrStep);

	if(t_ManageLogDoc->IsExistLogDoc(dld))
	{
		return AZPKT_CB_RTN_SUCCESS_END;
	}

	SetLogDoc(dld);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Del_Ext()
{
	DB_LOG_DOC dld;	

	if(t_ManageLogDoc->GetPkt_Link(RecvToken, dld))		return AZPKT_CB_RTN_PKT_INVALID;

	DeleteLogDoc(dld);

	return AZPKT_CB_RTN_SUCCESS_END;
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

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext_Find()
{
	DB_LOG_DOC dld;	

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(t_ManageLogDoc->GetPkt_Link(RecvToken, dld))		return AZPKT_CB_RTN_PKT_INVALID;

	dld.nEvtTime	= t_ValidTimeUtil->GetValidTime();

	{	
		t_LogicPoFaClear->SendPkt_Find(&dld);
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext_Scan()
{
	DB_LOG_DOC dld;	

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(t_ManageLogDoc->GetPkt_Link(RecvToken, dld))		return AZPKT_CB_RTN_PKT_INVALID;

	dld.nEvtTime	= t_ValidTimeUtil->GetValidTime();

	{	
		t_LogicLogDocDScan->SendPkt_Find(&dld);
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext_Sync()
{
	TListDBLogDoc tDBLogDocList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_LOG_DOC tDLD;
		if(t_ManageLogDoc->GetPkt(RecvToken, tDLD))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBLogDocList.push_back(tDLD);
	}

	{
		TListDBLogDocItor begin, end;
		begin = tDBLogDocList.begin();	end = tDBLogDocList.end();
		for(begin; begin != end; begin++)
		{
			t_LogicMgrLogDoc->SetLogDoc_Mgr(*begin);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext_RollBack()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_ROLLBACK;
	
	if(CheckPktResult(RecvToken))						return AZPKT_CB_RTN_RESULT_FAILED;
	if( RecvToken.TokenDel_IDList(m_tRecvIDList) < 0)	return AZPKT_CB_RTN_PKT_INVALID;

	{
		TListIDItor begin, end;
		begin = m_tRecvIDList.begin();	end = m_tRecvIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_DOC pdld = t_ManageLogDoc->FindKeyItem(*begin);
			if(!pdld)
			{
				SetDLEH_EC(ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND, 0);
				WriteLogE("[%s] not find host information for recovery [id:%d]", m_strLogicName.c_str(), *begin);
				continue;
			}

			if(pdld->nRemoveTime)			continue;
			if(pdld->nBackupTime == 0)		continue;

			{
				InitDLE_OBJ(pdld->nID, pdld->strObjectName, SS_LOG_EVENT_HOST_SYNC_MODE_ALL);
				t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
			}

			if(SetER(t_DocBackupUtil->RecoveryFile(pdld)))
			{
				WriteLogE("[%s] doc recovery operation fail : [er:%d][id:0]", m_strLogicName.c_str(), g_nErrRtn);
				return AZPKT_CB_RTN_SUCCESS_END;
			}

			HISYNCSTEPUP(pdld->nSyncSvrStep);
			SetLogDoc(*pdld);
		}
	}
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::AnalyzePkt_FromMgr_Ext_RollBack_Each_Host()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_ROLLBACK;

	UINT32 nStDate = 0, nEdDate = 0, nCnt = 0;

	if(CheckPktResult(RecvToken))						return AZPKT_CB_RTN_RESULT_FAILED;
	
	if(!RecvToken.TokenDel_32(nStDate))					return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(nEdDate))					return AZPKT_CB_RTN_PKT_INVALID;

	t_ManageLogDoc->GetItemIDList(m_tRecvIDList);

	{
		TListIDItor begin, end;
		begin = m_tRecvIDList.begin();	end = m_tRecvIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_DOC pdld = t_ManageLogDoc->FindItem(*begin);
			if(!pdld)
			{
				SetDLEH_EC(ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND, 0);
				WriteLogE("[%s] not find host information for delete [id:%d]", m_strLogicName.c_str(), *begin);
				continue;
			}

			if(pdld->nRemoveTime)			continue;
			if(pdld->nBackupTime == 0)		continue;
			if(nStDate != 0 && pdld->nBackupTime < nStDate)		continue;
			if(nEdDate != 0 && pdld->nBackupTime > nEdDate)		continue;

			if(SetER(t_DocBackupUtil->RecoveryFile(pdld)))
				WriteLogE("[%s] doc recovery operation fail : [er:%d][id:0]", m_strLogicName.c_str(), g_nErrRtn);

			HISYNCSTEPUP(pdld->nSyncSvrStep);
			SetLogDoc(*pdld);

			nCnt++;
		}

		WriteLogN("[%s] doc recovery each host operation success : [try_cnt:%u][suc_cnt:%u][st:%u][ed:%u]", m_strLogicName.c_str(), m_tRecvIDList.size(), nCnt, nStDate, nEdDate);

		{
			String strObjectInfo = "[doc recovery each host]";
			InitDLE_OBJ(0, strObjectInfo, SS_LOG_EVENT_HOST_SYNC_MODE_ALL);
			AppendDLEDesc(SS_LOG_EVENT_HOST_DESC_KEY_PERIOD_ST, nStDate);
			AppendDLEDesc(SS_LOG_EVENT_HOST_DESC_KEY_PERIOD_ED, nEdDate);
			AppendDLEDesc(SS_LOG_EVENT_HOST_DESC_KEY_RECOVERY_CNT, nCnt);
			t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
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

INT32		CLogicMgrLogDoc::DeleteLogDoc(DB_LOG_DOC& dld)
{	
	PDB_LOG_DOC pdld = NULL;
	UINT32 nPolUsedMode = STATUS_USED_MODE_ON;
	{
		pdld = t_ManageLogDoc->GetExistLogDoc(dld);
		if(pdld)
		{
			DB_LOG_DOC dld_temp = *pdld;
			dld_temp.nOpType = dld.nOpType;
			dld_temp.nPolicyType = dld.nPolicyType;

			dld = dld_temp;
		}
		else
		{
			dld.nEvtTime	= t_ValidTimeUtil->GetValidTime();
		}
	}

	if(t_LogicMgrPoFaClear != NULL)
	{
		if(t_LogicMgrPoFaClear->GetPoFaClearUserMode(nPolUsedMode) != 0)
			nPolUsedMode = STATUS_USED_MODE_ON;
	}

	UINT32 nDelCnt = 7, nLimitSize = 0, nLimitDelCnt = 0, nDelMethod = 0, nUsedMode = 1;
	if(dld.nOpType == SS_LOG_DOC_OP_TYPE_SCHEDULE || dld.nOpType == SS_LOG_DOC_OP_TYPE_SCHEDULE_DEMAND)
	{
		PDB_PO_FA_CLEAR_UNIT pdb_unit = t_ManagePoFaClearUnit->FindItem(dld.nPolicyType - ASI_EPS_APP_POLICY_GROUP_ID_FA_CLEAR);
		// 20200728 edit jhjung
		if(pdb_unit)
		{
			nDelCnt			= pdb_unit->nDelCount;
			nLimitSize		= pdb_unit->nLimitSize;
			nLimitDelCnt	= pdb_unit->nLimitCnt;
			nDelMethod		= pdb_unit->nDelMethod;
			if(nPolUsedMode == STATUS_USED_MODE_OFF || pdb_unit->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
				nUsedMode = STATUS_USED_MODE_OFF;
			else if(nPolUsedMode == STATUS_USED_MODE_WARN || pdb_unit->tDPH.nUsedMode == STATUS_USED_MODE_WARN)
				nUsedMode = STATUS_USED_MODE_WARN;
			else
				nUsedMode = STATUS_USED_MODE_ON;
		}
	}

	if(SetER(t_DocBackupUtil->BackupFile(&dld, nDelMethod, nDelCnt, nLimitSize, nLimitDelCnt, nUsedMode)))
	{
		WriteLogE("[%s] doc backup operation fail : [er:%d][id:0]", m_strLogicName.c_str(), g_nErrRtn);
		return -1;
	}

	HISYNCSTEPUP(dld.nSyncSvrStep);

	if(nUsedMode == STATUS_USED_MODE_WARN)
	{
		dld.nOpType = SS_LOG_DOC_OP_TYPE_WARNING;
	}

	

	HISYNCSTEPUP(dld.nSyncSvrStep);
	SetLogDoc(dld);
	return 0;
}
//---------------------------------------------------------------------------

void		CLogicMgrLogDoc::SetLogDoc(DB_LOG_DOC& dld)
{
	INT32 nRetVal = 0;
	if(!ISSYNCSTEP(dld.nSyncSvrStep)) 
	{
		if(dld.strObjectPath.find("%") == string::npos && dld.strObjectName.find("%") == string::npos &&
			dld.strSubjectPath.find("%") == string::npos && dld.strSubjectName.find("%") == string::npos)
		{

			WriteLogN("[%s] remain evt log to file : op[%d]:rm[%d]:bk[%d]:fi[%s/%s:%s/%s][%s]", 
				m_strLogicName.c_str(),
				dld.nOpType, dld.nRemoveTime, dld.nBackupTime,
				dld.strSubjectPath.c_str(), dld.strSubjectName.c_str(), 
				dld.strObjectPath.c_str(), dld.strObjectName.c_str(), dld.strBkFileName.c_str());

		}
	}

	{
		dld.nUserID = t_ManageHost->GetUserID(t_ManageHost->FirstID());
	}

	{
		PDB_ENV_LOG_UNIT pDELEU = t_ManageEnvLogUnit->FindRecordLogDocUnit(&dld);
		if(pDELEU && pDELEU->tDPH.nUsedMode == STATUS_USED_MODE_ON)
		{
			dld.nSkipTarget = pDELEU->tDPH.nExtOption;
		}
	}

	if(!(dld.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))
	{
		if(dld.nID != 0 && t_ManageLogDoc->FindItem(dld.nID))
		{
			t_ManageLogDoc->EditLogDoc(dld);
		}
		else
		{
			t_ManageLogDoc->AddLogDocWithHost(dld);
		}
	}
	
	{
		m_tMutex.Lock();
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageLogDoc->SetPkt(&dld, SendToken);
		if(!ISSYNCSTEP(dld.nSyncSvrStep) && !(dld.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))
		{
			nRetVal = SendData_Mgr(G_TYPE_LOG_DOC, G_CODE_COMMON_SYNC, SendToken);
		}
		SendToken.Clear();
		m_tMutex.UnLock();
/*		
		{
			SendToken.TokenAdd_32(1);
			t_ManageLogDoc->SetPkt_Link(&dld, SendToken);
			SendData_Link(G_TYPE_LOG_DOC, G_CODE_COMMON_SYNC, SendToken);
			SendToken.Clear();
		}
*/
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrLogDoc::SetLogDoc_Mgr(DB_LOG_DOC& dld)
{
	PDB_LOG_DOC pdld = t_ManageLogDoc->FindItem(dld.nID);
	if(pdld)
	{
		dld.nFileCrTime = pdld->nFileCrTime;
		dld.nFileMdTime = pdld->nFileMdTime;
		dld.nFileAcTime = pdld->nFileAcTime;
		dld.strObjectPath = pdld->strObjectPath;
		dld.strObjectName = pdld->strObjectName;
		dld.strBkFileName = pdld->strBkFileName;
	}

	SetLogDoc(dld);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::ChkBackupOp(UINT32 nDelMethod, UINT32 nDelCnt, UINT32 nLimitSize, UINT32 nLimitDelCnt, UINT32 nChkFDTType, UINT32 nDelAfterDay, UINT32 nUsedMode, UINT32& nContinue)
{
	CFileUtil tFileUtil;
	INT32 nDelDocCount = 0;
	INT32 nFileFormat = 0;
	INT32 i, nListCount = 0;
	DB_LOG_DOC stDbLogDoc;
	PNOTIFY_PATH pNotifyPath = NULL;
	PCHAR pFileName = NULL;

	nListCount = t_LogicMgrPoFaInotifyFile->GetInotifyPathCount();
	if(nListCount < 1 || t_ASIFFDLLUtil == NULL)
	{
		return 0;
	}

	pNotifyPath = (PNOTIFY_PATH)malloc(sizeof(NOTIFY_PATH )*nListCount);
	if(pNotifyPath == NULL)
	{
		return 0;
	}

	memset(pNotifyPath, 0, sizeof(NOTIFY_PATH )*nListCount);

	pFileName = (PCHAR)malloc(MAX_HBUFF);
	if(pFileName == NULL)
	{
		safe_free(pNotifyPath);
		return 0;
	}

	if(t_LogicMgrPoFaInotifyFile->GetAllInotifyPath(pNotifyPath, nListCount) != 0)
	{
		safe_free(pNotifyPath);
		safe_free(pFileName);
		return 0;
	}
	
	for(i=0; i<nListCount; i++)
	{
		memset(&stDbLogDoc, 0, sizeof(stDbLogDoc));
		memset(pFileName, 0, MAX_HBUFF);

		nFileFormat = 0;

		if(tFileUtil.FileExists(pNotifyPath[i].acNotifyPath) == FALSE)
		{
			continue;
		}

		if(t_ASIFFDLLUtil->ASIFF_IsDocFileFormat(pNotifyPath[i].acNotifyPath, &nFileFormat) != 0)
		{
			continue;
		}
		if(nFileFormat == 0)
		{
			continue;
		}


		if(tFileUtil.GetFileDateTime(pNotifyPath[i].acNotifyPath, &stDbLogDoc.nFileCrTime, &stDbLogDoc.nFileMdTime, &stDbLogDoc.nFileAcTime) != 0)
		{
			stDbLogDoc.nFileCrTime = stDbLogDoc.nFileMdTime = stDbLogDoc.nFileAcTime = 0;
		}

		if(get_basename(pNotifyPath[i].acNotifyPath, pFileName, MAX_HBUFF-1) == NULL)
		{
			continue;
		}
		pFileName[MAX_HBUFF-1] = 0;
		if(get_dirname(pNotifyPath[i].acNotifyPath, pNotifyPath[i].acNotifyPath, CHAR_MAX_SIZE-1) == NULL)
		{
			continue;
		}

		stDbLogDoc.strObjectName = pFileName;
		stDbLogDoc.strObjectPath = pNotifyPath[i].acNotifyPath;
		stDbLogDoc.strSubjectPath = SPrintf("%s/sbin", t_EnvInfo->m_strRootPath.c_str());
		stDbLogDoc.strSubjectName = STR_SERVICE_NAME;
		stDbLogDoc.strBkFileName = GetGUID();

		stDbLogDoc.nOpType = SS_LOG_DOC_OP_TYPE_DOCTRACE;
		stDbLogDoc.nPolicyType = pNotifyPath[i].nOrderID + ASI_EPS_APP_POLICY_GROUP_ID_FA_OP;

		stDbLogDoc.nBackupTime = 0;
		stDbLogDoc.nRemoveTime = 0;
		stDbLogDoc.nEvtTime = GetCurrentDateTimeInt();
		

		if(SetER(t_DocBackupUtil->BackupFile(&stDbLogDoc, nDelCnt, nLimitSize, nLimitDelCnt, nUsedMode)))
		{
			WriteLogE("[%s] doc backup operation fail : [er:%d][%s/%s]", m_strLogicName.c_str(), g_nErrRtn, pNotifyPath[i].acNotifyPath, pFileName);
			continue;
		}		
		HISYNCSTEPUP(stDbLogDoc.nSyncSvrStep);
		if(nUsedMode == STATUS_USED_MODE_WARN)
		{
			stDbLogDoc.nOpType = SS_LOG_DOC_OP_TYPE_WARNING;
		}
		SetLogDoc(stDbLogDoc);

		nDelDocCount++;
	}

	t_LogicMgrPoFaInotifyFile->DelAllInotifyPath(pNotifyPath, nListCount);
	safe_free(pNotifyPath);
	safe_free(pFileName);

	return nDelDocCount;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDoc::ChkDelBackupOp(UINT32& nContinue)
{
	INT32 nRmCount = 0;
	TListID tIDList;
	WriteLogN("[%s] doc remove backup operation start", m_strLogicName.c_str());
	t_ManageLogDoc->GetItemIDList(tIDList);
	TListIDRItor begin, end;
	begin = tIDList.rbegin();	end = tIDList.rend();
	for(begin; begin != end && !nContinue; begin++)
	{
		PDB_LOG_DOC pdld = t_ManageLogDoc->FindItem(*begin);
		if(!pdld)	continue;

		if(pdld->nBackupTime == 0 || pdld->nRemoveTime || !(pdld->nOpType == SS_LOG_DOC_OP_TYPE_DOCTRACE || pdld->nOpType == SS_LOG_DOC_OP_TYPE_SCHEDULE))		continue;

		if(SetER(t_DocBackupUtil->RemoveBackupFile(pdld)))
		{
			if(g_nErrRtn > 10)	continue;

			WriteLogE("[%s] doc remove backup operation fail : [er:%d][id:%d]", m_strLogicName.c_str(), g_nErrRtn, pdld->nID);
			continue;
		}

		nRmCount++;

		HISYNCSTEPUP(pdld->nSyncSvrStep);
		SetLogDoc(*pdld);
	}

	WriteLogN("[%s] doc remove backup operation end [rm_count:%d]", m_strLogicName.c_str(), nRmCount);

	return nRmCount;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogDoc::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageLogDoc->SetPktSync(tSendList);

		if(tSendList.empty())	return;
	}

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;
	INT32 nRetVal = 0;

	TListPVOIDItor begin, end;
	begin = tSendList.begin();	end = tSendList.end();

	while(nSendNum < tSendList.size())
	{
		nOnceNum = (((tSendList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tSendList.size() - nSendNum));
		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManageLogDoc->SetPkt((PDB_LOG_DOC)(*begin), SendToken);
			nSendNum += 1;
			nOnceNum -= 1;
		}
		nRetVal = SendData_Mgr(G_TYPE_LOG_DOC, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------

