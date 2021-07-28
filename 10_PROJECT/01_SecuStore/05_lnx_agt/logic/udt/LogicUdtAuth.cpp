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
#include "LogicUdtAuth.h"

//---------------------------------------------------------------------------

CLogicUdtAuth*		t_LogicUdtAuth = NULL;

//---------------------------------------------------------------------------

CLogicUdtAuth::CLogicUdtAuth()
{
	m_strLogicName = "logic udt svr auth";
}
//---------------------------------------------------------------------------

CLogicUdtAuth::~CLogicUdtAuth()
{
}
//---------------------------------------------------------------------------

void		CLogicUdtAuth::AnalyzePkt_Auth(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(pkt_data->hdr.code)
    {
		case AU_CODE_AUTH_LOGIN:		AnalyzePkt_Auth_Login();		break;
		default:
        {
        	WriteLogE("[%s] not define pkt code : [%d][%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode);
			break;
        }
    }
	InitBaseMember();
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

void		CLogicUdtAuth::AnalyzePkt_Auth_Login()
{
	UINT32 nEnableTimerID = 0;

	if(!RecvToken.TokenDel_32(m_nRecvValue))
		goto INVALID_AUTH_PKT;
	
	switch(m_nRecvValue)
	{
		case HOST_AUTH_RESULT_NO_ERROR:
		{
			String strSvrInfoList;
			TMapMemAgtUpFileInfo tMemAgtUpFileInfoMap;
			INT32 nFileIdx = 0;

			if( RecvToken.TokenDel_String(strSvrInfoList) < 0)		goto INVALID_AUTH_PKT;
			if(!RecvToken.TokenDel_32(m_nRecvNum))					goto INVALID_AUTH_PKT;
			while(m_nRecvNum--)
			{
				MEM_AGT_UP_FILE_INFO tMAUFI;

				if(!RecvToken.TokenDel_32(tMAUFI.nFileType))				goto INVALID_AUTH_PKT;
				if( RecvToken.TokenDel_String(tMAUFI.strFileName) < 0)		goto INVALID_AUTH_PKT;
				if( RecvToken.TokenDel_String(tMAUFI.strFileHash) < 0)		goto INVALID_AUTH_PKT;

				tMemAgtUpFileInfoMap[nFileIdx++] = tMAUFI;
			}
			if(tMemAgtUpFileInfoMap.size() == 0)
			{
				WriteLogE("[%s] invalid update operation.. recv up file info empty..", m_strLogicName.c_str());
				nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
			}
			else
			{
				t_EnvInfoOp->SetUdtDataSvrInfo(strSvrInfoList);
				t_EnvInfoOp->SetAgtUpFileInfo(tMemAgtUpFileInfoMap);
				
				nEnableTimerID = TIMER_ID_DOWNLOAD_UPDATE_FILE;
				WriteLogN("[%s] login auth last success : [%u]", m_strLogicName.c_str(), m_nRecvValue);
			}		
			
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_REGISTER:
		{
			WriteLogN("[%s] login fail : retry register on svr", m_strLogicName.c_str());
			t_EnvInfo->SetReg_HostRegTime(0);
			nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_CONNECT:
		{
			WriteLogN("[%s] login fail : retry connect on svr", m_strLogicName.c_str());
			nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
			break;
		}
		case HOST_AUTH_RESULT_NOT_READY_UDT_DATA_SVR_INFO:
		{
			WriteLogN("[%s] login fail : retry connect.. checking update data svr info", m_strLogicName.c_str());
			nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
			break;
		}
		default:
		{
			WriteLogE("[%s] fail login on udt svr : [%d]", m_strLogicName.c_str(), m_nRecvValue);
			nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
			break;
		}
	}	

	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_UPDATE_SVR_AUTH_LAST);
	t_ThreadTimer->t_TimerUtil.EnableTimer(nEnableTimerID);

	return;
	
INVALID_AUTH_PKT:
	m_tPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
	WriteLogE("[%s] recv invalid pkt : [%d][%d]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);

	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_UPDATE_SVR_AUTH_LAST);
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
	return;
}
//--------------------------------------------------------------------------- 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicUdtAuth::SendAuthLogin()
{
	SendToken.Clear();

	WriteLogN("[%s] udt current info : [%s]", m_strLogicName.c_str(), t_EnvInfo->m_strHostKey.c_str());

	SendToken.TokenAdd_String(t_EnvInfo->m_strHostKey);
	SendToken.TokenAdd_32(BIN_MODULE_RUN_TYPE);

	SendData_Udt(AU_TYPE_AUTH, AU_CODE_AUTH_LOGIN, SendToken);

	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
