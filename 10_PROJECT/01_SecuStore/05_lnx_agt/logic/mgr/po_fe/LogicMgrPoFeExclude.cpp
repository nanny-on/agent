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
#include "LogicMgrPoFeExclude.h"

//---------------------------------------------------------------------------

CLogicMgrPoFeExclude*		t_LogicMgrPoFeExclude = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFeExclude::CLogicMgrPoFeExclude()
{
	t_ManagePoFeExclude		= new CManagePoFeExclude();
	t_ManagePoFeExcludePkg	= new CManagePoFeExcludePkg();
	t_ManagePoFeExcludeUnit	= new CManagePoFeExcludeUnit();

	t_ManagePoFeExclude->LoadDBMS();
	t_ManagePoFeExcludePkg->LoadDBMS();
	t_ManagePoFeExcludeUnit->LoadDBMS();

	t_ManagePoFeExclude->InitHash();

	m_strLogicName		= "mgr agt po fe exclude";
	
	m_nPolicyType		= SS_POLICY_TYPE_FE_EXCLUDE;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_FE_EXCLUDE;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FE_EXCLUDE;
}
//---------------------------------------------------------------------------

CLogicMgrPoFeExclude::~CLogicMgrPoFeExclude()
{
	SAFE_DELETE(t_ManagePoFeExclude);
	SAFE_DELETE(t_ManagePoFeExcludePkg);
	SAFE_DELETE(t_ManagePoFeExcludeUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoFeExclude::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFeExclude::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FE_EXCLUDE pdpfe = NULL;
	DB_PO_FE_EXCLUDE dpfe;

	m_tDPH = &(dpfe.tDPH);

	TListDBPoFeExcludePkg	tPkgList;
	TListDBPoFeExcludeUnit	tUnitList;

	if( t_ManagePoFeExclude->GetPkt(RecvToken, dpfe))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_FE_EXCLUDE_PKG	dppp;		
		DB_PO_FE_EXCLUDE_UNIT dppu;

		if( t_ManagePoFeExcludePkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoFeExcludeUnit->GetPkt(RecvToken, dppu))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpfe.tDPH._add_id(dppp.tDPH.nID);
	}

	pdpfe = (PDB_PO_FE_EXCLUDE)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdpfe)
	{
		t_ManagePoFeExcludePkg->ClearItemByPolicyID(pdpfe->tDPH.nID);		
		t_ManagePoFeExclude->DelPoFeExclude(pdpfe->tDPH.nID);
	}

	{
		{
			TListDBPoFeExcludeUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFeExcludeUnit->ApplyPoFeExcludeUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoFeExcludePkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFeExcludePkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoFeExcludePkg->AddPoFeExcludePkg(*begin);
			}
		}

		if(SetER(t_ManagePoFeExclude->ApplyPoFeExclude(dpfe)))
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
