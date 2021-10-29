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
#include "LogicBase.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CLogicBase::SendData_Mgr(PPKT_DATA pkt_data, MemToken& SendToken)
{
	return SendData_Mgr(pkt_data->hdr.type, pkt_data->hdr.code, SendToken);
}
//---------------------------------------------------------------------------

INT32	CLogicBase::SendData_Mgr(UINT16 nType, UINT16 nCode, MemToken& SendToken)
{
	return t_MgrCltUtil->Send_CLD(nType, nCode, SendToken.GetLength(), SendToken.GetData());
}

INT32	CLogicBase::IsInitialize_Mgr()
{
	return t_MgrCltUtil->IsInitialize();
}

//---------------------------------------------------------------------------

void	CLogicBase::EditSKey_Mgr(UINT32 nSessionKey)
{
	t_MgrCltUtil->EditSessionKey_CLD(nSessionKey);

    return;
}
//---------------------------------------------------------------------------

void	CLogicBase::Disconnected_Mgr()
{
	t_MgrCltUtil->CloseSocket_CLD(0);
	return;
}
//---------------------------------------------------------------------------

String	CLogicBase::GetConnectAddr_Mgr()
{
	char pszAddr[CHAR_MAX_SIZE] = {0, };
	UINT32 nAddr = GetConnectIP_Mgr();

    return ConvertIP(nAddr);
}
//---------------------------------------------------------------------------

