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
#include "LogicMgrPoInDevOBL.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevOBL*		t_LogicMgrPoInDevOBL = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevOBL::CLogicMgrPoInDevOBL()
{
	t_ManagePoInDevOBL			= new CManagePoInDevOBL();
	t_ManagePoInDevOBLPkg		= new CManagePoInDevOBLPkg();

	t_ManagePoInDevOBL->LoadDBMS();
	t_ManagePoInDevOBLPkg->LoadDBMS();

	t_ManagePoInDevOBL->InitHash();

	m_strLogicName		= "mgr agt po in devo bl";
	
	m_nPolicyType		= SS_POLICY_TYPE_DV_BL;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DV_BL;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_BL;
}
//---------------------------------------------------------------------------

CLogicMgrPoInDevOBL::~CLogicMgrPoInDevOBL()
{
	SAFE_DELETE(t_ManagePoInDevOBLPkg);
	SAFE_DELETE(t_ManagePoInDevOBL);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOBL::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInDevOBL::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_DEVO_BL pdpdb = NULL;
	DB_PO_IN_DEVO_BL dpdb;
	m_tDPH = &(dpdb.tDPH);
	TListDBPoInDevOBLPkg	tPkgList;

	if( t_ManagePoInDevOBL->GetPkt(RecvToken, dpdb))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_DEVO_BL_PKG dppp;		
		if( t_ManagePoInDevOBLPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		tPkgList.push_back(dppp);
	}

	pdpdb = (PDB_PO_IN_DEVO_BL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpdb)
	{
		t_ManagePoInDevOBLPkg->ClearPkgUnitByPolicyID(pdpdb->tDPH.nID);		
		t_ManagePoInDevOBL->DelPoInDevOBL(pdpdb->tDPH.nID);
	}

	{
		{
			TListDBPoInDevOBLPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInDevOBLPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInDevOBLPkg->AddPoInDevOBLPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInDevOBL->ApplyPoInDevOBL(dpdb)))
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
