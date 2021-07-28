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
#include "LogicMgrPoNcPtnLo.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnLo*		t_LogicMgrPoNcPtnLo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnLo::CLogicMgrPoNcPtnLo()
{
	t_ManagePoNcPtnLo	= new CManagePoNcPtnLo();
	t_ManagePoNcPtnLo->LoadDBMS();
	t_ManagePoNcPtnLo->InitHash();

	t_LogicPoNcPtnLoPtn		= new CLogicPoNcPtnLoPtn();
	t_LogicMgrPtnLWC		= new CLogicMgrPtnLWC();

	m_strLogicName		= "mgr po nc ptn lo";

	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_LO;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_NC_PTN_LO;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnLo::~CLogicMgrPoNcPtnLo()
{
	SAFE_DELETE(t_LogicMgrPtnLWC);

	SAFE_DELETE(t_LogicPoNcPtnLoPtn);
	SAFE_DELETE(t_ManagePoNcPtnLo);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_NC_PTN_LO pdata = NULL;
	DB_PO_NC_PTN_LO data;
	m_tDPH = &(data.tDPH);

	if( t_ManagePoNcPtnLo->GetPkt(RecvToken, data))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		if( data.nPtnRangeType == pdata->nPtnRangeType &&
			data.nPtnObjType	== pdata->nPtnObjType)
		{
			data.strSavePath	= pdata->strSavePath;
			data.strSaveName	= pdata->strSaveName;
			data.strPtnVersion	= pdata->strPtnVersion;
			data.strFileHash	= pdata->strFileHash;
			data.nPtnChkValue	= pdata->nPtnChkValue;
			data.nPtnUnitNum	= pdata->nPtnUnitNum;
		}
		
		t_ManagePoNcPtnLo->DelPoNcPtnLo(pdata->tDPH.nID);
	}
	
	{
		if(SetER(t_ManagePoNcPtnLo->ApplyPoNcPtnLo(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::ApplyPolicy()
{
	PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoNcPtnLo->IsValidPtnFile(pdata) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				if(t_ThreadPoNcPtnLo)
					t_ThreadPoNcPtnLo->AddWork(MAKEUINT64(pdata->tDPH.nID, pdata->tDPH.nSeqNo));
			}
			else if(pdata->tDPH.nExtOption & SS_PO_NC_PTN_LO_EXT_OPTION_AUTO_MAKE_PTN)
			{
				if(t_ThreadPoNcPtnLo)
					t_ThreadPoNcPtnLo->AddWork(MAKEUINT64(pdata->tDPH.nID, pdata->tDPH.nSeqNo));
				WriteLogN("[%s] remake nc ptn local ptn by policy option : auto", m_strLogicName.c_str());
			}
			else
			{
				if(t_LogicMgrPtnLWC->ReloadPtnLWC())
				{
					WriteLogE("[%s] can not reload lwc ptn file  : [%s]", m_strLogicName.c_str(), pdata->strSaveName.c_str());
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

INT32		CLogicMgrPoNcPtnLo::CheckRunEnv()
{
	PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoNcPtnLo->IsValidPtnFile(pdata) == 0)
		{
			if(t_ThreadPoNcPtnLo)
				t_ThreadPoNcPtnLo->AddWork(MAKEUINT64(pdata->tDPH.nID, pdata->tDPH.nSeqNo));
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::LoadPtn()
{
	PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoNcPtnLo->IsValidPtnFile(pdata) == 0)
			{
				if(t_EnvInfoOp->m_nApplyPolicyStatus)
				{
					if(t_ThreadPoNcPtnLo)
						t_ThreadPoNcPtnLo->AddWork(MAKEUINT64(pdata->tDPH.nID, pdata->tDPH.nSeqNo));
				}
			}
			else
			{
				if(t_LogicMgrPtnLWC->ReloadPtnLWC())
				{
					WriteLogE("[%s] can not reload lwc ptn file  : [%s]", m_strLogicName.c_str(), pdata->strSaveName.c_str());
					return 0;
				}
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::MakePtn()
{
	PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ThreadPoNcPtnLo)
				t_ThreadPoNcPtnLo->AddWork(MAKEUINT64(pdata->tDPH.nID, pdata->tDPH.nSeqNo));
			WriteLogE("[%s] remake ptn start", m_strLogicName.c_str());
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnLo::MakePtnEnd(PDB_PO_NC_PTN_LO pDPFPO)
{
	PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata || pdata->tDPH.nSeqNo != pDPFPO->tDPH.nSeqNo || pdata->tDPH.nID != pDPFPO->tDPH.nID)
	{
		WriteLogE("[%s] invalid policy information : do not policy update", m_strLogicName.c_str());
		return -2;
	}

	if(_stricmp(pdata->strSaveName.c_str(), pDPFPO->strSaveName.c_str()))
	{
		CFileUtil tFileUtil;
		String strFileName;
		strFileName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), pdata->strSavePath.c_str(), pdata->strSaveName.c_str());
		if(tFileUtil.FileExists(strFileName.c_str()))
		{
			unlink(strFileName.c_str());
		}
	}

	{
		pdata->strSavePath		= pDPFPO->strSavePath;
		pdata->strSaveName		= pDPFPO->strSaveName;
		pdata->strPtnVersion	= pDPFPO->strPtnVersion;
		pdata->strFileHash		= pDPFPO->strFileHash;
		pdata->nPtnUnitNum		= pDPFPO->nPtnUnitNum;
	}

	{
		if(SetER(t_ManagePoNcPtnLo->ApplyPoNcPtnLo(*pdata)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return -1;
		}
	}	

	{
		DB_HOST_STATUS_PO_NC data_nc;
		PDB_HOST_STATUS_PO_NC pdata_nc = t_ManageHostStatusPoNc->FirstItem();
		if(pdata_nc)	data_nc = *pdata_nc;

		{
			data_nc.tPtnVerMap[SS_HOST_STATUS_PO_NC_INFO_INDEX_LO] = pdata->strPtnVersion;
			t_ManageHostStatusPoNc->ApplyHostStatusPoNc(data_nc);

			t_LogicMgrHostStatusPo->SendPkt_HostStatusPo();
		}
	}

	
	{
		if(t_LogicMgrPtnLWC != NULL && t_ThreadPoNcPtnLo != NULL)
		{
			if(t_LogicMgrPtnLWC->ReloadPtnLWC() == 0 && t_ThreadPoNcPtnLo->IsWork() == 0)
			{
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);

				t_LogicPoNcPtnLoPtn->SendPkt_Sync();
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
