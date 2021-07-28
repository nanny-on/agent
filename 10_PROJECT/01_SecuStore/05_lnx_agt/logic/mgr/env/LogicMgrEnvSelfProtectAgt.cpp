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
#include "LogicMgrEnvSelfProtectAgt.h"

//---------------------------------------------------------------------------

CLogicMgrEnvSelfProtectAgt*		t_LogicMgrEnvSelfProtectAgt = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvSelfProtectAgt::CLogicMgrEnvSelfProtectAgt()
{
	t_ManageEnvSelfProtectAgt	= new CManageEnvSelfProtectAgt();
	t_ManageEnvSelfProtectAgt->LoadDBMS();
	t_ManageEnvSelfProtectAgt->InitHash();

	m_strLogicName		= "mgr env self protect agt";

	m_nControlType		= SS_CONTROL_TYPE_ENV_SELF_PROTECT_AGT;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_SELF_PROTECT_AGT;
}
//---------------------------------------------------------------------------

CLogicMgrEnvSelfProtectAgt::~CLogicMgrEnvSelfProtectAgt()
{
	SAFE_DELETE(t_ManageEnvSelfProtectAgt);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSelfProtectAgt::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvSelfProtectAgt::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvSelfProtectAgt	tUnitList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_SELF_PROTECT_AGT despa;

		if( t_ManageEnvSelfProtectAgt->GetPkt(RecvToken, despa)) return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(despa);		
	}
	
	{
		TListID tIDList;
		t_ManageEnvSelfProtectAgt->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvSelfProtectAgt->IsExistID(*begin, tUnitList))		continue;
			
			t_ManageEnvSelfProtectAgt->DelEnvSelfProtectAgt(*begin);
		}
	}

	{
		TListDBEnvSelfProtectAgtItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvSelfProtectAgt->ApplyEnvSelfProtectAgt(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageEnvSelfProtectAgt->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSelfProtectAgt::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSelfProtectAgt::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
