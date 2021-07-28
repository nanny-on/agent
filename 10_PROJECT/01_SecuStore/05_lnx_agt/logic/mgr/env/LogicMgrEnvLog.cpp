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
#include "LogicMgrEnvLog.h"

//---------------------------------------------------------------------------

CLogicMgrEnvLog*		t_LogicMgrEnvLog = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvLog::CLogicMgrEnvLog()
{
	t_ManageEnvLog	= new CManageEnvLog();
	t_ManageEnvLog->LoadDBMS();
	t_ManageEnvLog->InitHash();

	m_strLogicName		= "mgr env log";

	m_nControlType		= SS_CONTROL_TYPE_ENV_LOG;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_LOG;
}
//---------------------------------------------------------------------------

CLogicMgrEnvLog::~CLogicMgrEnvLog()
{
	SAFE_DELETE(t_ManageEnvLog);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLog::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvLog::AnalyzePkt_FromMgr_Edit_Ext()
{
	TListDBEnvLog	tUnitList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_ENV_LOG deni;

		if( t_ManageEnvLog->GetPkt(RecvToken, deni))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tUnitList.push_back(deni);		
	}
	

	{
		TListDBEnvLogItor begin, end;
		begin = tUnitList.begin();	end = tUnitList.end();
		for(begin; begin != end; begin++)
		{
			if(t_ManageEnvLog->ApplyEnvLog(*begin))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] apply env information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageEnvLog->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLog::ApplyPolicy()
{	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLog::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
