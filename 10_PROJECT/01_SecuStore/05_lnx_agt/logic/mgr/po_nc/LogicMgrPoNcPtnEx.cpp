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


#include "stdafx.h"
#include "com_struct.h"
#include "LogicMgrPoNcPtnEx.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnEx*		t_LogicMgrPoNcPtnEx = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnEx::CLogicMgrPoNcPtnEx()
{
	t_ManagePoNcPtnEx		= new CManagePoNcPtnEx();
	t_ManagePoNcPtnExPkg	= new CManagePoNcPtnExPkg();
	t_ManagePoNcPtnExUnit	= new CManagePoNcPtnExUnit();

	t_ManagePoNcPtnEx->LoadDBMS();
	t_ManagePoNcPtnExPkg->LoadDBMS();
	t_ManagePoNcPtnExUnit->LoadDBMS();

	t_ManagePoNcPtnEx->InitHash();

	m_strLogicName		= "mgr agt po nc ptn ex";
	
	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_EX;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_NC_PTN_EX;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_NC_PTN_EX;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnEx::~CLogicMgrPoNcPtnEx()
{
	SAFE_DELETE(t_ManagePoNcPtnEx);
	SAFE_DELETE(t_ManagePoNcPtnExPkg);
	SAFE_DELETE(t_ManagePoNcPtnExUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoNcPtnEx::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnEx::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_NC_PTN_EX pdata = NULL;
	DB_PO_NC_PTN_EX data;

	m_tDPH = &(data.tDPH);

	TListDBPoNcPtnExPkg	tPkgList;
	TListDBPoNcPtnExUnit	tUnitList;

	if( t_ManagePoNcPtnEx->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_NC_PTN_EX_PKG		data_pkg;		
		DB_PO_NC_PTN_EX_UNIT	data_unit;

		if( t_ManagePoNcPtnExPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoNcPtnExUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_NC_PTN_EX)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoNcPtnExPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoNcPtnEx->DelPoNcPtnEx(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoNcPtnExUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnExUnit->ApplyPoNcPtnExUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoNcPtnExPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnExPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoNcPtnExPkg->AddPoNcPtnExPkg(*begin);
			}
		}

		if(SetER(t_ManagePoNcPtnEx->ApplyPoNcPtnEx(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}

		t_ManagePoNcPtnExTrust->SyncPoNcPtnExTrust(data.tDPH.nID);
		t_LogicPoNcPtnExTrust->SendPkt_Sync();
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
