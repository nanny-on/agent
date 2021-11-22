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
#include "LogicMgrPoFeNotify.h"

//---------------------------------------------------------------------------

CLogicMgrPoFeNotify*		t_LogicMgrPoFeNotify = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFeNotify::CLogicMgrPoFeNotify()
{
	t_ManagePoFeNotify			= new CManagePoFeNotify();
	t_ManagePoFeNotifyPkg		= new CManagePoFeNotifyPkg();

	t_ManagePoFeNotify->LoadDBMS();
	t_ManagePoFeNotifyPkg->LoadDBMS();

	t_ManagePoFeNotify->InitHash();

	m_strLogicName		= "mgr agt po fe notify";
	
	m_nPolicyType		= SS_POLICY_TYPE_FE_NOTIFY;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_FE_NOTIFY;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_NOTIFY;
}
//---------------------------------------------------------------------------

CLogicMgrPoFeNotify::~CLogicMgrPoFeNotify()
{
	SAFE_DELETE(t_ManagePoFeNotify);
	SAFE_DELETE(t_ManagePoFeNotifyPkg);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoFeNotify::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFeNotify::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_NOTIFY ppcp = NULL;
	DB_PO_FE_NOTIFY dpfn;

	m_tDPH = &(dpfn.tDPH);

	TListDBPoFeNotifyPkg	tPkgList;
	
	if( t_ManagePoFeNotify->GetPkt(RecvToken, dpfn))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_FE_NOTIFY_PKG	dppp;			
		if( t_ManagePoFeNotifyPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);	

		tPkgList.push_back(dppp);	
		dpfn.tDPH._add_id(dppp.tDPH.nID);
	}

	ppcp = (PDB_PO_FE_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoFeNotifyPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoFeNotify->DelPoFeNotify(ppcp->tDPH.nID);
	}

	{
		{
			TListDBPoFeNotifyPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFeNotifyPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoFeNotifyPkg->AddPoFeNotifyPkg(*begin);
			}
		}

		if(SetER(t_ManagePoFeNotify->ApplyPoFeNotify(dpfn)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
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
