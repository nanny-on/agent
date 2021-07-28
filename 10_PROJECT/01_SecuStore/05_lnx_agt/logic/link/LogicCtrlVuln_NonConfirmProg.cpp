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
//---------------------------------------------------------------------------


INT32		CLogicCtrlVuln::ScanNonConfirmProg()
{
	TListMemVulnRst tNotiList;

	PDB_PO_IN_VULN_SW pCurPolicy = (PDB_PO_IN_VULN_SW)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_VULN_SW);
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
	t_ManagePoInVulnSwPkg->GetKeyIDList(pCurPolicy->tDPH.nID, tIDList);
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_VULN_SW_PKG pdata_pkg = t_ManagePoInVulnSwPkg->FindPoInVulnSwPkg(pCurPolicy->tDPH.nID, *begin);
		if(!pdata_pkg)	continue;

		UINT8	nType = LOTYPEID32(*begin);
		UINT32	nUID = HITYPEID32(*begin);

		switch(nType)
		{
		case SS_COMMON_UNIT_TYPE_GLOBAL:	ScanNonConfirmProg_Global(pdata_pkg->strSwChkKey, pdata_pkg->strSwVersion, tNotiList);	break;
			case SS_COMMON_UNIT_TYPE_SITE:		ScanNonConfirmProg_Site(nUID, tNotiList);						break;
		}
	}

	{
		if(tNotiList.size())	SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_DANGER, &tNotiList);
		else					SendPkt_Scan(m_nRecvID, SS_PTN_VULN_SCAN_RESULT_SAFE, NULL);
	}
	return SS_PTN_VULN_SCAN_RESULT_SCANNING;

}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanNonConfirmProg_Global(String strSwName, String strSwVersion, TListMemVulnRst& tNotiList)
{
	PDB_HOST_SW pdata = t_ManageHostSw->FindChkKeyNameItem(strSwName, strSwVersion);
	if(!pdata || pdata->nUsedFlag != STATUS_USED_MODE_ON)
		return 0;

	{
		{
			MEM_VULN_RST tMVP;	
			tMVP._set_str(0, pdata->strName);
			tMVP._set_str(1, pdata->strPublisher);
			tMVP._set_str(2, pdata->strVersion);
			tMVP._set_str(3, pdata->strInsDate);

			tNotiList.push_back(tMVP);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanNonConfirmProg_Site(UINT32 nID, TListMemVulnRst& tNotiList)
{
	return 0;
}
//---------------------------------------------------------------------------


