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
#include "LogicMgrPoInDevONotify.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevONotify*		t_LogicMgrPoInDevONotify = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevONotify::CLogicMgrPoInDevONotify()
{
	t_ManagePoInDevONotify			= new CManagePoInDevONotify();
	t_ManagePoInDevONotifyPkg		= new CManagePoInDevONotifyPkg();

	t_ManagePoInDevONotify->LoadDBMS();
	t_ManagePoInDevONotifyPkg->LoadDBMS();

	t_ManagePoInDevONotify->InitHash();

	m_strLogicName		= "mgr agt po in devo notify";
	
	m_nPolicyType		= SS_POLICY_TYPE_DV_NOTIFY;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_DV_NOTIFY;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_NOTIFY;
}
//---------------------------------------------------------------------------

CLogicMgrPoInDevONotify::~CLogicMgrPoInDevONotify()
{
	SAFE_DELETE(t_ManagePoInDevONotify);
	SAFE_DELETE(t_ManagePoInDevONotifyPkg);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevONotify::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInDevONotify::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_DEVO_NOTIFY ppcp = NULL;
	DB_PO_IN_DEVO_NOTIFY dpfn;

	m_tDPH = &(dpfn.tDPH);

	TListDBPoInDevONotifyPkg	tPkgList;
	
	if( t_ManagePoInDevONotify->GetPkt(RecvToken, dpfn))			return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_DEVO_NOTIFY_PKG	dppp;			
		if( t_ManagePoInDevONotifyPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);	

		tPkgList.push_back(dppp);	
		dpfn.tDPH._add_id(dppp.tDPH.nID);
	}

	ppcp = (PDB_PO_IN_DEVO_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoInDevONotifyPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoInDevONotify->DelPoInDevONotify(ppcp->tDPH.nID);
	}

	{
		{
			TListDBPoInDevONotifyPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInDevONotifyPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInDevONotifyPkg->AddPoInDevONotifyPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInDevONotify->ApplyPoInDevONotify(dpfn)))
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
