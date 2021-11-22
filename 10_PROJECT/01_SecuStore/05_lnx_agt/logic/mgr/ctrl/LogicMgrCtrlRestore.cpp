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
#include "LogicMgrCtrlRestore.h"
//---------------------------------------------------------------------------

CLogicMgrCtrlRestore*		t_LogicMgrCtrlRestore = NULL;

//---------------------------------------------------------------------------

CLogicMgrCtrlRestore::CLogicMgrCtrlRestore()
{
	m_strLogicName		= "ctrl restore";

	m_nControlType		= SS_CONTROL_TYPE_CONTROL_RESTORE_CTRL;
	m_nEvtSubType		= EVENT_SUBJECT_TYPE_ADMIN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_HOST_CTRL_RESTORE;
}
//---------------------------------------------------------------------------

CLogicMgrCtrlRestore::~CLogicMgrCtrlRestore()
{
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRestore::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_INFO:		nRtn = AnalyzePkt_FromMgr_Ext_GetRestoreList();				break;
		case G_CODE_COMMON_PROGRESS:	nRtn = AnalyzePkt_FromMgr_Ext_ExcuteRestore();				break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRestore::AnalyzePkt_FromMgr_Add_Ext()
{
	INT32	nConSessionID = 0;
//	ASIWINRESTORE_POINT_INFO stRestoreInfo;
	String strDescription;

	if (!RecvToken.TokenDel_32(nConSessionID))		return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(m_nHostID))		return AZPKT_CB_RTN_PKT_INVALID;
//	if (RecvToken.TokenDel_String(stRestoreInfo.strDescription) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if (RecvToken.TokenDel_String(strDescription) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	RecvToken.TokenSkip_Block();

	m_tMutex.Lock();
	SendToken.TokenAdd_32(nConSessionID);
	SendToken.TokenAdd_32(m_nHostID);
	SendToken.TokenAdd_32(0);

	SendData_Mgr(G_TYPE_CTL_RESTORE, G_CODE_COMMON_INFO, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRestore::AnalyzePkt_FromMgr_Del_Ext()
{
	INT32	nConSessionID = 0, nSequenceNumber = 0;

	if (!RecvToken.TokenDel_32(nConSessionID))		return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(m_nHostID))		return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(nSequenceNumber))		return AZPKT_CB_RTN_PKT_INVALID;
	RecvToken.TokenSkip_Block();

	m_tMutex.Lock();
	SendToken.TokenAdd_32(nConSessionID);
	SendToken.TokenAdd_32(m_nHostID);
	SendToken.TokenAdd_32(nSequenceNumber);

	SendData_Mgr(G_TYPE_CTL_RESTORE, m_nPktCode, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------   
INT32		CLogicMgrCtrlRestore::AnalyzePkt_FromMgr_Ext_ExcuteRestore()
{
	INT32	nConSessionID = 0, nSequenceNumber = 0;

	if (!RecvToken.TokenDel_32(nConSessionID))		return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(m_nHostID))		return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(nSequenceNumber))		return AZPKT_CB_RTN_PKT_INVALID;
	RecvToken.TokenSkip_Block();

	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------   
INT32		CLogicMgrCtrlRestore::AnalyzePkt_FromMgr_Ext_GetRestoreList()
{
	INT32 nConSessionID = 0;

	if( RecvToken.TokenDel_32(nConSessionID) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_32(m_nHostID) < 0)		return AZPKT_CB_RTN_PKT_INVALID;

	m_tMutex.Lock();
	SendToken.TokenAdd_32(nConSessionID);
	SendToken.TokenAdd_32(m_nHostID);

	SendToken.TokenAdd_32(0);

	SendData_Mgr(G_TYPE_CTL_RESTORE, m_nPktCode, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return AZPKT_CB_RTN_SUCCESS;
}