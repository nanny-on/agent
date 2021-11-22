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

// ThreadSocket.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"

#include "ThreadSocket.h"

//---------------------------------------------------------------------------

INT32		CThreadSocket::Logic_CltLgn(INT32& nTotalPktCnt, INT32 nOncePktNum)
{
	PKT_DATA pkt_data;
	CMainDlg* pMainDlg = (CMainDlg*)t_EnvInfoOp->GetMainHandle();
	CCltSocketUtil*	tCltSocketUtil = t_LgnCltUtil;

	if(pMainDlg == NULL || tCltSocketUtil == NULL)
	{
		WriteLogE("[Logic_CltLgn] invalid logic clt lgn class");
		return 1;
	}

	memset(&pkt_data, 0, sizeof(PKT_DATA));
	
	try
	{
		while(t_EnvInfo->m_nContinueOP && nOncePktNum && tCltSocketUtil->GetPkt_CLD(pkt_data) > 0)
		{
			nOncePktNum--;
			if(pkt_data.nIsSockEvent)
			{
				if(pkt_data.sock_evt.nEventID == ASI_SOCKET_EVENT_CONNECT)
				{
					WriteLogN("agt_lgn connected [%d][%s]", pkt_data.nSessionID, ConvertIP(pkt_data.sock_evt.nAddress).c_str());
				}
				else if(pkt_data.sock_evt.nEventID == ASI_SOCKET_EVENT_DISCONNECT)
				{
					WriteLogN("agt_lgn disconnected [%d][%s][%d]", pkt_data.nSessionID,  ConvertIP(pkt_data.sock_evt.nAddress).c_str(), pkt_data.sock_evt.nErrorCode);
				}
			}
			else
			{
				InitPktDbgTime("agt_lgn", &pkt_data);

				pMainDlg->OnSockEvtCltLgn(&pkt_data);

				tCltSocketUtil->FreeMemory_CLD(pkt_data.body.data);
				nTotalPktCnt++;
				FreePktDbgTime("agt_lgn", &pkt_data);
			}
		}
	}
	catch(...)
	{
		WriteLogE("pkt operation (agt_lgn) exception %d,%d", pkt_data.hdr.type, pkt_data.hdr.code);
	}
	return 1;
}
//---------------------------------------------------------------------------
