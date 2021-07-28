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
#include "LogicMgrPoPmScan.h"

//---------------------------------------------------------------------------

CLogicMgrPoPmScan*		t_LogicMgrPoPmScan = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoPmScan::CLogicMgrPoPmScan()
{
	t_ManagePoPmScan		= new CManagePoPmScan();
	t_ManagePoPmScanPkg		= new CManagePoPmScanPkg();
	t_ManagePoPmScanUnit	= new CManagePoPmScanUnit();

	t_ManagePoPmScan->LoadDBMS();
	t_ManagePoPmScanPkg->LoadDBMS();
	t_ManagePoPmScanUnit->LoadDBMS();

	t_ManagePoPmScan->InitHash();

	m_strLogicName		= "mgr agt po pm scan";
	
	m_nPolicyType		= SS_POLICY_TYPE_PM_SCAN;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_PM_SCAN;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_PM_SCAN;
}
//---------------------------------------------------------------------------

CLogicMgrPoPmScan::~CLogicMgrPoPmScan()
{
	SAFE_DELETE(t_ManagePoPmScan);
	SAFE_DELETE(t_ManagePoPmScanPkg);
	SAFE_DELETE(t_ManagePoPmScanUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPmScan::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_PM_SCAN pdata = NULL;
	DB_PO_PM_SCAN data;

	m_tDPH = &(data.tDPH);

	TListDBPoPmScanPkg	tPkgList;
	TListDBPoPmScanUnit	tUnitList;

	if( t_ManagePoPmScan->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_PM_SCAN_PKG		data_pkg;		
		DB_PO_PM_SCAN_UNIT	data_unit;

		if( t_ManagePoPmScanPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoPmScanUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_PM_SCAN)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoPmScanPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoPmScan->DelPoPmScan(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoPmScanUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPmScanUnit->ApplyPoPmScanUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoPmScanPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoPmScanPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoPmScanPkg->AddPoPmScanPkg(*begin);
			}
		}

		if(SetER(t_ManagePoPmScan->ApplyPoPmScan(data)))
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

INT32		CLogicMgrPoPmScan::StartScanDefault()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::ApplyPolicy_Unit(UINT32 nUnitID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::ApplyPolicy_Unit(PDB_PO_PM_SCAN_UNIT pdata)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::StartScanUnit(PDB_PO_PM_SCAN_UNIT pdata)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::IsUsedOffLineScan()
{

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmScan::SendPkt_NewPatch(UINT32 nWorkID)
{
	return 0;
}
//---------------------------------------------------------------------------
