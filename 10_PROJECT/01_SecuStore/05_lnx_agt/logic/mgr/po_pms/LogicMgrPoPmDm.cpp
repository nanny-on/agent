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
#include "LogicMgrPoPmDm.h"

//---------------------------------------------------------------------------

CLogicMgrPoPmDm*		t_LogicMgrPoPmDm = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoPmDm::CLogicMgrPoPmDm()
{
	t_ManagePoPmDm		= new CManagePoPmDm();
	t_ManagePoPmDmPkg	= new CManagePoPmDmPkg();
	t_ManagePoPmDmUnit	= new CManagePoPmDmUnit();

	t_ManagePoPmDm->LoadDBMS();
	t_ManagePoPmDmPkg->LoadDBMS();
	t_ManagePoPmDmUnit->LoadDBMS();

	t_ManagePoPmDm->InitHash();

	m_strLogicName		= "mgr agt po pm dm";
	
	m_nPolicyType		= SS_POLICY_TYPE_PM_DM;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_PM_DM;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_PM_DM;
}
//---------------------------------------------------------------------------

CLogicMgrPoPmDm::~CLogicMgrPoPmDm()
{
	SAFE_DELETE(t_ManagePoPmDm);
	SAFE_DELETE(t_ManagePoPmDmPkg);
	SAFE_DELETE(t_ManagePoPmDmUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPmDm::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmDm::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_PM_DM pdata = NULL;
	DB_PO_PM_DM data;

	m_tDPH = &(data.tDPH);

	TListDBPoPmDmPkg	tPkgList;
	TListDBPoPmDmUnit	tUnitList;

	if( t_ManagePoPmDm->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_PM_DM_PKG		data_pkg;		
		DB_PO_PM_DM_UNIT	data_unit;

		if( t_ManagePoPmDmPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoPmDmUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_PM_DM)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoPmDmPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoPmDm->DelPoPmDm(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoPmDmUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPmDmUnit->ApplyPoPmDmUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoPmDmPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPmDmPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoPmDmPkg->AddPoPmDmPkg(*begin);
			}
		}

		if(SetER(t_ManagePoPmDm->ApplyPoPmDm(data)))
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

INT32		CLogicMgrPoPmDm::OnTimer_Logic()
{
	if(t_ManageEnvLicense->IsInvalidRight(m_nPackageType, m_nPolicyType, m_nControlType))
	{
		return 0;
	}
	
	PDB_PO_PM_DM pdb_po = (PDB_PO_PM_DM)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
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
		t_ManagePoPmDmPkg->GetKeyIDList(pdb_po->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			ApplyPolicy_Unit(*begin);
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmDm::ApplyPolicy_Unit(UINT32 nUnitID)
{
	PDB_PO_PM_DM_UNIT pdata = t_ManagePoPmDmUnit->FindItem(nUnitID);
	if(!pdata || pdata->tDPH.nUsedMode != STATUS_USED_MODE_ON)	return -1;

	return ApplyPolicy_Unit(pdata);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmDm::ApplyPolicy_Unit(PDB_PO_PM_DM_UNIT pdata)
{
	
	return 0;
}
//---------------------------------------------------------------------------