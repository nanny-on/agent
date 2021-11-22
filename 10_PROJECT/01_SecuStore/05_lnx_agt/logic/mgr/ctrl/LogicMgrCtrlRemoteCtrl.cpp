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
#include "LogicMgrCtrlRemoteCtrl.h"
//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteCtrl*		t_LogicMgrCtrlRemoteCtrl = NULL;

//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteCtrl::CLogicMgrCtrlRemoteCtrl()
{
	m_strLogicName		= "ctrl remote control";

	m_nControlType		= SS_CONTROL_TYPE_CONTROL_REMOTE_CTRL;
	m_nEvtSubType		= EVENT_SUBJECT_TYPE_ADMIN;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_HOST_CTRL_REMOTE_REQUEST;
}
//---------------------------------------------------------------------------

CLogicMgrCtrlRemoteCtrl::~CLogicMgrCtrlRemoteCtrl()
{
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteCtrl::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_CTRL:		nRtn = AnalyzePkt_FromMgr_Ext_Ctrl();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteCtrl::AnalyzePkt_FromMgr_Ext_Ctrl()
{
	MEM_CTRL_REMOTE_CTRL tMCRC;
	
	if ( RecvToken.TokenDel_String(tMCRC.strSvrAddress) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if ( RecvToken.TokenDel_String(tMCRC.strSvrPort) < 0)			return AZPKT_CB_RTN_PKT_INVALID;
	if ( RecvToken.TokenDel_String(tMCRC.strModuleVersion) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if ( RecvToken.TokenDel_String(tMCRC.strDownloadInfo) < 0)		return AZPKT_CB_RTN_PKT_INVALID;
	if ( RecvToken.TokenDel_String(tMCRC.strHash) < 0)				return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(tMCRC.nConnectMode))					return AZPKT_CB_RTN_PKT_INVALID;
	if (!RecvToken.TokenDel_32(tMCRC.nAlertType))					return AZPKT_CB_RTN_PKT_INVALID;
	if ( RecvToken.TokenDel_String(tMCRC.strAlertMsg) < 0)			return AZPKT_CB_RTN_PKT_INVALID;

	RecvToken.TokenSkip_Block();

	ApplyRemotePolicy(&tMCRC);

	return AZPKT_CB_RTN_SUCCESS;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrCtrlRemoteCtrl::ApplyRemotePolicy(PMEM_CTRL_REMOTE_CTRL pInfo)
{
	INT32 nRet = -1;

	if (!pInfo)
		return nRet;

	m_nEvtOpType = EVENT_OPERATION_TYPE_CONTROL;

	String strRemotePath;
	strRemotePath = SPrintf("%s/util/ra.exe", t_EnvInfo->m_strRootPath.c_str());

	CHAR szArgument[BUFSIZ]	= {0, };
	snprintf(szArgument, BUFSIZ-1, " -idx %d -connect %s:%s -multi -service_run", rand(), pInfo->strSvrAddress.c_str(), pInfo->strSvrPort.c_str());

	ASI_TS_INFO tATI;
	ASI_TS_MGR_PARAM tATMP;
	{
		sprintf_ext(CHAR_MAX_SIZE, tATI.szTaskName, STR_TS_NAME_REMOTE_CTRL, GetTickCount()); 
		{
			tATI.nChkPeriod = ASI_TS_CHECK_PREIOD_TYPE_ONCE;
			tATI.nStartTime	= GetCurrentDateTimeInt() + 5;
		}

		tATMP.strTSChildPath		= strRemotePath;
		tATMP.strTSChildArgument	= szArgument;
		tATMP.nTSAutoDel		= 1;
		tATMP.nTSSingleRun		= 1;
		tATMP.nTSShow			= 1;
		tATMP.nTSShowMode		= SHOW_MODE_TYPE_REMOTE_CTRL;
		tATMP.nTSWaitMode		= 0;

		if(pInfo->nAlertType && pInfo->strAlertMsg.empty() == FALSE)
		{
			CFileUtil tFileUtil;
			String strCMFName;
			{
				strCMFName = SPrintf("%s/inven/cmd", t_EnvInfo->m_strRootPath.c_str());
				tFileUtil.ForceDirectory(strCMFName.c_str());
				strCMFName = SPrintf("%s/inven/cmd/%s", t_EnvInfo->m_strRootPath.c_str(), GetGUID().c_str());
			}
			{
				tFileUtil.WriteFileBuffer(strCMFName, pInfo->strAlertMsg);				
			}	

			tATMP.strTSConfirmMsg = strCMFName;
		}

		tATMP.nTSChildWaitMode = 1;
		tATMP.nTSChildHideMode = 0;
	}		
	t_ExecuteFileUtil->ExecuteFileByUser(tATI, tATMP);

	{
		InitDLE_OBJ(0, pInfo->strSvrAddress);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}
	
	return nRet;
}