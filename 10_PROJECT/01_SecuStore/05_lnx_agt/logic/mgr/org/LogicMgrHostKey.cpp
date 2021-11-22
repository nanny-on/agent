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
#include "LogicMgrHostKey.h"

//---------------------------------------------------------------------------

CLogicMgrHostKey*		t_LogicMgrHostKey = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostKey::CLogicMgrHostKey()
{
 	t_ManageHostKey		= new CManageHostKey();
	
	t_ManageHostKey->LoadDBMS();

	m_strLogicName = "logic mgr host key";
	
	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;	
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostKey::~CLogicMgrHostKey()
{
	SAFE_DELETE(t_ManageHostKey);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostKey::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrHostKey::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_KEY pdata = NULL;
	DB_HOST_KEY data;

	if( t_ManageHostKey->GetPkt(RecvToken, data, SS_PKT_TYPE_AGENT))		return AZPKT_CB_RTN_PKT_INVALID;

	if( (pdata = t_ManageHostKey->FirstItem()) == NULL)
	{
		WriteLogE("[%s] not find host key info", m_strLogicName.c_str());
		return AZPKT_CB_RTN_DBMS_FAIL;	
	}

	if(pdata->strBkKey == data.strBkKey && 
		pdata->strCrKey == data.strCrKey)
	{
		pdata->strRcKey = data.strBkKey;
		return AZPKT_CB_RTN_SUCCESS_END;
	}
	
	WriteLogN("[%s] recv host key chg apply", m_strLogicName);

	pdata->strBkKey = data.strBkKey;
	pdata->strCrKey = data.strCrKey;

	if(SetER(t_ManageHostKey->EditHostKey(*pdata)))
	{
		WriteLogE("[%s] edit host information fail : [%u]", m_strLogicName.c_str(), pdata->nID);
		return AZPKT_CB_RTN_DBMS_FAIL;	
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

void		CLogicMgrHostKey::SendPkt_Hash()
{
	m_tMutex.Lock();
	SendToken.Clear();
	SendToken.TokenAdd_String(t_ManageHostKey->GetHash());

	SendData_Mgr(G_TYPE_HOST_KEY, G_CODE_COMMON_HASH, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	t_ManageHostKey->ClearHostKey_RcKey();

	return;
}
//---------------------------------------------------------------------------