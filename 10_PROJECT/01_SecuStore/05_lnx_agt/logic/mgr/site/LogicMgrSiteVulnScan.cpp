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
#include "LogicMgrSiteVulnScan.h"

//---------------------------------------------------------------------------

CLogicMgrSiteVulnScan*		t_LogicMgrSiteVulnScan = NULL;

//---------------------------------------------------------------------------

CLogicMgrSiteVulnScan::CLogicMgrSiteVulnScan()
{
	t_LogicSiteVulnScan		= new CLogicSiteVulnScan();
	t_ManageSiteVulnScan		= new CManageSiteVulnScan();
	
	t_ManageSiteVulnScan->LoadDBMS();
	
	t_ManageSiteVulnScan->InitHash();

	m_strLogicName		= "mgr agt site vuln scan";
	
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nControlType		= SS_CONTROL_TYPE_COMMON_SITE_VULN_SCAN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_COMMON;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_COMMON_SITE_VULN_SCAN;
}
//---------------------------------------------------------------------------

CLogicMgrSiteVulnScan::~CLogicMgrSiteVulnScan()
{
	SAFE_DELETE(t_ManageSiteVulnScan);

	SAFE_DELETE(t_LogicSiteVulnScan);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrSiteVulnScan::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteVulnScan::AnalyzePkt_FromMgr_Edit_Ext()
{	
	TListDBSiteVulnScan	tDBSiteVulnScanList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_SITE_VULN_SCAN		data;	

		if( t_ManageSiteVulnScan->GetPkt(RecvToken, data))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tDBSiteVulnScanList.push_back(data);
	}

	{
		TListID tIDList;
		t_ManageSiteVulnScan->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageSiteVulnScan->IsExistID(*begin, tDBSiteVulnScanList))		continue;

			t_ManageSiteVulnScan->DelSiteVulnScan(*begin);
		}
	}

	{
		TListDBSiteVulnScanItor begin, end;
		begin = tDBSiteVulnScanList.begin();	end = tDBSiteVulnScanList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageSiteVulnScan->ApplySiteVulnScan(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageSiteVulnScan->InitHash();
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
