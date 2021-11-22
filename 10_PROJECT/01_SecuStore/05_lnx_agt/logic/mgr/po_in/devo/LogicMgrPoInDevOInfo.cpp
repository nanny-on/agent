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
#include "LogicMgrPoInDevOInfo.h"

//---------------------------------------------------------------------------

CLogicMgrPoInDevOInfo*		t_LogicMgrPoInDevOInfo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInDevOInfo::CLogicMgrPoInDevOInfo()
{
	t_ManagePoInDevOInfo			= new CManagePoInDevOInfo();

	t_ManagePoInDevOInfo->LoadDBMS();

	t_ManagePoInDevOInfo->InitHash();

	m_strLogicName		= "mgr agt po in devo info";
	
	m_nControlType		= SS_CONTROL_TYPE_DVMS_DV_INFO;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_DV_INFO;
}
//---------------------------------------------------------------------------

CLogicMgrPoInDevOInfo::~CLogicMgrPoInDevOInfo()
{
	SAFE_DELETE(t_ManagePoInDevOInfo);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInDevOInfo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInDevOInfo::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBPoInDevOInfo	tUnitList;
	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_DEVO_INFO deni;

		if( t_ManagePoInDevOInfo->GetPkt(RecvToken, deni)) return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(deni);		
	}

	{
		TListID tIDList;
		t_ManagePoInDevOInfo->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoInDevOInfo->IsExistID(*begin, tUnitList))		continue;

			t_ManagePoInDevOInfo->DelPoInDevOInfo(*begin);
		}
	}

	{
		TListDBPoInDevOInfoItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManagePoInDevOInfo->ApplyPoInDevOInfo(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManagePoInDevOInfo->InitHash();
	}
//	t_ASIEPSAPPDLLUtil->Stop();
	t_ManageDevOInfo->CheckDvPolicy();
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);

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
