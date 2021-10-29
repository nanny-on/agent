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
#include "LogicMgrHostStatusPoNc.h"

//---------------------------------------------------------------------------

CLogicMgrHostStatusPoNc*		t_LogicMgrHostStatusPoNc = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostStatusPoNc::CLogicMgrHostStatusPoNc()
{
	t_ManageHostStatusPoNc		= new CManageHostStatusPoNc();
	
	t_ManageHostStatusPoNc->LoadDBMS();

	m_strLogicName = "logic mgr host status po";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostStatusPoNc::~CLogicMgrHostStatusPoNc()
{
	SAFE_DELETE(t_ManageHostStatusPoNc);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostStatusPoNc::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrHostStatusPoNc::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_STATUS_PO_NC pdata = NULL;
	DB_HOST_STATUS_PO_NC data_old, data;

	if( t_ManageHostStatusPoNc->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

	if((pdata = t_ManageHostStatusPoNc->FirstItem()))
	{
		data_old = *pdata;	
	}

	if((CompareMapID(data_old.tWorkModeMap, data.tWorkModeMap) == 0 &&
		CompareMapIDStr(data_old.tPtnVerMap, data.tPtnVerMap) == 0))
	{
		return AZPKT_CB_RTN_SUCCESS_END;
	}

	data_old.nHostID		= data.nHostID;

	data_old.tWorkModeMap	= data.tWorkModeMap;
	data_old.tPtnVerMap		= data.tPtnVerMap;

	if(SetER(t_ManageHostStatusPoNc->ApplyHostStatusPoNc(data_old)))
	{
		WriteLogE("[%s] edit host status po nc information fail : [%u]", m_strLogicName.c_str(), data_old.nID);
		return AZPKT_CB_RTN_DBMS_FAIL;
	}

	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(ERR_SUCCESS);	
		t_ManageHostStatusPoNc->SetPkt(&data_old, SendToken);
		SendData_Link(m_nPktType, G_CODE_COMMON_EDIT, SendToken);
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
//---------------------------------------------------------------------------

void		CLogicMgrHostStatusPoNc::SendPkt_HostStatusPoNc(DB_HOST_STATUS_PO_NC data)
{
	INT32 nSendPkt = 1;	
	
	DB_HOST_STATUS_PO_NC data_old;
	PDB_HOST_STATUS_PO_NC pdata = t_ManageHostStatusPoNc->FirstItem();
	if(pdata)
	{
		data_old = *pdata;
	}	

	do 
	{
		if((CompareMapID(data_old.tWorkModeMap, data.tWorkModeMap) ||
			CompareMapIDStr(data_old.tPtnVerMap, data.tPtnVerMap)))
		{
			break;
		}

		nSendPkt = 0;
	} while (FALSE);

	if(!nSendPkt)	return;

	m_tMutex.Lock();
	SendToken.Clear();
	t_ManageHostStatusPoNc->SetPkt(&data, SendToken);
	SendData_Mgr(G_TYPE_HOST_STATUS_PO_NC, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrHostStatusPoNc::SendPkt_HostStatusPoNc()
{
	PDB_HOST_STATUS_PO_NC pdata = t_ManageHostStatusPoNc->FirstItem();
	if(!pdata)
	{
		WriteLogE("[%s] not find host status po first item", m_strLogicName.c_str());
		return;
	}	

	m_tMutex.Lock();
	SendToken.Clear();
	t_ManageHostStatusPoNc->SetPkt(pdata, SendToken);
	SendData_Mgr(G_TYPE_HOST_STATUS_PO_NC, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------
