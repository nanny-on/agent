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
#include "LogicMgrPoSelfProtect.h"

//---------------------------------------------------------------------------

CLogicMgrPoSelfProtect*		t_LogicMgrPoSelfProtect = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoSelfProtect::CLogicMgrPoSelfProtect()
{
	t_ManagePoSelfProtect			= new CManagePoSelfProtect();
	t_ManagePoSelfProtectPkg		= new CManagePoSelfProtectPkg();
	t_ManagePoSelfProtectUnit		= new CManagePoSelfProtectUnit();

	t_ManagePoSelfProtect->LoadDBMS();
	t_ManagePoSelfProtectPkg->LoadDBMS();
	t_ManagePoSelfProtectUnit->LoadDBMS();

	t_ManagePoSelfProtect->InitHash();

	m_strLogicName			= "mgr agt po self protect";
	
	m_nPolicyType			= SS_POLICY_TYPE_SELF_PROTECT;
	m_nSSPoSeqIdx			= SS_POLICY_INDEX_SELF_PROTECT;
	m_nAgtPktEditCode		= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType			= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode			= EVENT_OBJECT_CODE_POLICY_SELF_PROTECT;
}
//---------------------------------------------------------------------------

CLogicMgrPoSelfProtect::~CLogicMgrPoSelfProtect()
{
	SAFE_DELETE(t_ManagePoSelfProtect);
	SAFE_DELETE(t_ManagePoSelfProtectPkg);
	SAFE_DELETE(t_ManagePoSelfProtectUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoSelfProtect::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoSelfProtect::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_SELF_PROTECT	ppsp = NULL;
	DB_PO_SELF_PROTECT	dpsp;

	m_tDPH = &(dpsp.tDPH);
	TListDBPoSelfProtectPkg		tPkgList;
	TListDBPoSelfProtectUnit	tUnitList;

	if( t_ManagePoSelfProtect->GetPkt(RecvToken, dpsp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_SELF_PROTECT_PKG	dppp;		
		DB_PO_SELF_PROTECT_UNIT dppu;

		if( t_ManagePoSelfProtectPkg->GetPkt(RecvToken, dppp))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoSelfProtectUnit->GetPkt(RecvToken, dppu))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpsp.tDPH._add_id(dppp.tDPH.nID);
	}

	ppsp = (PDB_PO_SELF_PROTECT)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppsp)
	{
		t_ManagePoSelfProtectPkg->ClearItemByPolicyID(ppsp->tDPH.nID);
		t_ManagePoSelfProtect->DelPoSelfProtect(ppsp->tDPH.nID);
	}

	{
		{
			TListDBPoSelfProtectUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoSelfProtectUnit->ApplyPoSelfProtectUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}
			}
		}

		{
			TListDBPoSelfProtectPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoSelfProtectPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoSelfProtectPkg->AddPoSelfProtectPkg(*begin);
			}
		}

		if(SetER(t_ManagePoSelfProtect->ApplyPoSelfProtect(dpsp)))
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
//---------------------------------------------------------------------------
