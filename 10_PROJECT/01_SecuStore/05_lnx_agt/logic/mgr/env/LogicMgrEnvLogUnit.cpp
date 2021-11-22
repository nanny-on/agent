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
#include "LogicMgrEnvLogUnit.h"

//---------------------------------------------------------------------------

CLogicMgrEnvLogUnit*		t_LogicMgrEnvLogUnit = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvLogUnit::CLogicMgrEnvLogUnit()
{
	t_ManageEnvLogUnit	= new CManageEnvLogUnit();
	t_ManageEnvLogUnit->LoadDBMS();
	t_ManageEnvLogUnit->InitHash();

	m_strLogicName		= "mgr env log unit";

	m_nControlType		= SS_CONTROL_TYPE_ENV_LOG_UNIT;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_LOG_UNIT;
}
//---------------------------------------------------------------------------

CLogicMgrEnvLogUnit::~CLogicMgrEnvLogUnit()
{
	SAFE_DELETE(t_ManageEnvLogUnit);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLogUnit::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvLogUnit::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvLogUnit	tUnitList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_LOG_UNIT deleu;

		if( t_ManageEnvLogUnit->GetPkt(RecvToken, deleu)) return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(deleu);		
	}
	
	{
		TListID tIDList;
		t_ManageEnvLogUnit->GetItemIDList(tIDList);
		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvLogUnit->IsExistID(*begin, tUnitList))		continue;
			
			t_ManageEnvLogUnit->DelEnvLogUnit(*begin);
		}
	}

	{
		TListDBEnvLogUnitItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvLogUnit->ApplyEnvLogUnit(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply env unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageEnvLogUnit->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLogUnit::ApplyPolicy()
{	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLogUnit::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
