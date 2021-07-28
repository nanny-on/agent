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
#include "LogicMgrSiteVuln.h"

//---------------------------------------------------------------------------

CLogicMgrSiteVuln*		t_LogicMgrSiteVuln = NULL;

//---------------------------------------------------------------------------

CLogicMgrSiteVuln::CLogicMgrSiteVuln()
{
	t_LogicSiteVuln		= new CLogicSiteVuln();
	t_ManageSiteVuln		= new CManageSiteVuln();
	
	t_ManageSiteVuln->LoadDBMS();
	
	t_ManageSiteVuln->InitHash();

	m_strLogicName		= "mgr agt site vuln";
	
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nControlType		= SS_CONTROL_TYPE_COMMON_SITE_VULN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_COMMON;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_COMMON_SITE_VULN;
}
//---------------------------------------------------------------------------

CLogicMgrSiteVuln::~CLogicMgrSiteVuln()
{
	SAFE_DELETE(t_ManageSiteVuln);

	SAFE_DELETE(t_LogicSiteVuln);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrSiteVuln::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrSiteVuln::AnalyzePkt_FromMgr_Edit_Ext()
{	
	TListDBSiteVuln	tDBSiteVulnList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_SITE_VULN		data;	

		if( t_ManageSiteVuln->GetPkt(RecvToken, data))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tDBSiteVulnList.push_back(data);
	}

	{
		TListID tIDList;
		t_ManageSiteVuln->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageSiteVuln->IsExistID(*begin, tDBSiteVulnList))		continue;

			t_ManageSiteVuln->DelSiteVuln(*begin);
		}
	}

	{
		TListDBSiteVulnItor begin, end;
		begin = tDBSiteVulnList.begin();	end = tDBSiteVulnList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageSiteVuln->ApplySiteVuln(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageSiteVuln->InitHash();
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
