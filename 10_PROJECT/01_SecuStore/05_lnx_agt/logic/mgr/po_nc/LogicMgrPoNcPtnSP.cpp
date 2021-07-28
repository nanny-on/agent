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
#include "LogicMgrPoNcPtnSP.h"

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSP*		t_LogicMgrPoNcPtnSP = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSP::CLogicMgrPoNcPtnSP()
{
	t_ManagePoNcPtnSP		= new CManagePoNcPtnSP();
	t_ManagePoNcPtnSPPkg	= new CManagePoNcPtnSPPkg();
	t_ManagePoNcPtnSPUnit	= new CManagePoNcPtnSPUnit();

	t_ManagePoNcPtnSP->LoadDBMS();
	t_ManagePoNcPtnSPPkg->LoadDBMS();
	t_ManagePoNcPtnSPUnit->LoadDBMS();

	t_ManagePoNcPtnSP->InitHash();

	m_strLogicName		= "mgr agt po nc ptn sp";
	
	m_nPolicyType		= SS_POLICY_TYPE_NC_PTN_SP;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_NC_PTN_SP;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_NC_PTN_SP;
}
//---------------------------------------------------------------------------

CLogicMgrPoNcPtnSP::~CLogicMgrPoNcPtnSP()
{
	SAFE_DELETE(t_ManagePoNcPtnSP);
	SAFE_DELETE(t_ManagePoNcPtnSPPkg);
	SAFE_DELETE(t_ManagePoNcPtnSPUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoNcPtnSP::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoNcPtnSP::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_NC_PTN_SP pdata = NULL;
	DB_PO_NC_PTN_SP data;

	m_tDPH = &(data.tDPH);

	TListDBPoNcPtnSPPkg	tPkgList;
	TListDBPoNcPtnSPUnit	tUnitList;

	if( t_ManagePoNcPtnSP->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_NC_PTN_SP_PKG		data_pkg;		
		DB_PO_NC_PTN_SP_UNIT	data_unit;

		if( t_ManagePoNcPtnSPPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoNcPtnSPUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_NC_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoNcPtnSPPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoNcPtnSP->DelPoNcPtnSP(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoNcPtnSPUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnSPUnit->ApplyPoNcPtnSPUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoNcPtnSPPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoNcPtnSPPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoNcPtnSPPkg->AddPoNcPtnSPPkg(*begin);
			}
		}

		if(SetER(t_ManagePoNcPtnSP->ApplyPoNcPtnSP(data)))
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
