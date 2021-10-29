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
#include "LogicMgrPoInVulnScan.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnScan*		t_LogicMgrPoInVulnScan = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnScan::CLogicMgrPoInVulnScan()
{
	t_ManagePoInVulnScan		= new CManagePoInVulnScan();
	t_ManagePoInVulnScanPkg		= new CManagePoInVulnScanPkg();
	t_ManagePoInVulnScanUnit	= new CManagePoInVulnScanUnit();
	t_ManagePoInVulnScanUnitPkg	= new CManagePoInVulnScanUnitPkg();

	t_ManagePoInVulnScan->LoadDBMS();
	t_ManagePoInVulnScanPkg->LoadDBMS();
	t_ManagePoInVulnScanUnit->LoadDBMS();
	t_ManagePoInVulnScanUnitPkg->LoadDBMS();

	t_ManagePoInVulnScan->InitHash();

	m_strLogicName		= "mgr agt po in vuln scan";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_SCAN;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_SCAN;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_SCAN;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnScan::~CLogicMgrPoInVulnScan()
{
	SAFE_DELETE(t_ManagePoInVulnScan);
	SAFE_DELETE(t_ManagePoInVulnScanPkg);
	SAFE_DELETE(t_ManagePoInVulnScanUnit);
	SAFE_DELETE(t_ManagePoInVulnScanUnitPkg);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnScan::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnScan::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_SCAN pdata = NULL;
	DB_PO_IN_VULN_SCAN data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnScanPkg	tPkgList;
	TListDBPoInVulnScanUnit	tUnitList;

	if( t_ManagePoInVulnScan->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_SCAN_PKG		data_pkg;		
		DB_PO_IN_VULN_SCAN_UNIT	data_unit;

		if( t_ManagePoInVulnScanPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInVulnScanUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_SCAN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnScanPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnScan->DelPoInVulnScan(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnScanUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnScanUnit->ApplyPoInVulnScanUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnScanPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnScanPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnScanPkg->AddPoInVulnScanPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnScan->ApplyPoInVulnScan(data)))
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

INT32		CLogicMgrPoInVulnScan::OnTimer_Logic()
{	
	PDB_PO_IN_VULN_OP pdata_op = (PDB_PO_IN_VULN_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_OP);
	if(!pdata_op)		return 0;	
	if(pdata_op->tDPH.nUsedMode == STATUS_USED_MODE_OFF)	return 0;
	
	PDB_PO_IN_VULN_SCAN pdata_po = (PDB_PO_IN_VULN_SCAN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
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
		t_ManagePoInVulnScanPkg->GetKeyIDList(pdata_po->tDPH.nID, tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_VULN_SCAN_UNIT pdata_unit = t_ManagePoInVulnScanUnit->FindItem(*begin);
			if(!pdata_unit || pdata_unit->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		continue;

			UINT32 nLastChkTime = pdata_unit->nLastScanTime;
			U64_SCHEDULE tIS;
			tIS.all = pdata_unit->nSchTime;
			if(IsValidSchedule(tIS.all, nLastChkTime) == 0)
			{
				continue;
			}			

			SendData_Link_PoInVulnScan(pdata_unit->nScanType, pdata_unit->tDPH.nExtOption);		

			pdata_unit->nLastScanTime = nLastChkTime;
			if(SetER(t_ManagePoInVulnScanUnit->EditPoInVulnScanUnit(*pdata_unit)))
			{
				WriteLogE("[%s] edit po in vuln scan unit fail : [%d]", m_strLogicName.c_str(), pdata_unit->tDPH.nID);
				return -1;
			}
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------

INT32    CLogicMgrPoInVulnScan::SendData_Link_PoInVulnScan(UINT32 nType, UINT32 nExtOpt)
{		
	WriteLogN("[%s] send link po in vuln scan unit type : [%d]", m_strLogicName.c_str(), nType);
	m_tMutex.Lock();
	SendToken.TokenAdd_32(nType);
	SendToken.TokenAdd_32(nExtOpt);
	SendData_Link(0, G_TYPE_PO_IN_VULN_SCAN, G_CODE_COMMON_SCAN, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
