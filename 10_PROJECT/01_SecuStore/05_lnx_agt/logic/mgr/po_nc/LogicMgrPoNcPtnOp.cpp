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
#include "LogicMgrPoNcPtnOp.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnOp*		t_LogicMgrPoNcPtnOp = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnOp::CLogicMgrPoNcPtnOp()
{
	t_ManagePoNcPtnOp		= new CManagePoNcPtnOp();
	t_ManagePoNcPtnOpPkg	= new CManagePoNcPtnOpPkg();
	t_ManagePoNcPtnOpUnit	= new CManagePoNcPtnOpUnit();


	t_ManagePoNcPtnOp->LoadDBMS();
	t_ManagePoNcPtnOpPkg->LoadDBMS();
	t_ManagePoNcPtnOpUnit->LoadDBMS();

	t_ManagePoNcPtnOp->InitHash();

	m_strLogicName		= "mgr agt po nc ptn op";

	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_OP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_NC_PTN_OP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;

	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_NC_PTN_OP;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnOp::~CLogicMgrPoNcPtnOp()
{
	SAFE_DELETE(t_ManagePoNcPtnOpUnit);
	SAFE_DELETE(t_ManagePoNcPtnOpPkg);
	SAFE_DELETE(t_ManagePoNcPtnOp);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CLogicMgrPoNcPtnOp::AnalyzePkt_FromMgr_Ext()
{
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SCAN:	AnalyzePkt_FromMgr_Ext_Scan();		break;
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnOp::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_NC_PTN_OP pdata = NULL;
	DB_PO_NC_PTN_OP data;

	m_tDPH = &(data.tDPH);

	TListDBPoNcPtnOpPkg		tPkgList;
	TListDBPoNcPtnOpUnit	tUnitList;

	if( t_ManagePoNcPtnOp->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_NC_PTN_OP_PKG	data_pkg;		
		DB_PO_NC_PTN_OP_UNIT data_unit;

		if( t_ManagePoNcPtnOpPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoNcPtnOpUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	{
		TListDBPoNcPtnOpUnitItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_NC_PTN_OP_UNIT pdata_unit = t_ManagePoNcPtnOpUnit->FindItem(begin->tDPH.nID);
			if(!pdata_unit)	
			{
				begin->nLastScanTime = GetCurrentDateTimeInt();
				continue;
			}

			begin->nLastScanTime = pdata_unit->nLastScanTime;
		}
	}

	pdata = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{		
		t_ManagePoNcPtnOpPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoNcPtnOp->DelPoNcPtnOp(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoNcPtnOpUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnOpUnit->ApplyPoNcPtnOpUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoNcPtnOpPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnOpPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoNcPtnOpPkg->AddPoNcPtnOpPkg(*begin);
			}
		}

		if(SetER(t_ManagePoNcPtnOp->ApplyPoNcPtnOp(data)))
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

INT32		CLogicMgrPoNcPtnOp::AnalyzePkt_FromMgr_Ext_Scan()
{
	PDB_PO_NC_PTN_OP pdb_po = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_po)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}	

	{
		WriteLogN("start(check) nc ptn op demand : [uid:%d]", pdb_po->tDPH.nID);		
		{
			DB_PO_NC_PTN_OP_UNIT data;
			{
/*
				{
					ASIDM_INIT tADI;
					t_EnvInfoOp->GetDMInitData(tADI);
					t_ASIDMDLLUtil->Init(&tADI);
				}
				data.strScanPath = t_ASIDMDLLUtil->GetFixDrvList(m_strLogicName.c_str());	
				t_ASIDMDLLUtil->Free();				
				data.tDPH.strName = "admin demand control";
				data.nFileGatherType = SS_PO_NC_PTN_OP_NEW_FILE_SEND_TYPE_INFO;
*/				
// 				tMFOI.nID		= t_ThreadPoNcPtnScan->GetNextOrderID();
// 
// 				tMFOI.nPoType	= m_nPolicyType;
// 				tMFOI.nPoID		= pdb_po->tDPH.nID;
// 				tMFOI.nOpType	= G_CODE_COMMON_SCAN;
			}
//			t_ThreadPoNcPtnScan->AddWork(&data);
		}
	}
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnOp::OnTimer_Logic()
{
	PDB_PO_NC_PTN_OP pdata_po = (PDB_PO_NC_PTN_OP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata_po)	
	{
		WriteLogE("[%s] not find current policy", m_strLogicName.c_str());
		return 0;
	}

	if(pdata_po->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return 0;
	}

	{
		UINT32 nRet = -1;

		TListID tIDList;
		t_ManagePoNcPtnOpPkg->GetKeyIDList(pdata_po->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_NC_PTN_OP_UNIT pdata_unit = t_ManagePoNcPtnOpUnit->FindItem(*begin);
			if(!pdata_unit || pdata_unit->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		continue;
						
			UINT32 nLastChkTime = pdata_unit->nLastScanTime;
			U64_SCHEDULE tIS;
			tIS.all = pdata_unit->nSchTime;
			if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
			{
				continue;
			}
			
			if(t_ThreadPoNcPtnScan)
				t_ThreadPoNcPtnScan->AddWork(pdata_unit);

			pdata_unit->nLastScanTime = nLastChkTime;
			if(SetER(t_ManagePoNcPtnOpUnit->EditPoNcPtnOpUnit(*pdata_unit)))
			{
				WriteLogE("[%s] edit po nc ptn op unit fail : [%d]", m_strLogicName.c_str(), pdata_unit->tDPH.nID);
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
