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
#include "LogicMgrPoNcPtnWL.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnWL*		t_LogicMgrPoNcPtnWL = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnWL::CLogicMgrPoNcPtnWL()
{
	t_ManagePoNcPtnWL		= new CManagePoNcPtnWL();
	t_ManagePoNcPtnWLPkg	= new CManagePoNcPtnWLPkg();
	t_ManagePoNcPtnWLUnit	= new CManagePoNcPtnWLUnit();

	t_LogicMgrPtnGWC		= new CLogicMgrPtnGWC();


	t_ManagePoNcPtnWL->LoadDBMS();
	t_ManagePoNcPtnWLPkg->LoadDBMS();
	t_ManagePoNcPtnWLUnit->LoadDBMS();

	t_ManagePoNcPtnWL->InitHash();

	m_strLogicName		= "mgr agt po nc ptn wl";
	
	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_WL;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_NC_PTN_WL;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;

	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_NC_PTN_WL;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnWL::~CLogicMgrPoNcPtnWL()
{
	SAFE_DELETE(t_LogicMgrPtnGWC);

	SAFE_DELETE(t_ManagePoNcPtnWLUnit);
	SAFE_DELETE(t_ManagePoNcPtnWLPkg);
	SAFE_DELETE(t_ManagePoNcPtnWL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoNcPtnWL::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_NC_PTN_WL pdata = NULL;
	DB_PO_NC_PTN_WL data;

	m_tDPH = &(data.tDPH);

	TListDBPoNcPtnWLPkg		tPkgList;
	TListDBPoNcPtnWLUnit	tUnitList;

	if( t_ManagePoNcPtnWL->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_NC_PTN_WL_PKG	data_pkg;		
		DB_PO_NC_PTN_WL_UNIT data_unit;

		if( t_ManagePoNcPtnWLPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoNcPtnWLUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoNcPtnWLPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoNcPtnWL->DelPoNcPtnWL(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoNcPtnWLUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnWLUnit->ApplyPoNcPtnWLUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoNcPtnWLPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnWLPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoNcPtnWLPkg->AddPoNcPtnWLPkg(*begin);
			}
		}

		if(SetER(t_ManagePoNcPtnWL->ApplyPoNcPtnWL(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::ApplyPolicy()
{
	PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoNcPtnWL->IsValidPtnFile(pdata) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				AddDpDownInfo();
			}
			else
			{
				if(t_LogicMgrPtnGWC->ReloadPtnGWC())
				{
					WriteLogE("[%s] can not reload gwc ptn file  : [%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str());
					return 0;
				}
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
			}
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::CheckRunEnv()
{
	PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoNcPtnWL->IsValidPtnFile(pdata) == 0)
		{
			AddDpDownInfo();
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::LoadPtn()
{
	PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoNcPtnWL->IsValidPtnFile(pdata) == 0)
			{
				if(t_EnvInfoOp->m_nApplyPolicyStatus)
				{
					AddDpDownInfo();
				}
			}
			else
			{
				if(t_LogicMgrPtnGWC->ReloadPtnGWC())
				{
					WriteLogE("[%s] can not reload gwc ptn file  : [%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str());
					return 0;
				}
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnWL::AddDpDownInfo()
{
	PDB_PO_SVR_INFO_UDT pdata_us = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdata_us)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName.c_str());
		return -1;
	}
	// 	else if(pdata_us->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	// 	{
	// 		WriteLogE("[%s] not used udt chk svr info list", m_strLogicName.c_str());
	// 		return -2;
	// 	}
	else if(pdata_us->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName.c_str());
		return -2;
	}

	PDB_PO_NC_PTN_WL pdata = (PDB_PO_NC_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)
	{
		WriteLogE("[%s] not find current policy info", m_strLogicName.c_str());
		return -3;
	}

	if(pdata->tDPFI.strSaveName.empty() || pdata->tDPFI.strFileHash.empty())
	{
		WriteLogE("[%s] invalid ptn file info [%s][%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str(), pdata->tDPFI.strFileHash.c_str());
		return -4;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = SS_DN_FILE_TYPE_WPTN_GWC;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdata->tDPH.nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_WPTN, MAX_PATH-1);
		tAFI.szDLPath[MAX_PATH-1] = 0;

		{	
			strncpy(tAFI.szFileName, pdata->tDPFI.strSaveName.c_str(), MAX_PATH-1);
			tAFI.szFileName[MAX_PATH-1] = 0;
			strncpy(tAFI.szFileHash, pdata->tDPFI.strFileHash.c_str(), MAX_PATH-1);
			tAFI.szFileHash[MAX_PATH-1] = 0;
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists nc ptn wl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdata_us->strDataSvrInfoList.c_str());
			return 0;
		}

		WriteLogN("start nc ptn wl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdata_us->strDataSvrInfoList.c_str());
//		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdata_us->strDataSvrInfoList);
//		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdata_us->strDataSvrInfoList.c_str());
		AddDLInfo(&tAFI);
		t_ManageFileDown->AddItem(tAFI);
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
