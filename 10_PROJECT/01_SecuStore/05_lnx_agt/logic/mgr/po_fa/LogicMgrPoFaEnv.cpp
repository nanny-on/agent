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
#include "LogicMgrPoFaEnv.h"

//---------------------------------------------------------------------------

CLogicMgrPoFaEnv*		t_LogicMgrPoFaEnv = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFaEnv::CLogicMgrPoFaEnv()
{
	t_ManagePoFaEnv		= new CManagePoFaEnv();
	t_ManagePoFaEnvPkg	= new CManagePoFaEnvPkg();
	t_ManagePoFaEnvUnit	= new CManagePoFaEnvUnit();

	t_ManagePoFaEnv->LoadDBMS();
	t_ManagePoFaEnvPkg->LoadDBMS();
	t_ManagePoFaEnvUnit->LoadDBMS();

	t_ManagePoFaEnv->InitHash();

	m_strLogicName		= "mgr agt po fa env";
	
	m_nPolicyType		= SS_POLICY_TYPE_FA_ENV;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_FA_ENV;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FA_ENV;
}
//---------------------------------------------------------------------------

CLogicMgrPoFaEnv::~CLogicMgrPoFaEnv()
{
	SAFE_DELETE(t_ManagePoFaEnv);
	SAFE_DELETE(t_ManagePoFaEnvPkg);
	SAFE_DELETE(t_ManagePoFaEnvUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoFaEnv::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaEnv::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FA_ENV ppcp = NULL;
	DB_PO_FA_ENV dpfe;

	m_tDPH = &(dpfe.tDPH);

	TListDBPoFaEnvPkg	tPkgList;
	TListDBPoFaEnvUnit	tUnitList;

	if( t_ManagePoFaEnv->GetPkt(RecvToken, dpfe))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_FA_ENV_PKG	dppp;		
		DB_PO_FA_ENV_UNIT dppu;

		if( t_ManagePoFaEnvPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoFaEnvUnit->GetPkt(RecvToken, dppu))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpfe.tDPH._add_id(dppp.tDPH.nID);
	}
/*
	ppcp = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoFaEnvPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoFaEnv->DelPoFaEnv(ppcp->tDPH.nID);
	}
*/

	t_ManagePoFaEnvPkg->ClearAllItem();
	t_ManagePoFaEnv->ClearPoFaEnv();

	{
		{
			TListDBPoFaEnvUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFaEnvUnit->ApplyPoFaEnvUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
			}
		}

		{
			TListDBPoFaEnvPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFaEnvPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoFaEnvPkg->AddPoFaEnvPkg(*begin);
			}
		}

		if(SetER(t_ManagePoFaEnv->ApplyPoFaEnv(dpfe)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaEnv::ApplyPolicy()
{
	PDB_PO_FA_ENV pdb_pol = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_pol)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	DB_ENV_NOTIFY_INFO db_info;
	db_info.tDPH.nUsedMode = 0;

	PDB_ENV_NOTIFY_INFO pdb_info = t_ManageEnvNotifyInfo->FindItem(pdb_pol->tDPH.nTargetOS);
	if(pdb_info)	db_info = *pdb_info;

	String const strJobTitle	= "Nanny-I_Notify";
	String const strRegPath	= "SOFTWARE/Microsoft/Windows/CurrentVersion/Run";

	CFileUtil tFileUtil;
	String strNotifyInfoFile;
	{
		strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());
		tFileUtil.ForceDirectory(strNotifyInfoFile.c_str());
		strNotifyInfoFile = SPrintf("%s/inven/info/fa_env_notify.info", t_EnvInfo->m_strRootPath.c_str());

		unlink(strNotifyInfoFile.c_str());
	}

//	t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NOTIFY_MSG);

	CRegUtil tRegUtil;
//	tRegUtil.SetWowDefaultKey();
	tRegUtil.RegDeleteValueExt((HKEY)HKEY_LOCAL_MACHINE, strRegPath.c_str(), strJobTitle.c_str());

	if(pdb_pol->tDPH.nUsedMode == STATUS_USED_MODE_OFF || db_info.tDPH.nUsedMode == STATUS_USED_MODE_OFF)
		return 0;

	String strArgument;
	{
		strArgument = SPrintf("used_mode=%u\f"
			"pos_type=%u\f"
			"show_size=%u\f"
			"show_pos=%u\f"
			"show_time=%u\f"
			"msg_fmt_type=%u\f"
			"msg_info=%s\f",
			db_info.tDPH.nUsedMode, db_info.nPosType, db_info.nShowSize, db_info.nShowPos, db_info.nShowTime,
			db_info.nMsgFmtType, db_info.strMsgInfo.c_str());
	}

	{
		tFileUtil.WriteFileBuffer(strNotifyInfoFile, strArgument);
		strArgument = SPrintf("/%s/", strNotifyInfoFile.c_str());
	}


	{
		String strRegValue;
		strRegValue = SPrintf("\"%s/util/%s\" \"%s\"", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_INTRO, strArgument.c_str());

		tRegUtil.RegWriteString((HKEY)HKEY_LOCAL_MACHINE, strRegPath.c_str(), strJobTitle.c_str(), strRegValue.c_str());
	}

	return 0;
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoFaEnv::ApplyPolicyBySchduleManager()
{
	PDB_PO_FA_ENV pdb_pol = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_pol)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}
	DB_ENV_NOTIFY_INFO db_info;
	db_info.tDPH.nUsedMode = 0;

	PDB_ENV_NOTIFY_INFO pdb_info = t_ManageEnvNotifyInfo->FindItem(pdb_pol->tDPH.nTargetOS);
	if(pdb_info)	db_info = *pdb_info;

	CFileUtil tFileUtil;
	String strNotifyInfoFile;
	{
		strNotifyInfoFile = SPrintf("%s/inven/info", t_EnvInfo->m_strRootPath.c_str());
		tFileUtil.ForceDirectory(strNotifyInfoFile.c_str());
		strNotifyInfoFile = SPrintf("%s/inven/info/fa_env_notify.info", t_EnvInfo->m_strRootPath.c_str());

		unlink(strNotifyInfoFile.c_str());
	}

	if(pdb_pol->tDPH.nUsedMode == STATUS_USED_MODE_OFF || db_info.tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
//		t_ASITSDLLUtil->DelTaskInfo(STR_TS_NAME_NOTIFY_MSG);
		return 0;
	}

	{
		String strChildPath, strChildArg;

		{
			CHAR szShortPath[CHAR_MAX_SIZE] = {0, };
			strChildPath = SPrintf("%s/util/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PROC_NAME_NANNY_INTRO);
//			GetShortPathName(strChildPath, szShortPath, MAX_PATH);
			strChildPath = szShortPath;
		}

		{
			strChildArg = SPrintf("used_mode=%u\f"
				"pos_type=%u\f"
				"show_size=%u\f"
				"show_pos=%u\f"
				"show_time=%u\f"
				"msg_fmt_type=%u\f"
				"msg_info=%s\f",
				db_info.tDPH.nUsedMode, db_info.nPosType, db_info.nShowSize, db_info.nShowPos, db_info.nShowTime,
				db_info.nMsgFmtType, db_info.strMsgInfo.c_str());
		}

		{
			tFileUtil.WriteFileBuffer(strNotifyInfoFile, strChildArg);
			strChildArg = SPrintf("/%s/", strNotifyInfoFile.c_str());
		}

		ASI_TS_INFO tATI;
		ASI_TS_MGR_PARAM tATMP;
		{
			sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, "%s", STR_TS_NAME_NOTIFY_MSG); 
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_USER_LOGIN;
			tATI.nStartTime	= GetCurrentDateTimeInt();

			tATMP.strTSChildPath = strChildPath;
			tATMP.strTSChildArgument = strChildArg;
		}
		t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);
	}

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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
