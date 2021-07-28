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
#include "LogicMgrPoInDevOWL.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevOWL*		t_LogicMgrPoInDevOWL = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevOWL::CLogicMgrPoInDevOWL()
{
	t_ManagePoInDevOWL			= new CManagePoInDevOWL();
	t_ManagePoInDevOWLPkg		= new CManagePoInDevOWLPkg();

	t_ManagePoInDevOWL->LoadDBMS();
	t_ManagePoInDevOWLPkg->LoadDBMS();

	t_ManagePoInDevOWL->InitHash();

	m_strLogicName		= "mgr agt po in devo wl";
	
	m_nPolicyType		= SS_POLICY_TYPE_DV_WL;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DV_WL;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_WL;
}
//---------------------------------------------------------------------------

CLogicMgrPoInDevOWL::~CLogicMgrPoInDevOWL()
{
	SAFE_DELETE(t_ManagePoInDevOWLPkg);
	SAFE_DELETE(t_ManagePoInDevOWL);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOWL::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInDevOWL::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_DEVO_WL pdpdb = NULL;
	DB_PO_IN_DEVO_WL dpdb;
	m_tDPH = &(dpdb.tDPH);
	TListDBPoInDevOWLPkg	tPkgList;

	if( t_ManagePoInDevOWL->GetPkt(RecvToken, dpdb))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_DEVO_WL_PKG dppp;		
		if( t_ManagePoInDevOWLPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		tPkgList.push_back(dppp);
	}

	pdpdb = (PDB_PO_IN_DEVO_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpdb)
	{
		t_ManagePoInDevOWLPkg->ClearPkgUnitByPolicyID(pdpdb->tDPH.nID);		
		t_ManagePoInDevOWL->DelPoInDevOWL(pdpdb->tDPH.nID);
	}

	{
		{
			TListDBPoInDevOWLPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInDevOWLPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInDevOWLPkg->AddPoInDevOWLPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInDevOWL->ApplyPoInDevOWL(dpdb)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}
//	t_ASIEPSAPPDLLUtil->Stop();
	t_ManageDevOInfo->CheckDvPolicy();
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
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
//---------------------------------------------------------------------------
