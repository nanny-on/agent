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
#include "LogicMgrPoInPtnEx.h"

//---------------------------------------------------------------------------

CLogicMgrPoInPtnEx*		t_LogicMgrPoInPtnEx = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInPtnEx::CLogicMgrPoInPtnEx()
{
	t_ManagePoInPtnEx		= new CManagePoInPtnEx();
	t_ManagePoInPtnExPkg	= new CManagePoInPtnExPkg();
	t_ManagePoInPtnExUnit	= new CManagePoInPtnExUnit();

	t_ManagePoInPtnEx->LoadDBMS();
	t_ManagePoInPtnExPkg->LoadDBMS();
	t_ManagePoInPtnExUnit->LoadDBMS();

	t_ManagePoInPtnEx->InitHash();

	m_strLogicName		= "mgr agt po in ptn ex";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_PTN_EX;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_PTN_EX;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_PTN_EX;
}
//---------------------------------------------------------------------------

CLogicMgrPoInPtnEx::~CLogicMgrPoInPtnEx()
{
	SAFE_DELETE(t_ManagePoInPtnEx);
	SAFE_DELETE(t_ManagePoInPtnExPkg);
	SAFE_DELETE(t_ManagePoInPtnExUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInPtnEx::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnEx::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_PTN_EX pdata = NULL;
	DB_PO_IN_PTN_EX data;

	m_tDPH = &(data.tDPH);

	TListDBPoInPtnExPkg	tPkgList;
	TListDBPoInPtnExUnit	tUnitList;

	if( t_ManagePoInPtnEx->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_PTN_EX_PKG		data_pkg;		
		DB_PO_IN_PTN_EX_UNIT	data_unit;

		if( t_ManagePoInPtnExPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInPtnExUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInPtnExPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInPtnEx->DelPoInPtnEx(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInPtnExUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnExUnit->ApplyPoInPtnExUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInPtnExPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnExPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInPtnExPkg->AddPoInPtnExPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInPtnEx->ApplyPoInPtnEx(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}

		t_ManagePoInPtnExTrust->SyncPoInPtnExTrust(data.tDPH.nID);
		t_LogicPoInPtnExTrust->SendPkt_Sync();
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

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
