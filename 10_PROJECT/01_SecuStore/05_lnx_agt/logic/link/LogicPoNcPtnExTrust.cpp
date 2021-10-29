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
#include "LogicPoNcPtnExTrust.h"

//---------------------------------------------------------------------------

CLogicPoNcPtnExTrust*		t_LogicPoNcPtnExTrust = NULL;

//---------------------------------------------------------------------------

CLogicPoNcPtnExTrust::CLogicPoNcPtnExTrust()
{	
	m_strLogicName = "logic po nc ptn ex trust";
}
//---------------------------------------------------------------------------

CLogicPoNcPtnExTrust::~CLogicPoNcPtnExTrust()
{
}
//---------------------------------------------------------------------------

INT32	CLogicPoNcPtnExTrust::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{		
		case G_CODE_COMMON_SYNC:		AnalyzePkt_FromLink_Ext_Sync();		break;
		default:						break;		
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPoNcPtnExTrust::AnalyzePkt_FromLink_Ext_Sync()
{
	SendPkt_Sync();

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

void		CLogicPoNcPtnExTrust::SendPkt_Sync()
{
	{
		m_tMutex.Lock();
		t_ManagePoNcPtnExTrust->SetPkt(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_PO_NC_PTN_EX_TRUST, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}
	return;
}
//---------------------------------------------------------------------------


