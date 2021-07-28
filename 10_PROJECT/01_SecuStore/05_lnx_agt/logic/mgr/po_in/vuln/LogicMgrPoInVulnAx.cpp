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
#include "LogicMgrPoInVulnAx.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnAx*		t_LogicMgrPoInVulnAx = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnAx::CLogicMgrPoInVulnAx()
{
	t_ManagePoInVulnAx		= new CManagePoInVulnAx();
	t_ManagePoInVulnAxPkg		= new CManagePoInVulnAxPkg();
	t_ManagePoInVulnAxUnit	= new CManagePoInVulnAxUnit();

	t_ManagePoInVulnAx->LoadDBMS();
	t_ManagePoInVulnAxPkg->LoadDBMS();
	t_ManagePoInVulnAxUnit->LoadDBMS();

	t_ManagePoInVulnAx->InitHash();

	m_strLogicName		= "mgr agt po in vuln ax";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_AX;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_AX;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_AX;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnAx::~CLogicMgrPoInVulnAx()
{
	SAFE_DELETE(t_ManagePoInVulnAx);
	SAFE_DELETE(t_ManagePoInVulnAxPkg);
	SAFE_DELETE(t_ManagePoInVulnAxUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnAx::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnAx::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_AX pdata = NULL;
	DB_PO_IN_VULN_AX data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnAxPkg	tPkgList;
	TListDBPoInVulnAxUnit	tUnitList;

	if( t_ManagePoInVulnAx->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_AX_PKG		data_pkg;		
		DB_PO_IN_VULN_AX_UNIT	data_unit;

		if( t_ManagePoInVulnAxPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInVulnAxUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_AX)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnAxPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnAx->DelPoInVulnAx(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnAxUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnAxUnit->ApplyPoInVulnAxUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnAxPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnAxPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnAxPkg->AddPoInVulnAxPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnAx->ApplyPoInVulnAx(data)))
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
