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
#include "LogicMgrHostHw.h"

//---------------------------------------------------------------------------

CLogicMgrHostHw*		t_LogicMgrHostHw = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostHw::CLogicMgrHostHw()
{
	t_ManageHostHw		= new CManageHostHw();
	
	t_ManageHostHw->LoadDBMS();

	m_strLogicName = "logic mgr host hw";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostHw::~CLogicMgrHostHw()
{
	SAFE_DELETE(t_ManageHostHw);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostHw::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
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

INT32		CLogicMgrHostHw::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_HW pdhh = NULL;
	DB_HOST_HW dhh;

	if( t_ManageHostHw->GetPktHost(RecvToken, dhh))		return AZPKT_CB_RTN_PKT_INVALID;

	if( (pdhh = t_ManageHostHw->FirstItem()) == NULL)
	{
		WriteLogE("[%s] not find host info", m_strLogicName.c_str());
		return AZPKT_CB_RTN_DBMS_FAIL;
	}

	if((pdhh->nPCType != dhh.nPCType ||
		pdhh->strCPUName != dhh.strCPUName ||
		pdhh->nMemSize != dhh.nMemSize ||
		pdhh->nHDDSize != dhh.nHDDSize ||
		pdhh->nLanNum != dhh.nLanNum ||
		pdhh->strBiosDate.compare(dhh.strBiosDate)) == FALSE)
	{
		return AZPKT_CB_RTN_SUCCESS_END;
	}

	pdhh->nPCType = dhh.nPCType;
	pdhh->strCPUName = dhh.strCPUName;
	pdhh->nMemSize = dhh.nMemSize;
	pdhh->nHDDSize = dhh.nHDDSize;
	pdhh->nLanNum = dhh.nLanNum;
	pdhh->strBiosDate = dhh.strBiosDate;

	if(SetER(t_ManageHostHw->EditHostHw(*pdhh)))
	{
		WriteLogE("[%s] edit host hw information fail : [%u]", m_strLogicName.c_str(), pdhh->nID);
		return AZPKT_CB_RTN_DBMS_FAIL;
	}

	{
		SendToken.TokenAdd_32(ERR_SUCCESS);	
		t_ManageHostHw->SetPktHost(pdhh, SendToken);

		SendData_Link(G_TYPE_HOST_HW, G_CODE_COMMON_EDIT, SendToken);
		SendToken.Clear();
	}
	return AZPKT_CB_RTN_SUCCESS_END;
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

void		CLogicMgrHostHw::SendPkt_HostHw(BOOL bCheck)
{
	INT32 nSendPkt = 1;
	DB_HOST_HW dhh;

	{				
		CHAR szBuf[CHAR_MAX_SIZE] = {0, };
		UINT64 nSize = 0;
		{
			if(get_cpu_type(szBuf, CHAR_MAX_SIZE) == 0)
				dhh.strCPUName = szBuf;
			else
				dhh.strCPUName = "";
			ZeroMemoryExt(szBuf);
		}

		{
			nSize = get_total_disk_size();
			if(nSize > 0)
				dhh.nHDDSize = UINT32(nSize);
			else
				dhh.nHDDSize = 0;
		}

		{
			nSize = get_total_mem();
			if(nSize > 0)
				dhh.nMemSize = UINT32(nSize);
			else
				dhh.nMemSize = 0;
		}

		{
			if(get_bios_date(szBuf, CHAR_MAX_SIZE) == 0)
				dhh.strBiosDate = szBuf;
			else
				dhh.strBiosDate = "";
			memset(szBuf, 0, CHAR_MAX_SIZE);
		}
	}
	
	PDB_HOST_HW pdhh = t_ManageHostHw->FirstItem();
	if(!pdhh)
	{
		WriteLogE("[%s] not find host hw first item..", m_strLogicName.c_str());
		goto SEND_PKT;
	}


	if(bCheck)
	{
		do 
		{
			if( pdhh->strCPUName != dhh.strCPUName ||
				pdhh->nHDDSize != dhh.nHDDSize ||
				pdhh->nMemSize != dhh.nMemSize ||
				pdhh->strBiosDate != dhh.strBiosDate)
			{
				//WriteLogN("[%s] -> changed [%s][%d][%d][%s].", m_strLogicName.c_str(), pdhh->strCPUName, dhh.nHDDSize, dhh.nMemSize, dhh.strBiosDate);
				break;
			}

			nSendPkt = 0;
		} while (FALSE);

		if(!nSendPkt)	
		{
			//WriteLogN("[%s] -> [%s][%d][%d][%s].", m_strLogicName.c_str(), pdhh->strCPUName, dhh.nHDDSize, dhh.nMemSize, dhh.strBiosDate);
			return;
		}
	}
SEND_PKT:
	SendToken.Clear();
	t_ManageHostHw->SetPkt(&dhh, SendToken);
	SendData_Mgr(G_TYPE_HOST_HW, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();

	return;
}
//---------------------------------------------------------------------------
