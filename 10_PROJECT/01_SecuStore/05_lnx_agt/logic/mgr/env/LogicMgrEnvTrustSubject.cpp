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
#include "LogicMgrEnvTrustSubject.h"

//---------------------------------------------------------------------------

CLogicMgrEnvTrustSubject*		t_LogicMgrEnvTrustSubject = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvTrustSubject::CLogicMgrEnvTrustSubject()
{
	t_ManageEnvTrustSubject	= new CManageEnvTrustSubject();
	t_ManageEnvTrustSubject->LoadDBMS();
	t_ManageEnvTrustSubject->InitHash();

	m_strLogicName		= "mgr env trust subject";

	m_nControlType		= SS_CONTROL_TYPE_ENV_TRUST_SUBJECT;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_TRUST_SUBJECT;
}
//---------------------------------------------------------------------------

CLogicMgrEnvTrustSubject::~CLogicMgrEnvTrustSubject()
{
	SAFE_DELETE(t_ManageEnvTrustSubject);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvTrustSubject::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvTrustSubject::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvTrustSubject	tUnitList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_TRUST_SUBJECT dets;

		if( t_ManageEnvTrustSubject->GetPkt(RecvToken, dets)) return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(dets);		
	}
	
	{
		TListID tIDList;
		t_ManageEnvTrustSubject->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvTrustSubject->IsExistID(*begin, tUnitList))		continue;
			
			t_ManageEnvTrustSubject->DelEnvTrustSubject(*begin);
		}
	}

	{
		TListDBEnvTrustSubjectItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvTrustSubject->ApplyEnvTrustSubject(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageEnvTrustSubject->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvTrustSubject::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvTrustSubject::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
