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
#include "LogicMgrPoPmOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoPmOp*		t_LogicMgrPoPmOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoPmOp::CLogicMgrPoPmOp()
{
	t_ManagePoPmOp		= new CManagePoPmOp();

	t_ManagePoPmOp->LoadDBMS();

	t_ManagePoPmOp->InitHash();

	m_strLogicName		= "mgr agt po pm op";
	
	m_nPolicyType		= SS_POLICY_TYPE_PM_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_PM_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_PM_OP;

	m_nMRPLastTime = 0;
}
//---------------------------------------------------------------------------

CLogicMgrPoPmOp::~CLogicMgrPoPmOp()
{
	SAFE_DELETE(t_ManagePoPmOp);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPmOp::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_PM_OP pdata = NULL;
	DB_PO_PM_OP data, data_old;

	m_tDPH = &(data.tDPH);
	
	if( t_ManagePoPmOp->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);	
	pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		data_old = *pdata;
		t_ManagePoPmOp->DelPoPmOp(pdata->tDPH.nID);
	}

	{
		if(SetER(t_ManagePoPmOp->ApplyPoPmOp(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	if(data_old.strPtnFileName != data.strPtnFileName)
	{
		m_nForceApplyPolicy = STATUS_MODE_ON;
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CLogicMgrPoPmOp::ApplyPolicy()
{
	PDB_PO_PM_OP pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoPmOp->IsValidPtnFile(pdata) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				AddDpDownInfo();
			}
			else if(m_nForceApplyPolicy)
			{
				if(t_LogicMgrPtnPatch->ReloadPtnPatch() == 0)
				{
					t_LogicMgrHostPatch->ScanPatchList(G_TYPE_PO_PM_OP, G_CODE_COMMON_EDIT);
					
					t_LogicMgrPtnPatch->InitPtnDeploy();
				}
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmOp::CheckRunEnv()
{
	PDB_PO_PM_OP pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -1;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoPmOp->IsValidPtnFile(pdata) == 0)
		{
			AddDpDownInfo();
			return -10;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmOp::AddDpDownInfo()
{
	PDB_PO_SVR_INFO_UDT pdpsiu = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdpsiu)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName.c_str());
		return -1;
	}
	else if(pdpsiu->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName.c_str());
		return -2;
	}

	PDB_PO_PM_OP pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -3;
	}

	String strDnFileName, strDnFileHash;
	strDnFileName = (pdata->nPtnDnFileType ? pdata->strPtnDnFileName : pdata->strPtnFileSvName);
	strDnFileHash = (pdata->nPtnDnFileType ? pdata->strPtnDnFileHash : pdata->strPtnFileHash);

	if(strDnFileName.empty() || strDnFileHash.empty())
	{
//		WriteLogE("[%s] invalid ptn file info [%s][%s]", m_strLogicName.c_str(), strDnFileName.c_str(), strDnFileHash.c_str());
		return -4;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = SS_DN_FILE_TYPE_PPTN_ASP;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdata->tDPH.nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_PPTN, MAX_PATH-1);

		{	
			strncpy(tAFI.szFileName, strDnFileName.c_str(), MAX_PATH-1);
			strncpy(tAFI.szFileHash, strDnFileHash.c_str(), MAX_PATH-1);
		}	

		if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
		{
			WriteLogN("already exists pm ptn op download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
			return 0;
		}

		WriteLogN("start pm ptn op download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdpsiu->strDataSvrInfoList.c_str());
/*
		t_ASIFDLDLLUtil->SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList);
		t_ASIFDLDLLUtil->AddDLInfo(&tAFI);
*/
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdpsiu->strDataSvrInfoList.c_str());
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

INT32		CLogicMgrPoPmOp::OnTimer_Logic()
{	
	if(t_ManageEnvLicense->IsInvalidRight(m_nPackageType, m_nPolicyType, m_nControlType))
	{
		return 0;
	}

	PDB_PO_PM_OP pdata = (PDB_PO_PM_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return 0;
	}

	if(t_LogicMgrPtnPatch->IsEnableRunMode(G_TYPE_PO_PM_EX))
	{
		//WriteLogN("[%s] can not check ... checking now to rollback policy ...", m_strLogicName.c_str());
		return 0;
	}

	{
		U64_SCHEDULE tDT;
		tDT.all = pdata->nMonRmPatch;
		
		if(tDT.U8.type == STATUS_USED_MODE_OFF)		return 0;
		UINT32 nCurMin = GetCurrentYearMonDayHourMin();
		if(nCurMin - m_nMRPLastTime < tDT.U16.value_16)		return 0;

		m_nMRPLastTime = nCurMin;

		t_LogicMgrHostPatch->ScanPatchList(G_TYPE_PO_PM_OP, G_CODE_COMMON_ROLLBACK);
	}
	return 0;
}
//---------------------------------------------------------------------------
