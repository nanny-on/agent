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
#include "LogicMgrHostVulnRst.h"

//---------------------------------------------------------------------------

CLogicMgrHostVulnRst*		t_LogicMgrHostVulnRst = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostVulnRst::CLogicMgrHostVulnRst()
{
	t_ManageHostVulnRst		= new CManageHostVulnRst();
	t_ManageHostVulnRstUnit	= new CManageHostVulnRstUnit();

	t_LogicHostVulnRst		= new CLogicHostVulnRst();

	t_ManageHostVulnRst->LoadDBMS();
	t_ManageHostVulnRstUnit->LoadDBMS();

	m_strLogicName = "logic mgr host vuln rst";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= EVENT_OBJECT_CODE_HOST_VULN_RST;
}
//---------------------------------------------------------------------------

CLogicMgrHostVulnRst::~CLogicMgrHostVulnRst()
{
	SAFE_DELETE(t_ManageHostVulnRst);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostVulnRst::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
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

INT32		CLogicMgrHostVulnRst::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_VULN_RST pdata = NULL;
	TListDBHostVulnRst tDBHostVulnRstList;
	DB_HOST_VULN_RST data;

	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		if( t_ManageHostVulnRst->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBHostVulnRstList.push_back(data);
	}

	
	{
		TListDBHostVulnRstItor begin, end;
		begin = tDBHostVulnRstList.begin();	end = tDBHostVulnRstList.end();
		for(begin; begin != end; begin++)
		{
			if( (pdata = t_ManageHostVulnRst->FindKeyItem(begin->nScanMon)) == NULL)
			{
				t_ManageHostVulnRst->AddHostVulnRst(*begin);
			}
			else
			{
				t_ManageHostVulnRst->EditHostVulnRst(*begin);
			}

			m_tSendIDList.push_back(begin->nID);
		}
	}

	{
		TListIDItor begin, end;
		begin = m_tSendIDList.begin();	end = m_tSendIDList.end();
		for(begin; begin != end; begin++)
		{
			SendToken.TokenAdd_32(ERR_SUCCESS);
			t_ManageHostVulnRst->SetPkt_Link(*begin, SendToken);
			SendData_Link(G_TYPE_HOST_VULN_RST, G_CODE_COMMON_SYNC, SendToken);
			SendToken.Clear();
		}
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrHostVulnRst::SetHostVulnRst(DB_HOST_VULN_RST& data)
{
	if(!ISSYNCSTEP(data.nSyncSvrStep)) 
	{
		{
			WriteLogN("[%s] remain evt log to file : id[%d]:status[%d]", 
				m_strLogicName.c_str(),
				data.nScanMon, data.nScanDate);

		}
	}

	{	
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageHostVulnRst->SetPkt(&data, SendToken);
		{
			SendData_Mgr(G_TYPE_HOST_VULN_RST, G_CODE_COMMON_EDIT, SendToken);
		}
		SendToken.Clear();

		{
			SendToken.TokenAdd_32(ERR_SUCCESS);
			t_ManageHostVulnRst->SetPkt_Link(data.nID, SendToken);
			SendData_Link(G_TYPE_HOST_VULN_RST, G_CODE_COMMON_SYNC, SendToken);
			SendToken.Clear();
		}
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------