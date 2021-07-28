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
#include "LogicMgrLogRsBk.h"

//---------------------------------------------------------------------------

CLogicMgrLogRsBk*		t_LogicMgrLogRsBk = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogRsBk::CLogicMgrLogRsBk()
{		
	t_ManageLogRsBk	= new CManageLogRsBk();
	t_LogicLogRsBk	= new CLogicLogRsBk();

	t_ManageLogRsBk->LoadDBMS();

	m_strLogicName		= "mgr log rs bk";

	m_nControlType		= SS_CONTROL_TYPE_LOG_RS_BK_ROLLBACK;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogRsBk::~CLogicMgrLogRsBk()
{
	SAFE_DELETE(t_LogicLogRsBk);
	SAFE_DELETE(t_ManageLogRsBk);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRsBk::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
// 		case G_CODE_COMMON_FIND:					nRtn = AnalyzePkt_FromMgr_Ext_Find();				break;
// 		case G_CODE_COMMON_SCAN:					nRtn = AnalyzePkt_FromMgr_Ext_Scan();				break;
// 		case G_CODE_COMMON_SYNC:					nRtn = AnalyzePkt_FromMgr_Ext_Sync();				break;
// 		case G_CODE_COMMON_ROLLBACK:				nRtn = AnalyzePkt_FromMgr_Ext_RollBack();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRsBk::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_LOG_RS_BK data;
	UINT32 nID = 0;
	if(t_ManageLogRsBk->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

	data.nEvtTime	= t_ValidTimeUtil->GetValidTime();
	HISYNCSTEPUP(data.nSyncSvrStep);

// 	nID = t_ManageLogRsBk->IsExistLogRsBk(data);
// 
// 	if(nID)
// 	{
// 		return AZPKT_CB_RTN_SUCCESS_END;
// 	}

	SetLogRsBk(data);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRsBk::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogRsBk::AnalyzePkt_FromMgr_Del_Ext()
{
// 	DB_LOG_RS_BK data;	
// 	if(t_ManageLogRsBk->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;
// 
// 	DeleteLogRsBk(data);

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

INT32		CLogicMgrLogRsBk::MakeLogRsBk()
{
	PDB_PO_IN_RS_BK pdata_bk = (PDB_PO_IN_RS_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_BK);
	if(!pdata_bk)	
	{
		WriteLogE("[%s] not find in rs bk policy", m_strLogicName.c_str());
		return 0;
	}

	PDB_PO_IN_RS_OP pdata_op = (PDB_PO_IN_RS_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_OP);
	if(!pdata_op)	
	{
		WriteLogE("[%s] not find in rs op policy", m_strLogicName.c_str());
		return 0;
	}

	if(pdata_bk->tDPH.nUsedMode != STATUS_USED_MODE_ON)	
	{
		return 0;
	}

/*
	{
		MEM_FIND_ORDER_INFO tMFOI;
		tMFOI.nPoType	= SS_POLICY_TYPE_IN_RS_BK;
		tMFOI.nPoID		= pdata_op->tDPH.nID;

		tMFOI._set_opt(0, pdata_bk->nFExtType);
		tMFOI._set_str_opt(0, pdata_bk->strFExtInfo);

		t_ThreadPoInRsBk->AddWork(&tMFOI);
	}
*/
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

void		CLogicMgrLogRsBk::SetLogRsBk(DB_LOG_RS_BK& data)
{
	PDB_PO_IN_RS_BK pdata_bk = (PDB_PO_IN_RS_BK)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_RS_BK);
	if(!pdata_bk || pdata_bk->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	{
		return;
	}

	{
//		data.strObjectName = _strlwr(data.strObjectName.c_str());
//		data.strObjectPath = _strlwr(data.strObjectPath.c_str());
		if(data.nRegDate == 0)	data.nRegDate = GetCurrentDateTimeInt();
	}
	
	String strObjectName = SPrintf("%s/%s", data.strObjectPath.c_str(), data.strObjectName.c_str());

	if(!ISSYNCSTEP(data.nSyncSvrStep)) 
	{
		if(data.strObjectPath.find("%") == string::npos && data.strObjectName.find("%") == string::npos  &&
			data.strSubjectPath.find("%") == string::npos  && data.strSubjectName.find("%") == string::npos )
		{

			WriteLogN("[%s] remain evt log to file : op[%d]:bs[%d]:bk[%d]:fi[%s/%s:%s/%s][%s]", 
				m_strLogicName.c_str(),
				data.nOpType, data.nBackupSize, data.nBackupTime,
				data.strSubjectPath.c_str(), data.strSubjectName.c_str(), 
				data.strObjectPath.c_str(), data.strObjectName.c_str(), data.strBkFileName.c_str());

		}
	}

	{
		data.nSkipTarget = SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER;
	}
	
	if(data.strOrFileHash.empty())
	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		if (SetER(SHAFile(ASIHASH_SHA_LEN_TYPE_256, strObjectName.c_str(), szHash, CHAR_MAX_SIZE)))
		{
			WriteLogE("[%s] get file hash fail : [%s]", m_strLogicName.c_str(), strObjectName.c_str());
			return;
		}
		data.strOrFileHash = szHash;
	}

	if(t_ManageLogRsBk->CompareLogRsBkLast(data) == 0)
	{
		WriteLogN("[%s] get file hash is same : [%s]", m_strLogicName.c_str(), strObjectName.c_str());
		return;
	}

	PDB_LOG_RS_BK pdata = t_ManageLogRsBk->GetTodayLogRsBk(data);
	if(pdata)
	{		
		data.nID = pdata->nID;
		data.strBkFileName = pdata->strBkFileName;		
	}

	{
		t_DocBackupUtil->BackupFile(&data);
		t_ManageLogRsBk->ApplyLogRsBk(data);
	}

	{
		TListID tIDList;
		t_ManageLogRsBk->GetOldRsBk(&data, tIDList, pdata_bk->nFileKeepNum);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_RS_BK pdata_log = t_ManageLogRsBk->FindItem(*begin);
			if(!pdata_log)	continue;
			t_DocBackupUtil->RemoveBackupFile(pdata_log);

			t_ManageLogRsBk->DelLogRsBk(*begin);
		}
	}
	
	{	
		SendToken.Set(1024);
		if(!ISSYNCSTEP(data.nSyncSvrStep) && !(data.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))
		{
			SendToken.TokenAdd_32(1);
			t_ManageLogRsBk->SetPkt(&data, SendToken);

			SendData_Mgr(G_TYPE_LOG_RS_BK, G_CODE_COMMON_SYNC, SendToken);

			SendToken.Clear();
		}
		
		{
			SendToken.TokenAdd_32(1);
			t_ManageLogRsBk->SetPkt(&data, SendToken);
			SendData_Link(G_TYPE_LOG_RS_BK, G_CODE_COMMON_SYNC, SendToken);
			SendToken.Clear();
		}
	}
	return;
}
//---------------------------------------------------------------------------



