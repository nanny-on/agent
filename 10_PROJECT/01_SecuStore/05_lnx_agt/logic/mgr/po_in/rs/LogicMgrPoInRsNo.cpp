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
#include "LogicMgrPoInRsNo.h"

//---------------------------------------------------------------------------

CLogicMgrPoInRsNo*		t_LogicMgrPoInRsNo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInRsNo::CLogicMgrPoInRsNo()
{
	t_ManagePoInRsNo			= new CManagePoInRsNo();
	t_ManagePoInRsNoPkg		= new CManagePoInRsNoPkg();

	t_ManagePoInRsNo->LoadDBMS();
	t_ManagePoInRsNoPkg->LoadDBMS();

	t_ManagePoInRsNo->InitHash();

	m_strLogicName		= "mgr agt po in rs no";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_RS_NO;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_RS_NO;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_RS_NO;
}
//---------------------------------------------------------------------------

CLogicMgrPoInRsNo::~CLogicMgrPoInRsNo()
{
	SAFE_DELETE(t_ManagePoInRsNo);
	SAFE_DELETE(t_ManagePoInRsNoPkg);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInRsNo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInRsNo::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_RS_NO ppcp = NULL;
	DB_PO_IN_RS_NO data;

	m_tDPH = &(data.tDPH);

	TListDBPoInRsNoPkg	tPkgList;
	
	if( t_ManagePoInRsNo->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_RS_NO_PKG	dppp;			
		if( t_ManagePoInRsNoPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);	

		tPkgList.push_back(dppp);	
		data.tDPH._add_id(dppp.tDPH.nID);
	}

	ppcp = (PDB_PO_IN_RS_NO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoInRsNoPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoInRsNo->DelPoInRsNo(ppcp->tDPH.nID);
	}

	{
		{
			TListDBPoInRsNoPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInRsNoPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInRsNoPkg->AddPoInRsNoPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInRsNo->ApplyPoInRsNo(data)))
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
