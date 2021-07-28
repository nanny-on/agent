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
#include "LogicMgrEnvSOrgLink.h"

//---------------------------------------------------------------------------

CLogicMgrEnvSOrgLink*		t_LogicMgrEnvSOrgLink = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvSOrgLink::CLogicMgrEnvSOrgLink()
{
	t_ManageEnvSOrgLink	= new CManageEnvSOrgLink();
	t_ManageEnvSOrgLink->LoadDBMS();
	t_ManageEnvSOrgLink->InitHash(); 

	m_strLogicName		= "mgr env sorg link";

	m_nControlType		= SS_CONTROL_TYPE_ENV_SORG_LINK;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_SORG_LINK;
}
//---------------------------------------------------------------------------

CLogicMgrEnvSOrgLink::~CLogicMgrEnvSOrgLink()
{
	SAFE_DELETE(t_ManageEnvSOrgLink);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSOrgLink::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvSOrgLink::AnalyzePkt_FromMgr_Edit_Ext()
{
	DB_ENV_SORG_LINK desl, desl_old;
	PDB_ENV_SORG_LINK pdesl = NULL;

	if( t_ManageEnvSOrgLink->GetPkt(RecvToken, desl))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	
	pdesl = t_ManageEnvSOrgLink->FirstItem();
	if(pdesl)	desl_old = *pdesl;

	if(t_ManageEnvSOrgLink->ApplyEnvSOrgLink(desl))
	{
		SetDLEH_EC(g_nErrRtn);
		WriteLogE("[%s] apply env information : [%d]", m_strLogicName.c_str(), g_nErrRtn);			
		return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
	}
	t_ManageEnvSOrgLink->InitHash();

	{
		if(desl.tDPH.nUsedMode != desl_old.tDPH.nUsedMode || desl.nUserCatchType != desl_old.nUserCatchType)
		{
			t_LogicMgrHost->SendPkt_Edit();
			WriteLogN("[%s] resend host info pkt because changed option..[%d]/[%d],[%d]/[%d]", m_strLogicName.c_str(),
						desl.tDPH.nUsedMode, desl_old.tDPH.nUsedMode,
						desl.nUserCatchType, desl_old.nUserCatchType);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSOrgLink::ApplyPolicy()
{		
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvSOrgLink::OnTimer_Logic()
{
/*
	if(t_ManageWinSession.IsWinSessionChange())
	{
		PDB_ENV_SORG_LINK pdesl = t_ManageEnvSOrgLink->FirstItem();
		if(!pdesl || pdesl->tDPH.nUsedMode != STATUS_USED_MODE_ON)	return 0;
		if(pdesl->nUserCatchType != SS_ENV_SORG_LINK_USER_CATCH_TYPE_USER_NAME)	return 0;
		
		t_LogicMgrHost->SendPkt_Edit();
	}
*/
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
