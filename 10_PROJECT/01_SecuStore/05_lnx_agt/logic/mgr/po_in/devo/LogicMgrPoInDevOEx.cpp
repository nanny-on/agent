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
#include "LogicMgrPoInDevOEx.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevOEx*		t_LogicMgrPoInDevOEx = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevOEx::CLogicMgrPoInDevOEx()
{
	t_ManagePoInDevOEx			= new CManagePoInDevOEx();
	t_ManagePoInDevOExPkg			= new CManagePoInDevOExPkg();
	t_ManagePoInDevOExUnit		= new CManagePoInDevOExUnit();

	t_ManagePoInDevOEx->LoadDBMS();
	t_ManagePoInDevOExPkg->LoadDBMS();
	t_ManagePoInDevOExUnit->LoadDBMS();

	t_ManagePoInDevOEx->InitHash();

	m_strLogicName		= "mgr agt po in devo ex";
	
	m_nPolicyType		= SS_POLICY_TYPE_DV_EX;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DV_EX;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	

	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_EX;
}
//---------------------------------------------------------------------------
CLogicMgrPoInDevOEx::~CLogicMgrPoInDevOEx()
{
	SAFE_DELETE(t_ManagePoInDevOExUnit);
	SAFE_DELETE(t_ManagePoInDevOExPkg);
	SAFE_DELETE(t_ManagePoInDevOEx);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOEx::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOEx::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_DEVO_EX pdphn = NULL;
	DB_PO_IN_DEVO_EX dphn;
	m_tDPH = &(dphn.tDPH);
	TListDBPoInDevOExPkg	tPkgList;
	TListDBPoInDevOExUnit	tUnitList;

	if( t_ManagePoInDevOEx->GetPkt(RecvToken, dphn))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_DEVO_EX_PKG dppp;		
		DB_PO_IN_DEVO_EX_UNIT dppu;

		if( t_ManagePoInDevOExPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInDevOExUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dphn.tDPH._add_id(dppp.tDPH.nID);
	}

	pdphn = (PDB_PO_IN_DEVO_EX)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdphn)
	{
		t_ManagePoInDevOExPkg->ClearPkgUnitByPolicyID(pdphn->tDPH.nID);		
		t_ManagePoInDevOEx->DelPoInDevOEx(pdphn->tDPH.nID);
	}

	{
		{
			TListDBPoInDevOExUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInDevOExUnit->ApplyPoInDevOExUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
			}
		}

		{
			TListDBPoInDevOExPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInDevOExPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				
				t_ManagePoInDevOExPkg->AddPoInDevOExPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInDevOEx->ApplyPoInDevOEx(dphn)))
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
