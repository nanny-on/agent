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
#include "LogicMgrPoInVulnQna.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnQna*		t_LogicMgrPoInVulnQna = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnQna::CLogicMgrPoInVulnQna()
{
	t_ManagePoInVulnQna		= new CManagePoInVulnQna();
	t_ManagePoInVulnQnaPkg		= new CManagePoInVulnQnaPkg();
	t_ManagePoInVulnQnaUnit	= new CManagePoInVulnQnaUnit();

	t_ManagePoInVulnQna->LoadDBMS();
	t_ManagePoInVulnQnaPkg->LoadDBMS();
	t_ManagePoInVulnQnaUnit->LoadDBMS();

	t_ManagePoInVulnQna->InitHash();

	m_strLogicName		= "mgr agt po in vuln qna";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_QNA;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_QNA;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_QNA;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnQna::~CLogicMgrPoInVulnQna()
{
	SAFE_DELETE(t_ManagePoInVulnQna);
	SAFE_DELETE(t_ManagePoInVulnQnaPkg);
	SAFE_DELETE(t_ManagePoInVulnQnaUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnQna::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnQna::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_QNA pdata = NULL;
	DB_PO_IN_VULN_QNA data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnQnaPkg	tPkgList;
	TListDBPoInVulnQnaUnit	tUnitList;

	if( t_ManagePoInVulnQna->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_QNA_PKG		data_pkg;		
		DB_PO_IN_VULN_QNA_UNIT	data_unit;

		if( t_ManagePoInVulnQnaPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInVulnQnaUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_QNA)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnQnaPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnQna->DelPoInVulnQna(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnQnaUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnQnaUnit->ApplyPoInVulnQnaUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnQnaPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnQnaPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnQnaPkg->AddPoInVulnQnaPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnQna->ApplyPoInVulnQna(data)))
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
