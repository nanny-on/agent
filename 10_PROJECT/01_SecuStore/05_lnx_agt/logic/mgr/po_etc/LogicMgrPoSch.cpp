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
#include "LogicMgrPoSch.h"

//---------------------------------------------------------------------------

CLogicMgrPoSch*		t_LogicMgrPoSch = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoSch::CLogicMgrPoSch()
{
	t_ManagePoSch			= new CManagePoSch();
	t_ManagePoSchPkg		= new CManagePoSchPkg();
	t_ManagePoSchUnit		= new CManagePoSchUnit();

	t_ManagePoSch->LoadDBMS();
	t_ManagePoSchPkg->LoadDBMS();
	t_ManagePoSchUnit->LoadDBMS();

	t_ManagePoSch->InitHash();

	m_strLogicName		= "logic po schdule";

	m_nPackageType		= SS_PACKAGE_TYPE_SAMS;
	m_nPolicyType		= SS_POLICY_TYPE_SCH;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_SCH;
}
//---------------------------------------------------------------------------

CLogicMgrPoSch::~CLogicMgrPoSch()
{
	SAFE_DELETE(t_ManagePoSchUnit);
	SAFE_DELETE(t_ManagePoSchPkg);
	SAFE_DELETE(t_ManagePoSch);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_SCH pdphps = NULL;
	DB_PO_SCH dphps;

	m_tDPH = &(dphps.tDPH);

	TListDBPoSchPkg	tPkgList;
	TListDBPoSchUnit	tUnitList;

	if( t_ManagePoSch->GetPkt(RecvToken, dphps))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_SCH_PKG	dphpsp;		
		DB_PO_SCH_UNIT dphpsu;

		if( t_ManagePoSchPkg->GetPkt(RecvToken, dphpsp))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoSchUnit->GetPkt(RecvToken, dphpsu))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(dphpsp);
		tUnitList.push_back(dphpsu);

		dphps.tDPH._add_id(dphpsp.tDPH.nID);
	}

	{
		pdphps = (PDB_PO_SCH)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
		if(pdphps)
		{
			t_ManagePoSchPkg->ClearItemByPolicyID(pdphps->tDPH.nID);		
			t_ManagePoSch->DelPoSch(pdphps->tDPH.nID);
		}

		{
			{
				TListDBPoSchUnitItor begin, end;
				begin = tUnitList.begin();	end = tUnitList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoSchUnit->ApplyPoSchUnit(*begin))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}
				}
			}

			{
				TListDBPoSchPkgItor begin, end;
				begin = tPkgList.begin();	end = tPkgList.end();
				for(begin; begin != end; begin++)
				{
					if(t_ManagePoSchPkg->FindItem(begin->tDPH.nID))
					{
						SetDLEH_EC(g_nErrRtn);
						WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
						continue;
					}

					t_ManagePoSchPkg->AddPoSchPkg(*begin);
				}
			}

			if(SetER(t_ManagePoSch->ApplyPoSch(dphps)))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
			}
		}
	}

	{
		{
			WriteLogN("[%s] sync policy start by edit po sch", m_strLogicName.c_str());
			t_LogicMgrAuth->SendMgrInitData_Polcy();
			t_LogicMgrAuth->SendMgrInitData_Sync();
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

INT32		CLogicMgrPoSch::OnTimer_Logic()
{
	PDB_PO_SCH pdb_po = (PDB_PO_SCH)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdb_po)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	if(pdb_po->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{
		return 0;
	}

	{
		UINT32 nRet = -1;

		TListID tIDList;
		t_ManagePoSchPkg->GetKeyIDList(pdb_po->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			nRet = ApplyPolicy_Unit(*begin);

			if(!nRet)
				ApplyPolicy_SendLastChkTime(*begin);
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::ApplyPolicy_Unit(UINT32 nUnitID)
{
	PDB_PO_SCH_UNIT pdphpsu = t_ManagePoSchUnit->FindItem(nUnitID);
	if(!pdphpsu || pdphpsu->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
	{ 
		return -1;
	}

	return ApplyPolicy_Unit(pdphpsu);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::ApplyPolicy_Unit(PDB_PO_SCH_UNIT pdphpsu)
{
//	CSystemInfo tSysInfo;
//	CProcUtil tProcUtil;

	//if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
	//{
		//return -1;
	//}

	//if(IsValidPoSchdule())
		//return -1;

	PDB_HOST_POLICY pdhp = t_ManageHostPolicy->FindItem(DEFAULT_ID);
	if(!pdhp)
		return -1;

	/*switch(pdphpsu->nPolicyType)
	{
	case SS_POLICY_TYPE_FA_CLEAR:	pdhp->tDP.nPFaClearID = pdphpsu->nNextPolID;
	}*/

	//t_ManageSchHostPolicy->EditHostPolicy()
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::ApplyPolicy_SendLastChkTime(INT32 nUnitID)
{	
	//SendToken.Clear();

	//SendToken.TokenAdd_32(nUnitID);
	////SendToken.TokenAdd_32(t_ManagePoSchUnit->GetLastChkTime(nUnitID));

	//SendData_Link(G_TYPE_PO_HOST_PO_SCH_UNIT, G_CODE_COMMON_EDIT, SendToken);
	//SendToken.Clear();
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoSch::IsValidPoSchdule(UINT32 nStTime, UINT32 nEdTime)
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



