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
#include "LogicMgrHostSys.h"

//---------------------------------------------------------------------------

CLogicMgrHostSys*		t_LogicMgrHostSys = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostSys::CLogicMgrHostSys()
{
	char acLogicName[MAX_TYPE_LEN] = "logic mgr host sys";
 	t_ManageHostSys		= new CManageHostSys();
	
	t_ManageHostSys->LoadDBMS();

	m_strLogicName = acLogicName;
	
	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;	
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
	m_nSendSysFlag = 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostSys::~CLogicMgrHostSys()
{
	SAFE_DELETE(t_ManageHostSys);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostSys::AnalyzePkt_FromMgr_Ext()
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrHostSys::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_SYS pdhs = NULL;
	DB_HOST_SYS dhs;

	if( t_ManageHostSys->GetPkt(RecvToken, dhs, SS_PKT_TYPE_AGENT))		return AZPKT_CB_RTN_PKT_INVALID;

	if( (pdhs = t_ManageHostSys->FirstItem()) == NULL)
	{
		WriteLogE("[%s] not find host sys info", m_strLogicName.c_str());
		return AZPKT_CB_RTN_DBMS_FAIL;	
	}

	if(pdhs->nOsID == dhs.nOsID && pdhs->nOsSP == dhs.nOsSP && 
		pdhs->nOsPd == dhs.nOsPd && pdhs->nOsPa == dhs.nOsPa &&
		pdhs->strOsPdLux == dhs.strOsPdLux)							return AZPKT_CB_RTN_SUCCESS_END;

	// 20200724 add jhjung
	if(pdhs->nOsID != dhs.nOsID)
		t_ManageHostSys->SetHostSysType(dhs.nOsID);

	pdhs->nOsID			= dhs.nOsID;
	pdhs->nOsSP			= dhs.nOsSP;
	pdhs->nOsPd			= dhs.nOsPd;
	pdhs->nOsPa			= dhs.nOsPa;
	pdhs->strOsPdLux	= dhs.strOsPdLux;	

	if(SetER(t_ManageHostSys->EditHostSys(*pdhs)))
	{
		WriteLogE("[%s] edit host information fail : [%u]", m_strLogicName.c_str(), pdhs->nID);
		return AZPKT_CB_RTN_DBMS_FAIL;	
	}

	{
		m_tMutex.Lock();
		t_ManageHostSys->SetPkt(SendToken);
		SendData_Link(G_TYPE_HOST_SYS, G_CODE_COMMON_EDIT, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrHostSys::SendPkt_Edit()
{
	INT32 nSendPkt = 1;	
	DB_HOST_SYS dhs;

	memset(&dhs, 0, sizeof(dhs));
	{
//		CSystemInfo tSystemInfo;
		if(t_SystemInfo != NULL)
		{
			dhs.nOsID	= t_SystemInfo->GetSystemID();
			dhs.nOsSP	= t_SystemInfo->GetSPID();
			dhs.nOsPa	= t_SystemInfo->GetSystemPaType();
			dhs.nOsPd	= t_SystemInfo->GetSystemPdType();
		}
		// 20200724 add jhjung
		//dhs.strOsPdLux	= "???????????????????????";
	}
	
	PDB_HOST_SYS pdhs = t_ManageHostSys->FirstItem();
	if(!pdhs)
	{
		WriteLogE("[%s] not find host sys first item..", m_strLogicName.c_str());
		goto SEND_PKT;
	}	

	do 
	{
		if(m_nSendSysFlag == 0)
		{
			break;
		}
		// 20200724 edit jhjung
		if( pdhs->nOsID != dhs.nOsID ||
			pdhs->nOsSP != dhs.nOsSP ||
			pdhs->nOsPa != dhs.nOsPa ||
			pdhs->nOsPd != dhs.nOsPd)
		{
			break;
		}

		nSendPkt = 0;
	} while (FALSE);

	if(!nSendPkt)	return;

SEND_PKT:
	m_tMutex.Lock();
	SendToken.Clear();
	t_ManageHostSys->SetPkt(&dhs, SendToken);
	SendData_Mgr(G_TYPE_HOST_SYS, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	m_nSendSysFlag = 1;
	return;
}
//---------------------------------------------------------------------------
void		CLogicMgrHostSys::SendPkt_SysInfo()
{
	INT32 nSendPkt = 1;	
	DB_HOST_SYS dhs;

	memset(&dhs, 0, sizeof(dhs));
	{
//		CSystemInfo tSystemInfo;
		if(t_SystemInfo != NULL)
		{
			dhs.nOsID	= t_SystemInfo->GetSystemID();
			dhs.nOsSP	= t_SystemInfo->GetSPID();
			dhs.nOsPa	= t_SystemInfo->GetSystemPaType();
			dhs.nOsPd	= t_SystemInfo->GetSystemPdType();
		}
	}
	m_tMutex.Lock();
	SendToken.Clear();
	t_ManageHostSys->SetPkt(&dhs, SendToken);
	SendData_Mgr(G_TYPE_HOST_SYS, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}