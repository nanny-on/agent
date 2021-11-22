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
#include "LogicMgrPoInPtnOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoInPtnOp*		t_LogicMgrPoInPtnOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInPtnOp::CLogicMgrPoInPtnOp()
{
	t_ManagePoInPtnOp		= new CManagePoInPtnOp();
	t_ManagePoInPtnOpPkg	= new CManagePoInPtnOpPkg();
	t_ManagePoInPtnOpUnit	= new CManagePoInPtnOpUnit();


	t_ManagePoInPtnOp->LoadDBMS();
	t_ManagePoInPtnOpPkg->LoadDBMS();
	t_ManagePoInPtnOpUnit->LoadDBMS();

	t_ManagePoInPtnOp->InitHash();

	m_strLogicName		= "mgr agt po in ptn op";

	m_nPolicyType		= SS_POLICY_TYPE_IN_PTN_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_PTN_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;

	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_PTN_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoInPtnOp::~CLogicMgrPoInPtnOp()
{
	SAFE_DELETE(t_ManagePoInPtnOpUnit);
	SAFE_DELETE(t_ManagePoInPtnOpPkg);
	SAFE_DELETE(t_ManagePoInPtnOp);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CLogicMgrPoInPtnOp::AnalyzePkt_FromMgr_Ext()
{
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SCAN:	AnalyzePkt_FromMgr_Ext_Scan();		break;
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_PTN_OP pdata = NULL;
	DB_PO_IN_PTN_OP data;

	m_tDPH = &(data.tDPH);

	TListDBPoInPtnOpPkg		tPkgList;
	TListDBPoInPtnOpUnit	tUnitList;

	if( t_ManagePoInPtnOp->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_PTN_OP_PKG	data_pkg;		
		DB_PO_IN_PTN_OP_UNIT data_unit;

		if( t_ManagePoInPtnOpPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInPtnOpUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	{
		TListDBPoInPtnOpUnitItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_PTN_OP_UNIT pdata_unit = t_ManagePoInPtnOpUnit->FindItem(begin->tDPH.nID);
			if(!pdata_unit)	
			{
				begin->nLastScanTime = GetCurrentDateTimeInt();
				continue;
			}

			begin->nLastScanTime = pdata_unit->nLastScanTime;
		}
	}

	pdata = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{		
		t_ManagePoInPtnOpPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInPtnOp->DelPoInPtnOp(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInPtnOpUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnOpUnit->ApplyPoInPtnOpUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInPtnOpPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnOpPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInPtnOpPkg->AddPoInPtnOpPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInPtnOp->ApplyPoInPtnOp(data)))
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

INT32		CLogicMgrPoInPtnOp::AnalyzePkt_FromMgr_Ext_Scan()
{
	PDB_PO_IN_PTN_OP pdb_po = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_po)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}	

	{
		WriteLogN("start(check) in ptn op demand : [uid:%d]", pdb_po->tDPH.nID);		
		{
			DB_PO_IN_PTN_OP_UNIT data;
			{
				{
					{
						ASIDM_INIT tADI;
						t_EnvInfoOp->GetDMInitData(tADI);
					}
					data.strScanPath = "/";	
				}
				data.tDPH.strName = "admin demand control";
				data.nFileGatherType = SS_PO_IN_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
				
// 				tMFOI.nID		= t_ThreadPoInPtnScan->GetNextOrderID();
// 
// 				tMFOI.nPoType	= m_nPolicyType;
// 				tMFOI.nPoID		= pdb_po->tDPH.nID;
// 				tMFOI.nOpType	= G_CODE_COMMON_SCAN;
			}
			t_ThreadPoInPtnScan->AddWork(&data);
		}
	}
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnOp::OnTimer_Logic()
{
	PDB_PO_IN_PTN_OP pdata_po = (PDB_PO_IN_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata_po)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	if(pdata_po->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return 0;
	}

	{
		UINT32 nRet = -1;

		TListID tIDList;
		t_ManagePoInPtnOpPkg->GetKeyIDList(pdata_po->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_PTN_OP_UNIT pdata_unit = t_ManagePoInPtnOpUnit->FindItem(*begin);
			if(!pdata_unit || pdata_unit->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		continue;
						
			UINT32 nLastChkTime = pdata_unit->nLastScanTime;
			U64_SCHEDULE tIS;
			tIS.all = pdata_unit->nSchTime;
			if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
			{
				continue;
			}
			
			t_ThreadPoInPtnScan->AddWork(pdata_unit);

			pdata_unit->nLastScanTime = nLastChkTime;
			if(SetER(t_ManagePoInPtnOpUnit->EditPoInPtnOpUnit(*pdata_unit)))
			{
				WriteLogE("[%s] edit po in ptn op unit fail : [%d]", m_strLogicName.c_str(), pdata_unit->tDPH.nID);
				return -1;
			}
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
