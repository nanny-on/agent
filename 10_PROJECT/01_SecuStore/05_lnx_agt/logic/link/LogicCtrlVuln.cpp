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
#include "LogicCtrlVuln.h"

//---------------------------------------------------------------------------

CLogicCtrlVuln*		t_LogicCtrlVuln = NULL;

//---------------------------------------------------------------------------

CLogicCtrlVuln::CLogicCtrlVuln()
{	
	m_strLogicName = "logic ctrl vuln";
}
//---------------------------------------------------------------------------

CLogicCtrlVuln::~CLogicCtrlVuln()
{
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SCAN:		AnalyzePkt_FromLink_Ext_Scan();		break;
		default:						break;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::AnalyzePkt_FromLink_Ext_Scan()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_SCAN;
	UINT32 nRst = SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
	
	if(!RecvToken.TokenDel_32(m_nRecvID))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanPtnVuln(PDB_PTN_VULN pdata)
{
	return SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanPtnVulnCustom(PDB_PTN_VULN_SCAN pdata)
{
	return SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
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

void		CLogicCtrlVuln::SendPkt_Scan(UINT32 nID, UINT32 nRst, TListMemVulnRst* pVulnPatchList)
{

	m_tMutex.Lock();
	SendToken.Clear();
	SendToken.TokenAdd_32(nID);
	SendToken.TokenAdd_32(nRst);

	if(pVulnPatchList && pVulnPatchList->size())
	{
		SendToken.TokenAdd_32(pVulnPatchList->size());

		TListMemVulnRstItor begin, end;
		begin = pVulnPatchList->begin();	end = pVulnPatchList->end();
		for(begin; begin != end; begin++)
		{
			PMEM_VULN_RST pdata_rst = &(*begin);

			SendToken.TokenAdd_IDMap(pdata_rst->tIntMap);
			SendToken.TokenAdd_IDMapStr(pdata_rst->tStrMap);

			SendToken.TokenSet_Block();
		}
	}
	else
	{
		SendToken.TokenAdd_32(0);
	}

	SendData_Link(G_TYPE_CTL_VULN, G_CODE_COMMON_SCAN, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	WriteLogN("[%s] send pkt scan : [id:%d][rst:%d][rst_num:%d]", m_strLogicName.c_str(), nID, nRst, (pVulnPatchList ? pVulnPatchList->size() : 0));
	
}
//---------------------------------------------------------------------------



