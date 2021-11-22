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
#include "LogicMgrPoFaProc.h"

//---------------------------------------------------------------------------

CLogicMgrPoFaProc*		t_LogicMgrPoFaProc = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoFaProc::CLogicMgrPoFaProc()
{
	t_ManagePoFaProc		= new CManagePoFaProc();
	t_ManagePoFaProcPkg		= new CManagePoFaProcPkg();
	t_ManagePoFaProcUnit	= new CManagePoFaProcUnit();

	t_ManagePoFaProc->LoadDBMS();
	t_ManagePoFaProcPkg->LoadDBMS();
	t_ManagePoFaProcUnit->LoadDBMS();

	t_ManagePoFaProc->InitHash();

	m_strLogicName		= "mgr agt po fa proc";
	
	m_nPolicyType		= SS_POLICY_TYPE_FA_PROC;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_FA_PROC;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_FA_PROC;
}
//---------------------------------------------------------------------------

CLogicMgrPoFaProc::~CLogicMgrPoFaProc()
{
	SAFE_DELETE(t_ManagePoFaProc);
	SAFE_DELETE(t_ManagePoFaProcPkg);
	SAFE_DELETE(t_ManagePoFaProcUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoFaProc::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoFaProc::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_FA_PROC ppcp = NULL;
	DB_PO_FA_PROC dpfp;

	m_tDPH = &(dpfp.tDPH);

	TListDBPoFaProcPkg	tPkgList;
	TListDBPoFaProcUnit	tUnitList;

	if( t_ManagePoFaProc->GetPkt(RecvToken, dpfp))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_FA_PROC_PKG	dppp;		
		DB_PO_FA_PROC_UNIT dppu;

		if( t_ManagePoFaProcPkg->GetPkt(RecvToken, dppp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoFaProcUnit->GetPkt(RecvToken, dppu))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dppp);
		tUnitList.push_back(dppu);

		dpfp.tDPH._add_id(dppp.tDPH.nID);
	}

	ppcp = (PDB_PO_FA_PROC)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(ppcp)
	{
		t_ManagePoFaProcPkg->ClearItemByPolicyID(ppcp->tDPH.nID);		
		t_ManagePoFaProc->DelPoFaProc(ppcp->tDPH.nID);
	}

	{
		{
			TListDBPoFaProcUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFaProcUnit->ApplyPoFaProcUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoFaProcPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoFaProcPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoFaProcPkg->AddPoFaProcPkg(*begin);
			}
		}

		if(SetER(t_ManagePoFaProc->ApplyPoFaProc(dpfp)))
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
