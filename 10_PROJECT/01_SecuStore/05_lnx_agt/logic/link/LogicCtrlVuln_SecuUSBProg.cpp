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
#include "LogicCtrlVuln.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32		CLogicCtrlVuln::ScanSecuUSBProg()
{
	TListMemVulnRst tInsiList, tNotiList;

	PDB_PO_IN_VULN_SECU_USB pCurPolicy = (PDB_PO_IN_VULN_SECU_USB)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SECU_USB);
	if(!pCurPolicy)
	{
		SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_CHK_FAIL, NULL);
		return SS_PTN_VULN_SCAN_RESULT_CHK_FAIL;
	}
	if(pCurPolicy->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	{
		SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_SAFE, NULL);
		return SS_PTN_VULN_SCAN_RESULT_SAFE;
	}

	TListID tIDList;
	t_ManagePoInVulnSecuUsbPkg->GetKeyIDList(pCurPolicy->tDPH.nID, tIDList);
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SECU_USB_UNIT pdata_unit = t_ManagePoInVulnSecuUsbUnit->FindItem(*begin);
		if(!pdata_unit)	return -1;

		if(pdata_unit->tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;

		INT32 nMatched = -1;
		do 
		{
			INT32 nBlockRst = 0;
			{
				nBlockRst = ScanSecuUSBProg_ChkReg(pdata_unit->strRegValue);
				if(nBlockRst > -1)		nMatched = nBlockRst;
				if(nMatched == 1)	break;
			}
			{
				nBlockRst = ScanSecuUSBProg_ChkProc(pdata_unit->strProcess);
				if(nBlockRst > -1)		nMatched = nBlockRst;
				if(nMatched == 1)	break;
			}
			{
				nBlockRst = ScanSecuUSBProg_ChkFile(pdata_unit->strFilePath);
				if(nBlockRst > -1)		nMatched = nBlockRst;
				if(nMatched == 1)	break;
			}
		} while (FALSE);
		

		if(nMatched == -1)	continue;

		{
			MEM_VULN_RST tMVP;	
			tMVP._set_str(0, pdata_unit->tDPH.strName);
			tMVP._set_str(1, pdata_unit->tDPH.strDescr);

			if(nMatched)			tInsiList.push_back(tMVP);
			else					tNotiList.push_back(tMVP);
		}
	}

	{
		if(tInsiList.size() + tNotiList.size() == 0)	SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_SAFE, NULL);
		else 
		{
			if(tInsiList.size())						SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_SAFE, NULL);
			else										SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_DANGER, &tNotiList);
		}
	}
	return SS_PTN_VULN_SCAN_RESULT_SCANNING;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanSecuUSBProg_ChkReg(String strRegKey)
{
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanSecuUSBProg_ChkProc(String strProcess)
{
	if(strProcess.empty())		return -1;

	CProcUtil tProcUtil;
	if(tProcUtil.ProcessExist(strProcess.c_str()))	return 1;
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanSecuUSBProg_ChkFile(String strFilePath)
{
	if(strFilePath.empty())		return -1;

	CFileUtil tFileUtil;
	String strOrFilePath = tFileUtil.GetEnvPathToLocalPathSingle(strFilePath);

	if(tFileUtil.FileExists(strOrFilePath.c_str()))	return 1;
	return 0;
}
//---------------------------------------------------------------------------




