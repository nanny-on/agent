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
#include "LogicMgrPoCtlPanel.h"

//---------------------------------------------------------------------------

CLogicMgrPoCtlPanel*		t_LogicMgrPoCtlPanel = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoCtlPanel::CLogicMgrPoCtlPanel()
{
	t_ManagePoCtlPanel		= new CManagePoCtlPanel();
	t_ManagePoCtlPanelPkg	= new CManagePoCtlPanelPkg();
	t_ManagePoCtlPanelUnit	= new CManagePoCtlPanelUnit();

	t_ManagePoCtlPanel->LoadDBMS();
	t_ManagePoCtlPanelPkg->LoadDBMS();
	t_ManagePoCtlPanelUnit->LoadDBMS();

	t_ManagePoCtlPanel->InitHash();

	m_strLogicName		= "mgr agt po ctl panel";
	
	m_nPolicyType		= SS_POLICY_TYPE_CTL_PANEL;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_CTL_PANEL;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_CTL_PANEL;
}
//---------------------------------------------------------------------------

CLogicMgrPoCtlPanel::~CLogicMgrPoCtlPanel()
{
	SAFE_DELETE(t_ManagePoCtlPanel);
	SAFE_DELETE(t_ManagePoCtlPanelPkg);
	SAFE_DELETE(t_ManagePoCtlPanelUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoCtlPanel::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoCtlPanel::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_CTL_PANEL ppcp = NULL;
	DB_PO_CTL_PANEL dpcp;

	m_tDPH = &(dpcp.tDPH);

	TListDBPoCtlPanelPkg	tPkgList;
	TListDBPoCtlPanelUnit	tUnitList;

	if( t_ManagePoCtlPanel->GetPkt(RecvToken, dpcp))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_CTL_PANEL_PKG	dppp;		
		DB_PO_CTL_PANEL_UNIT dppu;

		if( t_ManagePoCtlPanelPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoCtlPanelUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpcp.tDPH._add_id(dppp.tDPH.nID);
	}

	ppcp = (PDB_PO_CTL_PANEL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoCtlPanelPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoCtlPanel->DelPoCtlPanel(ppcp->tDPH.nID);
	}

	{
		{
			TListDBPoCtlPanelUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoCtlPanelUnit->ApplyPoCtlPanelUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
			}
		}

		{
			TListDBPoCtlPanelPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoCtlPanelPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoCtlPanelPkg->AddPoCtlPanelPkg(*begin);
			}
		}

		if(SetER(t_ManagePoCtlPanel->ApplyPoCtlPanel(dpcp)))
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
