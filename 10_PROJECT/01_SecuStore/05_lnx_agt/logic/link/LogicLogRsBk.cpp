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
#include "LogicLogRsBk.h"

//---------------------------------------------------------------------------

CLogicLogRsBk*		t_LogicLogRsBk = NULL;

//---------------------------------------------------------------------------

CLogicLogRsBk::CLogicLogRsBk()
{	
	m_strLogicName = "logic log rs bk";
}
//---------------------------------------------------------------------------

CLogicLogRsBk::~CLogicLogRsBk()
{
}
//---------------------------------------------------------------------------

INT32	CLogicLogRsBk::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{		
		case G_CODE_COMMON_SYNC:		AnalyzePkt_FromLink_Ext_Sync();		break;
		case G_CODE_COMMON_RESTORE:		AnalyzePkt_FromLink_Ext_Restore();	break;
		default:						break;		
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicLogRsBk::AnalyzePkt_FromLink_Ext_Sync()
{
	INT32 nOnceNum = 0, nSendedNum = 0;;
	INT32 nTotalNum = t_ManageLogRsBk->Count();
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(nTotalNum);
		SendData_Link(m_nSessionID, G_TYPE_LOG_RS_BK, G_CODE_COMMON_MULTI_BEGIN, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}


	do
	{
		m_tMutex.Lock();
		nOnceNum = t_ManageLogRsBk->SetPkt(SendToken, m_nSessionID, 1000);
		if(nOnceNum)
		{
			SendData_Link(m_nSessionID, G_TYPE_LOG_RS_BK, G_CODE_COMMON_MULTI_LOOP, SendToken);
			SendToken.Clear();
		}
		m_tMutex.UnLock();
		if(nOnceNum)
		{
			nSendedNum += nOnceNum;
			WriteLogN("[%s] send init data loop [%d/%d]", m_strLogicName.c_str(), nSendedNum, nTotalNum);
		}
	}while(nSendedNum < nTotalNum);
	
	
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(nSendedNum);
		SendData_Link(m_nSessionID, G_TYPE_LOG_RS_BK, G_CODE_COMMON_MULTI_END, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicLogRsBk::AnalyzePkt_FromLink_Ext_Restore()
{
	if(RecvToken.TokenDel_IDList(m_tRecvIDList) < 0)	SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);	
	
	{
		TListIDItor begin, end;
		begin = m_tRecvIDList.begin();	end = m_tRecvIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_LOG_RS_BK pdata = t_ManageLogRsBk->FindItem(*begin);
			if(!pdata)	continue;

			if(SetER(t_DocBackupUtil->RecoveryFile(pdata)))
			{
				WriteLogE("[%s] restore fail : [%d][%s][%s]", m_strLogicName.c_str(), pdata->nID, pdata->strObjectPath.c_str(), pdata->strObjectName.c_str());
				continue;
			}

			m_tSendIDList.push_back(*begin);
		}
	}


	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(m_tSendIDList.size());
		SendData_Link(m_nSessionID, G_TYPE_LOG_RS_BK, G_CODE_COMMON_RESTORE, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------