UINT32	CLogicBase::GetConnectIP_Mgr()
{
	UINT32 nAddr = 0;
	t_MgrCltUtil->GetLocalAddress_CLD(nAddr);

    return nAddr;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void	CLogicBase::AnalyzePkt_FromMgr(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_ADD:					AnalyzePkt_FromMgr_Add();			break;
		case G_CODE_COMMON_EDIT:				AnalyzePkt_FromMgr_Edit();			break;
		case G_CODE_COMMON_DEL:					AnalyzePkt_FromMgr_Del();			break;
		default:
		{
			UINT32 nExtRtn = AnalyzePkt_FromMgr_Ext();
			switch(nExtRtn)
			{
				case AZPKT_CB_RTN_SUCCESS:			break;
				case AZPKT_CB_RTN_SUCCESS_END:		break;
				case AZPKT_CB_RTN_PKT_INVALID:
				{
					WriteLogE("[%s] recv invalid pkt from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
					break;
				}
				case AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE:
				{
					WriteLogE("[%s] recv not define pkt code from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
					break;
				}
				case AZPKT_CB_RTN_SEND_ACK:
				{
					m_tMutex.Lock();
					SendData_Mgr(m_nPktType, m_nPktCode, SendToken);
					SendToken.Clear();
					m_tMutex.UnLock();
					break;
				}
				case AZPKT_CB_RTN_RESULT_FAILED:
				{
					WriteLogE("[%s] recv invalid result from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
					break;
				}
				case AZPKT_CB_RTN_DBMS_FAIL:
				{
					break;
				}
				default:
				{
					WriteLogE("[%s] check pkt fail from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, nExtRtn);
					break;
				}
			}
			break;		
		}
	}
	InitBaseMember();
	return;
}
//---------------------------------------------------------------------------

void	CLogicBase::AnalyzePkt_FromMgr_Add()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_ADD;

	if(CheckPktResult(RecvToken))
	{
		WriteLogE("[%s] check pkt result is : [%x]:[pt:%x][pc:%x]", m_strLogicName.c_str(), m_nPktRst, m_nPktType, m_nPktCode);
		return;
	}

	UINT32 nExtRtn = AnalyzePkt_FromMgr_Add_Ext();
	switch(nExtRtn)
	{
		case AZPKT_CB_RTN_SUCCESS:
		{
			WriteLogN("[%s] recv pkt operation success : [op:%d][p_type:%x][p_code:%x]", m_strLogicName.c_str(), m_nEvtOpType, m_nPktType, m_nPktCode);
			break;
		}
		case AZPKT_CB_RTN_SUCCESS_END:
		{
			break;
		}
		case AZPKT_CB_RTN_PKT_INVALID:
		{
			WriteLogE("[%s] recv invalid pkt from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE:
		{
			WriteLogE("[%s] recv not define pkt code from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_SEND_ACK:
		{
			break;
		}
		case AZPKT_CB_RTN_RESULT_FAILED:
		{
			WriteLogE("[%s] recv invalid result from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_DBMS_FAIL:
		{
			break;
		}
		default:
		{
			WriteLogE("[%s] check pkt fail from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, nExtRtn);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void	CLogicBase::AnalyzePkt_FromMgr_Edit()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_EDIT;

	if(CheckPktResult(RecvToken))
	{
		WriteLogE("[%s] check pkt result is : [%x]:[pt:%x][pc:%x]", m_strLogicName.c_str(), m_nPktRst, m_nPktType, m_nPktCode);
		return;
	}

	UINT32 nExtRtn = AnalyzePkt_FromMgr_Edit_Ext();
	switch(nExtRtn)
	{
		case AZPKT_CB_RTN_SUCCESS:
		{
			if(m_tDPH)
				WriteLogN("[%s] recv policy pkt operation success : [op:%d][id:%d][sn:%d][used:%d]", m_strLogicName.c_str(), m_nEvtOpType, m_tDPH->nID, m_tDPH->nSeqNo, m_tDPH->nUsedMode);
			else
				WriteLogN("[%s] recv pkt operation success : [op:%d][p_type:%x][p_code:%x]", m_strLogicName.c_str(), m_nEvtOpType, m_nPktType, m_nPktCode);
			break;
		}
		case AZPKT_CB_RTN_SUCCESS_END:		
		{
			break;
		}
		case AZPKT_CB_RTN_PKT_INVALID:
		{
			WriteLogE("[%s] recv invalid pkt from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE:
		{
			WriteLogE("[%s] recv not define pkt code from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_SEND_ACK:
		{
			m_tMutex.Lock();
			SendData_Mgr(m_nPktType, m_nPktCode, SendToken); 
			SendToken.Clear();
			m_tMutex.UnLock();
			break;
		}
		case AZPKT_CB_RTN_RESULT_FAILED:
		{
			WriteLogE("[%s] recv invalid result from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_DBMS_FAIL:
		{
			break;
		}
		default:
		{
			WriteLogE("[%s] check pkt fail from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, nExtRtn);
			break;
		}
	}

	if(m_nPolicyType && m_tDPH)
	{
		t_DeployPolicyUtil->ApplyPolicy(m_nPolicyType, m_tDPH->nID);
		WriteLogN("[%s] recv changed policy info : [%s]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str());

		switch(ApplyPolicy())
		{
			case AZPKT_CB_RTN_SUCCESS:
				break;
			default:
			{
				WriteLogE("[%s] fail apply the received policy from mgr svr : [%s][%x]:[%x]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str(), m_nPktCode, m_nDbgPos);
				break;
			}
		}

		{
			m_tMutex.Lock();
			SendToken.TokenAdd_32(m_tDPH->nID);
			SendToken.TokenAdd_32(m_tDPH->nSeqNo);
			SendData_Mgr(m_nPktType, m_nPktCode, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}
	}	
	else
	{
		switch(ApplyPolicy())
		{
			case AZPKT_CB_RTN_SUCCESS:
				break;
			default:
			{
				WriteLogE("[%s] fail apply the received control from mgr svr : [%s][%x]:[%x]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str(), m_nPktCode, m_nDbgPos);
				break;
			}
		}
	}

	if(m_tDPH)
	{
		InitDLE_OBJ(m_tDPH->nID, m_tDPH->strName);
		t_LogicMgrLogEvent->SetLogEvent(m_tDLE);
	}

	{
		SendPktToLink();
	}
}
//---------------------------------------------------------------------------

void	CLogicBase::AnalyzePkt_FromMgr_Del()
{
	m_nEvtOpType = EVENT_OPERATION_TYPE_DEL;

	if(CheckPktResult(RecvToken))
	{
		WriteLogE("[%s] check pkt result is : [%x]:[pt:%x][pc:%x]", m_strLogicName.c_str(), m_nPktRst, m_nPktType, m_nPktCode);
		return;
	}

	UINT32 nExtRtn = AnalyzePkt_FromMgr_Del_Ext();
	switch(nExtRtn)
	{
		case AZPKT_CB_RTN_SUCCESS:
		{
			WriteLogN("[%s] recv pkt operation success : [op:%d][p_type:%x][p_code:%x]", m_strLogicName.c_str(), m_nEvtOpType, m_nPktType, m_nPktCode);
			break;
		}
		case AZPKT_CB_RTN_SUCCESS_END:
		{
			break;
		}
		case AZPKT_CB_RTN_PKT_INVALID:
		{
			WriteLogE("[%s] recv invalid pkt from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE:
		{
			WriteLogE("[%s] recv not define pkt code from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_SEND_ACK:
		{
			break;
		}
		case AZPKT_CB_RTN_RESULT_FAILED:
		{
			WriteLogE("[%s] recv invalid result from  svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, m_nDbgPos);
			break;
		}
		case AZPKT_CB_RTN_DBMS_FAIL:
		{
			break;
		}
		default:
		{
			WriteLogE("[%s] check pkt fail from svr mgr : [%x][%x]:[%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode, nExtRtn);
			break;
		}
	}
}
//---------------------------------------------------------------------------






