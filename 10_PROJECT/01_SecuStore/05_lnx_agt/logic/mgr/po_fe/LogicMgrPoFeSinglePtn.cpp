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
#include "LogicMgrPoFeSinglePtn.h"

//---------------------------------------------------------------------------

CLogicMgrPoFeSinglePtn*		t_LogicMgrPoFeSinglePtn = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFeSinglePtn::CLogicMgrPoFeSinglePtn()
{
	t_ManagePoFeSinglePtn		= new CManagePoFeSinglePtn();
	t_ManagePoFeSinglePtnUnit	= new CManagePoFeSinglePtnUnit();

	t_ManagePoFeSinglePtn->LoadDBMS();
	t_ManagePoFeSinglePtnUnit->LoadDBMS();

	t_ManagePoFeSinglePtn->InitHash();
	t_ManagePoFeSinglePtnUnit->InitHash();

	m_strLogicName		= "mgr penv fe single ptn";

	m_nPolicyType		= SS_POLICY_TYPE_FE_SINGLE_PTN;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_SINGLE_PTN;
}
//---------------------------------------------------------------------------

CLogicMgrPoFeSinglePtn::~CLogicMgrPoFeSinglePtn()
{
	SAFE_DELETE(t_ManagePoFeSinglePtnUnit);
	SAFE_DELETE(t_ManagePoFeSinglePtn);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFeSinglePtn::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFeSinglePtn::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_SINGLE_PTN pdpfsp = NULL;
	DB_PO_FE_SINGLE_PTN dpfsp;

	m_tDPH = &(dpfsp.tDPH);

	TListDBPoFeSinglePtnUnit	tUnitList;

	if( t_ManagePoFeSinglePtn->GetPkt(RecvToken, dpfsp))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_FE_SINGLE_PTN_UNIT dppu;

		if( t_ManagePoFeSinglePtnUnit->GetPkt(RecvToken, dppu))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(dppu);
	}

	pdpfsp = (PDB_PO_FE_SINGLE_PTN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpfsp)
	{
		t_ManagePoFeSinglePtn->DelPoFeSinglePtn(pdpfsp->tDPH.nID);
	}

	{
		TListID tIDList;
		t_ManagePoFeSinglePtnUnit->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoFeSinglePtnUnit->IsExistID(*begin, tUnitList))		continue;

			t_ManagePoFeSinglePtnUnit->DelPoFeSinglePtnUnit(*begin);
		}
	}

	{
		{
			TListDBPoFeSinglePtnUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFeSinglePtnUnit->ApplyPoFeSinglePtnUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		if(SetER(t_ManagePoFeSinglePtn->ApplyPoFeSinglePtn(dpfsp)))
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

INT32		CLogicMgrPoFeSinglePtn::ApplyPolicy()
{	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFeSinglePtn::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
