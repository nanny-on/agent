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
#include "LogicBase.h"

//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH(UINT32 nOpType, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, String strDesc, UINT32 nSyncMode)
{
	return InitDLE(EVENT_NOTIFY_TYPE_HOST, 0, EVENT_SUBJECT_TYPE_HOST, 0, EVENT_TARGET_TYPE_HOST, 0, nOType, nOCode, nOID, strOInfo, nOpType, strDesc, "", "", nSyncMode);
}
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH_PE(UINT32 nOID, String strOInfo, String strDesc)
{
	return InitDLEH(EVENT_OPERATION_TYPE_EDIT, m_nEvtObjType, m_nEvtObjCode, nOID, strOInfo, strDesc, SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
}
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH_Sync(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc)
{
	return InitDLEH(nOpType, m_nEvtObjType, m_nEvtObjCode, nOID, strOInfo, strDesc, SS_LOG_EVENT_HOST_SYNC_MODE_ALL);
}
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH_SyncAgt(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc)
{
	return InitDLEH(nOpType, m_nEvtObjType, m_nEvtObjCode, nOID, strOInfo, strDesc, SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
}
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH_SyncSvr(UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc)
{
	return InitDLEH(nOpType, m_nEvtObjType, m_nEvtObjCode, nOID, strOInfo, strDesc, SS_LOG_EVENT_HOST_SYNC_MODE_SVR);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEU(UINT32 nOpType, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, String strDesc)
{
	return InitDLE(EVENT_NOTIFY_TYPE_HOST, 0, EVENT_SUBJECT_TYPE_USER, 0, EVENT_TARGET_TYPE_HOST, 0, nOType, nOCode, nOID, strOInfo, nOpType, strDesc, "", "", SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void	CLogicBase::InitDLEH_EC()
{	
	InitDLEH(m_nEvtOpType, m_nEvtObjType, m_nEvtObjCode, 0, "", "");
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::SetDLEH_EC(UINT32 nErrCode, INT32 nApplyPkt)
{
	if(nErrCode)	m_nLastErrCode = nErrCode;
	InitDLEH_EC();
	t_LogicMgrLogEvent->SetLogEvent(m_tDLE);

	if(nApplyPkt)
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(nErrCode);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void	CLogicBase::InitDLE_OBJ(UINT32 nOID, String strOInfo, UINT32 nSyncMode)
{	
	return InitDLE(EVENT_NOTIFY_TYPE_HOST, 0, m_nEvtSubType, 0, EVENT_TARGET_TYPE_HOST, 0, m_nEvtObjType, m_nEvtObjCode, nOID, strOInfo, m_nEvtOpType, "", "", "", nSyncMode);
}
//---------------------------------------------------------------------------

void	CLogicBase::InitDLE(UINT32 nNType, UINT32 nNID, UINT32 nSType, UINT32 nSID, UINT32 nTType, UINT32 nTID, UINT32 nOType, UINT32 nOCode, UINT32 nOID, String strOInfo, UINT32 nOpType, String strDesc, String strSInfo, String strTInfo, UINT32 nSyncMode)
{
	m_tDLE._init();
	String strSubjectName = "";

	m_tDLE.nRegDate			= GetCurrentDateTimeInt();
	m_tDLE.nEvtTime			= GetCurrentDateTimeInt();
	m_tDLE.nEvtErrCode		= m_nLastErrCode;

	m_tDLE.nSyncSvrMode		= nSyncMode;
	if(nSyncMode & SS_LOG_EVENT_HOST_SYNC_MODE_SVR)		HISYNCSTEPUP(m_tDLE.nSyncSvrStep);

	m_tDLE.nNotifyType		= nNType;
	m_tDLE.nNotifyID		= nNID;

	m_tDLE.nSubjectType		= nSType;
	m_tDLE.nSubjectID		= nSID;
	m_tDLE.strSubjectInfo	= strSInfo;

	m_tDLE.nTargetType		= nTType;
	m_tDLE.nTargetID		= nTID;
	m_tDLE.strTargetInfo	= strTInfo;

	m_tDLE.nObjectType		= nOType;
	m_tDLE.nObjectCode		= nOCode;
	m_tDLE.nObjectID		= nOID;
	m_tDLE.strObjectInfo	= strOInfo;

	m_tDLE.nOperationType	= nOpType;
	m_tDLE.strEventDescr	= strDesc;

	if(m_tDLE.strSubjectInfo.empty())
	{
		switch(nSType)
		{
			case EVENT_SUBJECT_TYPE_ADMIN:		m_tDLE.strSubjectInfo = "admin";		break;
			case EVENT_SUBJECT_TYPE_GROUP:		m_tDLE.strSubjectInfo = "group";		break;
			case EVENT_SUBJECT_TYPE_HOST:		m_tDLE.strSubjectInfo = "host";			break;
			case EVENT_SUBJECT_TYPE_USER:		m_tDLE.strSubjectInfo = "user";			break;		// USER로 변경
			case EVENT_SUBJECT_TYPE_SVR:		m_tDLE.strSubjectInfo = "system";		break;
		}
	}

	if(m_tDLE.strTargetInfo.empty())
	{
		switch(nTType)
		{
			case EVENT_TARGET_TYPE_ADMIN:		m_tDLE.strTargetInfo = "admin";			break;
			case EVENT_TARGET_TYPE_GROUP:		m_tDLE.strTargetInfo = "group";			break;
			case EVENT_TARGET_TYPE_HOST:		m_tDLE.strTargetInfo = "host";			break;
			case EVENT_TARGET_TYPE_USER:		m_tDLE.strTargetInfo = "user";			break;		// USER로 변경
			case EVENT_TARGET_TYPE_SVR:			m_tDLE.strTargetInfo = "system";		break;
		}
	}

	if(strDesc.empty() == FALSE)	m_tDLE.strEventDescr += SPrintf("%c", CHAR_TOKEN_F);
}
//---------------------------------------------------------------------------

void	CLogicBase::AppendDLEDesc(String strValue)
{
	if(strValue.empty())	return;

	m_tDLE.strEventDescr += strValue;
	m_tDLE.strEventDescr += SPrintf("%c", CHAR_TOKEN_F);
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::AppendDLEDesc(UINT32 nValue)
{
	String strValue;
	strValue = SPrintf("%u", nValue);

	m_tDLE.strEventDescr += strValue;
	m_tDLE.strEventDescr += SPrintf("%c", CHAR_TOKEN_F);
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::AppendDLEDesc(UINT32 nKey, String strValue)
{
	if(!nKey)	return;

	m_tDLE._set_desc_info(nKey, strValue);
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::AppendDLEDesc(UINT32 nKey, UINT32 nValue)
{
	if(!nKey)	return;

	String strValue;
	strValue = SPrintf("%u", nValue);

	AppendDLEDesc(nKey, strValue);
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::AppendDLEDescHex(UINT32 nKey, UINT32 nValue)
{
	if(!nKey)	return;

	String strValue;
	strValue = SPrintf("%u", nValue);

	AppendDLEDesc(nKey, strValue);
	return;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------







