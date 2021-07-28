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
#include "LogicLgnAuth.h"

//---------------------------------------------------------------------------

CLogicLgnAuth*		t_LogicLgnAuth = NULL;

//---------------------------------------------------------------------------

CLogicLgnAuth::CLogicLgnAuth()
{
	m_strLogicName = "logic lgn svr";
}
//---------------------------------------------------------------------------

CLogicLgnAuth::~CLogicLgnAuth()
{
}
//---------------------------------------------------------------------------

void		CLogicLgnAuth::AnalyzePkt_Auth(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
    {
		case AL_CODE_AUTH_REG:			AnalyzePkt_Auth_Reg();			break;
		case AL_CODE_AUTH_LOGIN:		AnalyzePkt_Auth_Login();		break;
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

void		CLogicLgnAuth::AnalyzePkt_Auth_Reg()
{
	if(!RecvToken.TokenDel_32(m_nRecvValue))	goto INVALID_PKT;

	switch(m_nRecvValue)
	{
		case HOST_AUTH_RESULT_NO_ERROR:
		{
			t_EnvInfo->SetReg_HostRegTime(GetCurrentDateTimeInt());
			break;
		}
		case HOST_AUTH_RESULT_INVALID_GUID:
		{
			String strHostKey;			
			if(RecvToken.TokenDel_String(strHostKey) < 0)	goto INVALID_PKT;

			t_EnvInfo->SetReg_HostKey(strHostKey);
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_REG_CHG_GUID:
		{
			t_EnvInfo->SetReg_HostKey(GetGUID());
			break;
		}
		default:
		{
			WriteLogE("[%s] fail register on lgn svr : [%d]", m_strLogicName.c_str(), m_nRecvValue);
		}
	}	
	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_LOGIN_SVR_AUTH_LAST);
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);

	return;
	
INVALID_PKT:
	m_tPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
	WriteLogE("[%s] recv invalid pkt : [%d][%d]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
	return;
}
//--------------------------------------------------------------------------- 

void		CLogicLgnAuth::AnalyzePkt_Auth_Login()
{
	UINT32 nEnableTimerID = TIMER_ID_LOGIN_SVR_AUTH;
	UINT32 nRevChkValue = 0;
	
	if(!RecvToken.TokenDel_32(m_nRecvValue))
		goto INVALID_LGN_PKT;
	
	switch(m_nRecvValue)
	{
		case HOST_AUTH_RESULT_NO_ERROR:
		{
			String strSvrInfoList;
			String strCrossAuthKey;

			m_nDbgPos = HOST_AUTH_RESULT_NO_ERROR;
			if( RecvToken.TokenDel_String(strSvrInfoList) < 0)
				goto INVALID_LGN_PKT;
			if(!RecvToken.TokenDel_32(nRevChkValue))
				goto INVALID_LGN_PKT;
			if( RecvToken.TokenDel_String(strCrossAuthKey) < 0)
				goto INVALID_LGN_PKT;

			if(nRevChkValue != HOST_AUTH_CHECK_VALUE)
			{
				WriteLogE("invalid check value when auth login svr..");
				goto INVALID_LGN_PKT;
			}

			if(IsValidCrossAuth(strCrossAuthKey))
			{
				WriteLogE("invalid cross auth key when auth login svr..");
				goto INVALID_LGN_PKT;
			}

			t_EnvInfoOp->SetMgrSvrInfo(strSvrInfoList);
			
			nEnableTimerID = TIMER_ID_MANAGER_SVR_AUTH;
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_REGISTER:
		{
			WriteLogN("[%s] login fail : retry register on svr", m_strLogicName.c_str());
			t_EnvInfo->SetReg_HostRegTime(0);
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_CONNECT:
		{
			WriteLogN("[%s] login fail : retry connect on svr", m_strLogicName.c_str());
			break;
		}
		case HOST_AUTH_RESULT_FAIL_RETRY_REG_CHG_GUID:
		{
			WriteLogN("[%s] login fail : retry register on svr after changing host key", m_strLogicName.c_str());
			t_EnvInfo->SetReg_HostKey(GetGUID());
			break;
		}
		case HOST_AUTH_RESULT_AUTO_UNINSTALL:
		{
			String strCrossAuthKey;

			if(!RecvToken.TokenDel_32(nRevChkValue))
			{
				goto INVALID_LGN_PKT;
			}
			if(nRevChkValue != HOST_AUTH_CHECK_VALUE)
			{
				WriteLogE("invalid check value when auth login svr..");
				goto INVALID_LGN_PKT;
			}

			if(IsValidCrossAuth(strCrossAuthKey))
			{
				WriteLogE("invalid cross auth key when auth login svr..");
				goto INVALID_LGN_PKT;
			}
			
			WriteLogN("[%s] login fail : auto remove module", m_strLogicName.c_str());
			t_LogicMgrHost->SetUninstallHost();
			break;
		}
		case HOST_AUTH_RESULT_NOT_READY_UDT_SVR_INFO:
		{
			WriteLogN("[%s] login fail : retry connect.. checking update svr info", m_strLogicName.c_str());
			break;
		}
		case HOST_AUTH_RESULT_NOT_READY_MGR_SVR_INFO:
		{
			WriteLogN("[%s] login fail : retry connect.. checking manager svr info", m_strLogicName.c_str());
			break;
		}
		case HOST_AUTH_RESULT_REQUEST_UPDATE:
		{
			String strSvrInfoList;
			String strCrossAuthKey;


			m_nDbgPos = HOST_AUTH_RESULT_REQUEST_UPDATE;
			if( RecvToken.TokenDel_String(strSvrInfoList) < 0)
				goto INVALID_LGN_PKT;
			if(!RecvToken.TokenDel_32(nRevChkValue))
				goto INVALID_LGN_PKT;
			if( RecvToken.TokenDel_String(strCrossAuthKey) < 0)
				goto INVALID_LGN_PKT;

			if(nRevChkValue != HOST_AUTH_CHECK_VALUE)
			{
				WriteLogE("invalid check value when auth login svr..");
				goto INVALID_LGN_PKT;
			}

			if(IsValidCrossAuth(strCrossAuthKey))
			{
				WriteLogE("invalid cross auth key when auth login svr..");
				goto INVALID_LGN_PKT;
			}

			t_EnvInfoOp->SetUdtChkSvrInfo(strSvrInfoList);

			nEnableTimerID = TIMER_ID_UPDATE_SVR_AUTH;
			break;
		}
		default:
		{
			WriteLogE("[%s] fail login on lgn svr : [%d]", m_strLogicName.c_str(), m_nRecvValue);
		}
	}	

	t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_LOGIN_SVR_AUTH_LAST);
	t_ThreadTimer->t_TimerUtil.EnableTimer(nEnableTimerID);

	return;
	
INVALID_LGN_PKT:
	m_tPktData->sock_evt.nErrorCode = ERR_SOCKET_SVR_TO_CLT_INVALID_PKT;
	WriteLogE("[%s] recv invalid pkt : [%d][%d]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
	return;
}
//--------------------------------------------------------------------------- 
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicLgnAuth::SendAuthReg()
{
	String strMacAddr;
    char szIPAddr[MAX_NETADDR_LEN] = {0, };
	char szMacAddr[MAX_NETADDR_LEN] = {0, };
	int i, nCount = 0;
	BOOL bMatch = FALSE;
	String strCurIP = GetConnectAddr_Lgn();
	SendToken.Clear();

	nCount = t_NetworkDLLUtil->GetInterfaceCount();
	if(nCount < 1)
	{
		WriteLogE("[%s] invalid interface count (%d)", m_strLogicName.c_str(), nCount);
		return;
	}
	bMatch = FALSE;
	for(i=0; i<nCount; i++)
	{
		memset(szIPAddr, 0, MAX_NETADDR_LEN);
		memset(szMacAddr, 0, MAX_NETADDR_LEN);
		t_NetworkDLLUtil->GetIPAddr(i, szIPAddr, MAX_NETADDR_LEN);
		t_NetworkDLLUtil->GetMacAddress(i, szMacAddr, MAX_NETADDR_LEN);
		if(!_stricmp(strCurIP.c_str(), "127.0.0.1"))
		{
			bMatch = TRUE;
			break;
		}

		if(!strcmp(strCurIP.c_str(), szIPAddr))
		{
			bMatch = TRUE;
			break;
		}
	}		

	if(bMatch == FALSE)
	{
		WriteLogE("[%s] not find auth reg info", m_strLogicName.c_str());
		return;
	}

	if(t_EnvInfo->m_strLogicalMacAddr.empty())
	{
		strMacAddr = szMacAddr;
	}
	else
	{
		strMacAddr = t_EnvInfo->m_strLogicalMacAddr;
	}

	WriteLogN("[%s] reg current net hardware info : [%s][%s]", m_strLogicName.c_str(), szIPAddr, strMacAddr.c_str());

	if (t_EnvInfo->m_strHostKey.empty())
		t_EnvInfo->SetReg_HostKey(GetGUID());

	SendToken.TokenAdd_String(t_EnvInfo->m_strHostKey);
	SendToken.TokenAdd_String(strMacAddr);
	SendToken.TokenAdd_String(t_EnvInfoOp->GetComputerName());

	SendToken.TokenAdd_String(szIPAddr);

	SendData_Lgn(AL_TYPE_AUTH, AL_CODE_AUTH_REG, SendToken);

	return;
}
//---------------------------------------------------------------------------

void		CLogicLgnAuth::SendAuthLogin()
{
    char szIPAddr[MAX_NETADDR_LEN] = {0, };
	char szMacAddr[MAX_NETADDR_LEN] = {0, };
	int nCount = 0;
	BOOL bMatch = FALSE;
	String strMacAddr;
	String strCurIP = GetConnectAddr_Lgn();

	SendToken.Clear();

	nCount = t_NetworkDLLUtil->GetInterfaceCount();
	if(nCount < 1)
	{
		WriteLogE("[%s] invalid interface count (%d)", m_strLogicName.c_str(), nCount);
		return;
	}
	bMatch = FALSE;

	for(int i=0; i<nCount; i++)
	{
		memset(szIPAddr, 0, MAX_NETADDR_LEN);
		memset(szMacAddr, 0, MAX_NETADDR_LEN);
		t_NetworkDLLUtil->GetIPAddr(i, szIPAddr, MAX_NETADDR_LEN);
		t_NetworkDLLUtil->GetMacAddress(i, szMacAddr, MAX_NETADDR_LEN);
		if(!_stricmp(strCurIP.c_str(), "127.0.0.1"))
		{
			bMatch = TRUE;
			break;
		}

		if(!strcmp(strCurIP.c_str(), szIPAddr))
		{
			bMatch = TRUE;
			break;
		}
	}		

	if(bMatch == FALSE)
	{
		WriteLogE("[%s] not find auth login info", m_strLogicName.c_str());
		return;
	}

	if(t_EnvInfo->m_strLogicalMacAddr.empty())
	{
		strMacAddr = szMacAddr;
	}
	else
	{
		strMacAddr = t_EnvInfo->m_strLogicalMacAddr;
	}

	t_EnvInfoOp->m_strPriIPAddr = szIPAddr;
	t_EnvInfoOp->m_strPriMacAddr = strMacAddr;

	WriteLogN("[%s] login current net hardware info : [%s][%s]", m_strLogicName.c_str(), szIPAddr, strMacAddr.c_str());

	SendToken.TokenAdd_String(t_EnvInfo->m_strHostKey);
	SendToken.TokenAdd_String(t_EnvInfoOp->m_strPriMacAddr);
	SendToken.TokenAdd_String(t_EnvInfoOp->m_strPriIPAddr);
	SendToken.TokenAdd_String(t_EnvInfo->m_strHostBinVer);
	SendToken.TokenAdd_String(t_EnvInfo->m_strHostPtnVer);
	//linux/windows ±¸ºÐÀÚ
	SendToken.TokenAdd_32(BIN_MODULE_RUN_TYPE);

	SendData_Lgn(AL_TYPE_AUTH, AL_CODE_AUTH_LOGIN, SendToken);

	return;
}
//---------------------------------------------------------------------------

INT32		CLogicLgnAuth::IsValidCrossAuth(String strCrKey)
{
	PDB_PO_HOST_RUN pdata_run = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
	if(!pdata_run || pdata_run->nCrossAuthMode != STATUS_USED_MODE_ON)
	{
		return 0;
	}

	if(t_ManageHostKey->GetDecCrKey() != strCrKey)
	{
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
