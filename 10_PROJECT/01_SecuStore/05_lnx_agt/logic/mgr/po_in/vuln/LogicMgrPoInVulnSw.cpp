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
#include "LogicMgrPoInVulnSw.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnSw*		t_LogicMgrPoInVulnSw = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnSw::CLogicMgrPoInVulnSw()
{
	t_ManagePoInVulnSw		= new CManagePoInVulnSw();
	t_ManagePoInVulnSwPkg		= new CManagePoInVulnSwPkg();
	t_ManagePoInVulnSwUnit	= new CManagePoInVulnSwUnit();

	t_ManagePoInVulnSw->LoadDBMS();
	t_ManagePoInVulnSwPkg->LoadDBMS();
	t_ManagePoInVulnSwUnit->LoadDBMS();

	t_ManagePoInVulnSw->InitHash();

	m_strLogicName		= "mgr agt po in vuln sw";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_SW;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_SW;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_SW;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnSw::~CLogicMgrPoInVulnSw()
{
	SAFE_DELETE(t_ManagePoInVulnSw);
	SAFE_DELETE(t_ManagePoInVulnSwPkg);
	SAFE_DELETE(t_ManagePoInVulnSwUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnSw::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnSw::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_SW pdata = NULL;
	DB_PO_IN_VULN_SW data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnSwPkg	tPkgList;
	TListDBPoInVulnSwUnit	tUnitList;

	if( t_ManagePoInVulnSw->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_SW_PKG		data_pkg;		
		DB_PO_IN_VULN_SW_UNIT	data_unit;

		if( t_ManagePoInVulnSwPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		tPkgList.push_back(data_pkg);

		if(LOTYPEID32(data_pkg.tDPH.nUnitID) == SS_COMMON_UNIT_TYPE_SITE)
		{
			if( t_ManagePoInVulnSwUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
			tUnitList.push_back(data_unit);
		}
				
		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_SW)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnSwPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnSw->DelPoInVulnSw(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnSwUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnSwUnit->ApplyPoInVulnSwUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnSwPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnSwPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnSwPkg->AddPoInVulnSwPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnSw->ApplyPoInVulnSw(data)))
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
