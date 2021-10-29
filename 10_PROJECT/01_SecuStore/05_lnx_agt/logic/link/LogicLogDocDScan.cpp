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
#include "LogicLogDocDScan.h"

//---------------------------------------------------------------------------

CLogicLogDocDScan*		t_LogicLogDocDScan = NULL;

//---------------------------------------------------------------------------

CLogicLogDocDScan::CLogicLogDocDScan()
{
	t_ManageLogDocDScan = new CManageLogDocDScan();

	t_ManageLogDocDScan->LoadDBMS();

	m_strLogicName		= "link log doc dscan";
		
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicLogDocDScan::~CLogicLogDocDScan()
{
	SAFE_DELETE(t_ManageLogDocDScan);
}
//---------------------------------------------------------------------------

INT32	CLogicLogDocDScan::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_DEL:			AnalyzePkt_FromLink_Ext_Del();			break;
		case G_CODE_COMMON_SCAN:		AnalyzePkt_FromLink_Ext_Scan();			break;
		default:								break;		
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicLogDocDScan::AnalyzePkt_FromLink_Ext_Scan()
{
	DB_LOG_DOC_DSCAN dldd;
	DB_PO_FA_CLEAR_UNIT tDPFCU;
	MEM_FIND_ORDER_INFO tMFOI;

	if(t_ManageLogDocDScan->GetPkt(RecvToken, dldd))
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(ERR_SOCKET_CLT_TO_SVR_INVALID_PKT);
		SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_SCAN, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogE("[%s] recv invalid pkt from kernel : [%s][%x]:[%x]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str(), m_nPktCode, m_nDbgPos);
		return 0;
	}
	{
		dldd.nRegDate = GetCurrentDateTimeInt();
		if(SetER(t_ManageLogDocDScan->AddLogDocDScan(dldd)))
		{
			SetDLEH_EC(g_nErrRtn);
			m_tMutex.Lock();
			SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_SCAN, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
			WriteLogE("[%s] doc demand scan add fail : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return 0;
		}
		
		{
			{
				tDPFCU.nScanType	= SS_PO_FA_CLEAR_SCAN_TYPE_DEMAND;
				tDPFCU.nFilePtnType = SS_PO_FA_CLEAR_PTN_TYPE_LOCAL;

				//tDPFCU.tDPH.nExtOption |= SS_PO_FA_CLEAR_UNIT_OPTION_FLAG_SCAN_DOC_FILE_FMT;
				tDPFCU.tDPH.nExtOption |= SS_PO_FA_CLEAR_UNIT_OPTION_FLAG_EXCLUDE_DEFAULT_PATH;				

				tDPFCU.strExPath		= dldd.strScanPath;
				tDPFCU.strFilePtnLocal	= dldd.strScanPtn;
			}

			{
				tMFOI.nEvtTime		= GetCurrentDateTimeInt();
				tMFOI.nOpType		= SS_LOG_DOC_OP_TYPE_SCAN;
				tMFOI.nNextOp		= G_CODE_COMMON_SCAN;
				tMFOI.nNotiPg		= STATUS_USED_MODE_ON;
			}			
		}

		{
			if(t_LogicMgrPoFaClear->ApplyPolicy_Unit(&tDPFCU, tMFOI))
			{
				m_tMutex.Lock();
				SendToken.TokenAdd_32(ERR_INFO_NOT_OP_BECAUSE_INVALID_SET_ENV);
				SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_SCAN, SendToken);
				SendToken.Clear();
				m_tMutex.UnLock();
				return 0;
			}
		}
		
		{
			m_tMutex.Lock();
			SendToken.TokenAdd_32(ERR_SUCCESS);
			SendToken.TokenAdd_32(tMFOI.nID);
			t_ManageLogDocDScan->SetPkt(&dldd, SendToken);		
			SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_SCAN, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicLogDocDScan::AnalyzePkt_FromLink_Ext_Del()
{

	INT32 nGroupID = 0;

	if(!RecvToken.TokenDel_32(nGroupID))	return AZPKT_CB_RTN_PKT_INVALID;
	INT32 nRet = TokenRecvIDList(RecvToken, m_tRecvIDList);
	if(nRet < 0)		goto INVALID_PKT;

	{
		//t_UserDeleteUtil->StartUserDelete(m_tRecvIDList, nGroupID);
	}

	goto OP_END;

INVALID_PKT:
	WriteLogE("[%s] recv invalid pkt from kernel : [%s][%x]:[%x]", m_strLogicName.c_str(), Int64ToHex(m_nPktType).c_str(), m_nPktCode, m_nDbgPos);

OP_END:
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicLogDocDScan::SendPkt_Find(PDB_LOG_DOC pdld)
{
	m_tMutex.Lock();
	SendToken.Set(1024);
	SendToken.TokenAdd_32(1);
	t_ManageLogDoc->SetPkt_Link(pdld, SendToken);
	SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_FIND, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
}
//---------------------------------------------------------------------------

void		CLogicLogDocDScan::SendPkt_Progress(PMEM_FIND_ORDER_INFO pMFOI)
{
	m_tMutex.Lock();
	SendToken.TokenAdd_32(pMFOI->nID);
	SendToken.TokenAdd_32(pMFOI->nNotiPgTotal);
	SendToken.TokenAdd_32(pMFOI->nNotiPgEnd);
	SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_PROGRESS, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------

void		CLogicLogDocDScan::SendPkt_End(MEM_FIND_ORDER_INFO& tMFOI)
{
	m_tMutex.Lock();
	SendToken.TokenAdd_32(tMFOI.nID);
	SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_END, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
	return;
}
//---------------------------------------------------------------------------

void		CLogicLogDocDScan::SendPkt_Del_Last(MEM_FIND_ORDER_INFO& tMFOI)
{
	m_tMutex.Lock();
	SendToken.TokenAdd_32(tMFOI.nPoID);
	SendToken.TokenAdd_32(tMFOI.nOpType);
	SendToken.TokenAdd_32(tMFOI.nNotiTotalFind);
	SendData_Link(G_TYPE_LOG_DOC_DSCAN, G_CODE_COMMON_LAST, SendToken);
	SendToken.Clear();
	m_tMutex.UnLock();
}
//---------------------------------------------------------------------------
