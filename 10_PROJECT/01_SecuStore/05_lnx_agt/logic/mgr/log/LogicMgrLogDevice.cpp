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
#include "LogicMgrLogDevice.h"

//---------------------------------------------------------------------------

CLogicMgrLogDevice*		t_LogicMgrLogDevice = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogDevice::CLogicMgrLogDevice()
{	
	t_ManageLogDevice	= new CManageLogDevice();
	t_LogicLogDevice	= new CLogicLogDevice();

	t_ManageLogDevice->LoadDBMS();

	m_strLogicName		= "logic mgr log device";

	m_nControlType		= SS_CONTROL_TYPE_LOG_DEVICE;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicMgrLogDevice::~CLogicMgrLogDevice()
{
	SAFE_DELETE(t_LogicLogDevice);
	SAFE_DELETE(t_ManageLogDevice);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDevice::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		nRtn = AnalyzePkt_FromMgr_Ext_Sync();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDevice::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_LOG_DEVICE dls;

	if(t_ManageLogDevice->GetPkt(RecvToken, dls))		return AZPKT_CB_RTN_PKT_INVALID;

	dls.nEvtTime	= t_ValidTimeUtil->GetValidTime();
	HISYNCSTEPUP(dls.nSyncSvrStep);

	SetLogDevice(dls);

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDevice::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDevice::AnalyzePkt_FromMgr_Del_Ext()
{
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogDevice::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBLogDevice tDBLogDeviceList;

	if(CheckPktResult(RecvToken))					return AZPKT_CB_RTN_RESULT_FAILED;
	if(!RecvToken.TokenDel_32(m_nRecvNum))			return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		DB_LOG_DEVICE tDLS;
		if(t_ManageLogDevice->GetPkt(RecvToken, tDLS))		return AZPKT_CB_RTN_PKT_INVALID;

		tDBLogDeviceList.push_back(tDLS);
	}

	{
		TListDBLogDeviceItor begin, end;
		begin = tDBLogDeviceList.begin();	end = tDBLogDeviceList.end();
		for(begin; begin != end; begin++)
		{
			t_ManageLogDevice->EditLogDevice(*begin);
		}
	}
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogDevice::SetLogDevice(DB_LOG_DEVICE& dls)
{
	{
		PDB_ENV_LOG_UNIT pDELEU = t_ManageEnvLogUnit->FindRecordLogDeviceUnit(&dls);
		if(pDELEU && pDELEU->tDPH.nUsedMode == STATUS_USED_MODE_ON)
		{
			dls.nSkipTarget = pDELEU->tDPH.nExtOption;
		}
	}

	{
		dls.nUserID = t_ManageHost->GetUserID(t_ManageHost->FirstID());
	}
	
	if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))
	{
		t_ManageLogDevice->AddLogDevice(dls);
	}

	{	
		m_tMutex.Lock();
		SendToken.Set(1024);
		SendToken.TokenAdd_32(1);
		t_ManageLogDevice->SetPkt(&dls, SendToken);
		if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_SERVER))
		{
			SendData_Mgr(G_TYPE_LOG_DEVICE, G_CODE_COMMON_SYNC, SendToken);
		}
		if(!(dls.nSkipTarget & SS_ENV_LOG_OPTION_FLAGE_SKIP_SAVE_AGENT))		
		{
			SendData_Link(G_TYPE_LOG_DEVICE, G_CODE_COMMON_SYNC, SendToken);
		}
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrLogDevice::SendPkt_Sync(INT32 nOnceMaxNum)
{
	TListPVOID tSendList;
	{
		t_ManageLogDevice->SetPktSync(tSendList);

		if(tSendList.empty())	return;
	}

	INT32 nOnceNum = nOnceMaxNum;
	INT32 nSendNum = 0;

	TListPVOIDItor begin, end;
	begin = tSendList.begin();	end = tSendList.end();

	while(nSendNum < tSendList.size())
	{
		nOnceNum = (((tSendList.size() - nSendNum) > nOnceMaxNum && nOnceMaxNum > 0) ? nOnceMaxNum : (tSendList.size() - nSendNum));

		m_tMutex.Lock();
		SendToken.Clear();
		SendToken.TokenAdd_32(nOnceNum);
		for(begin; begin != end && nOnceNum; begin++)
		{
			t_ManageLogDevice->SetPkt((PDB_LOG_DEVICE)(*begin), SendToken);

			nSendNum += 1;
			nOnceNum -= 1;
		}
		SendData_Mgr(G_TYPE_LOG_DEVICE, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------
