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
#include "LogicLogEvent.h"

//---------------------------------------------------------------------------

CLogicLogEvent*		t_LogicLogEvent = NULL;

//---------------------------------------------------------------------------

CLogicLogEvent::CLogicLogEvent()
{
	m_strLogicName = "logic log event";
}
//---------------------------------------------------------------------------

CLogicLogEvent::~CLogicLogEvent()
{	
}
//---------------------------------------------------------------------------

void		CLogicLogEvent::AnalyzePkt_LogEvent(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
    {		
		case G_CODE_COMMON_SYNC:		AnalyzePkt_LogEvent_Ext_Sync();					break;
        default:
        {
        	WriteLogE("[%s] not define pkt code from console : [%d][%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode);
            break;
        }
    }
	InitBaseMember();
    return;
}
//---------------------------------------------------------------------------

void		CLogicLogEvent::AnalyzePkt_LogEvent_Ext_Sync()
{
	TListDBLogEvent	tDBLogEventList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))					goto INVALID_PKT;
	while(m_nRecvNum--)
	{
		DB_LOG_EVENT	data;
		if(t_ManageLogEvent->GetPkt(RecvToken, data))		goto INVALID_PKT;

		tDBLogEventList.push_back(data);
	}

	t_LogicMgrLogEvent->SetLogEvent(tDBLogEventList);
	goto OP_END;

INVALID_PKT:	
	WriteLogE("[%s] recv invalid pkt from mgr : [%s][%x]:[%x]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str(), m_nPktCode, m_nDbgPos);
	goto OP_END;

OP_END:
	return;
}



