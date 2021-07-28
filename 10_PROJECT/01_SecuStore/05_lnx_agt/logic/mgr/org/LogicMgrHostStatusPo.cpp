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
#include "LogicMgrHostStatusPo.h"

//---------------------------------------------------------------------------

CLogicMgrHostStatusPo*		t_LogicMgrHostStatusPo = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostStatusPo::CLogicMgrHostStatusPo()
{
	t_ManageHostStatusPo		= new CManageHostStatusPo();
	
	t_ManageHostStatusPo->LoadDBMS();

	m_strLogicName = "logic mgr host status po";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostStatusPo::~CLogicMgrHostStatusPo()
{
	SAFE_DELETE(t_ManageHostStatusPo);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostStatusPo::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrHostStatusPo::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_STATUS_PO pdhsp = NULL;
	DB_HOST_STATUS_PO dhsp_old, dhsp;

	if( t_ManageHostStatusPo->GetPkt(RecvToken, dhsp))		return AZPKT_CB_RTN_PKT_INVALID;

	if((pdhsp = t_ManageHostStatusPo->FirstItem()))
	{
		dhsp_old = *pdhsp;	
	}

	if((dhsp_old.nFaODrv		== dhsp.nFaODrv &&
		dhsp_old.nFaCUnit		== dhsp.nFaCUnit &&
		dhsp_old.nDfRun			== dhsp.nDfRun &&
		dhsp_old.nDfUnit		== dhsp.nDfUnit &&
		dhsp_old.nFeOpDrv		== dhsp.nFeOpDrv &&
		dhsp_old.nFeLoDrv		== dhsp.nFeLoDrv &&
		dhsp_old.nFeLoRun		== dhsp.nFeLoRun &&
		dhsp_old.strFeLoPtn.compare(dhsp.strFeLoPtn) == 0 &&
		dhsp_old.nFeGbDrv		== dhsp.nFeGbDrv &&
		dhsp_old.nFeGbRun		== dhsp.nFeGbRun &&
		dhsp_old.strFeGbPtn.compare(dhsp.strFeGbPtn) == 0 &&
		dhsp_old.nFeGwDrv		== dhsp.nFeGwDrv &&
		dhsp_old.nFeGwRun		== dhsp.nFeGwRun &&
		dhsp_old.strFeGwPtn.compare(dhsp.strFeGwPtn) == 0))
	{
		return AZPKT_CB_RTN_SUCCESS_END;
	}

	dhsp_old.nHostID		= dhsp.nHostID;

	dhsp_old.nFaODrv		= dhsp.nFaODrv;
	dhsp_old.nFaCUnit		= dhsp.nFaCUnit;
	dhsp_old.nDfRun			= dhsp.nDfRun;
	dhsp_old.nDfUnit		= dhsp.nDfUnit;
	dhsp_old.nFeOpDrv		= dhsp.nFeOpDrv;
	dhsp_old.nFeLoDrv		= dhsp.nFeLoDrv;
	dhsp_old.nFeLoRun		= dhsp.nFeLoRun;
	dhsp_old.strFeLoPtn		= dhsp.strFeLoPtn;
	dhsp_old.nFeGbDrv		= dhsp.nFeGbDrv;
	dhsp_old.nFeGbRun		= dhsp.nFeGbRun;
	dhsp_old.strFeGbPtn		= dhsp.strFeGbPtn;
	dhsp_old.nFeGwDrv		= dhsp.nFeGwDrv;
	dhsp_old.nFeGwRun		= dhsp.nFeGwRun;
	dhsp_old.strFeGwPtn		= dhsp.strFeGwPtn;

	if(SetER(t_ManageHostStatusPo->ApplyHostStatusPo(dhsp_old)))
	{
		WriteLogE("[%s] edit host status po information fail : [%u]", m_strLogicName.c_str(), dhsp_old.nID);
		return AZPKT_CB_RTN_DBMS_FAIL;
	}

	{
		SendToken.TokenAdd_32(ERR_SUCCESS);	
		t_ManageHostStatusPo->SetPkt(&dhsp_old, SendToken);

		SendData_Link(m_nPktType, G_CODE_COMMON_EDIT, SendToken);
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

void		CLogicMgrHostStatusPo::SendPkt_HostStatusPo(DB_HOST_STATUS_PO dhsp)
{
	INT32 nSendPkt = 1;	
	
	DB_HOST_STATUS_PO dhsp_old;
	PDB_HOST_STATUS_PO pdhsp = t_ManageHostStatusPo->FirstItem();
	if(pdhsp)
	{
		dhsp_old = *pdhsp;
	}	

	do 
	{
		if((dhsp_old.nFaODrv	!= dhsp.nFaODrv ||
			dhsp_old.nFaCUnit	!= dhsp.nFaCUnit ||
			dhsp_old.nDfRun		!= dhsp.nDfRun ||
			dhsp_old.nDfUnit	!= dhsp.nDfUnit ||
			dhsp_old.nFeOpDrv	!= dhsp.nFeOpDrv ||
			dhsp_old.nFeLoDrv	!= dhsp.nFeLoDrv ||
			dhsp_old.nFeLoRun	!= dhsp.nFeLoRun ||
			dhsp_old.strFeLoPtn.compare(dhsp.strFeLoPtn) ||
			dhsp_old.nFeGbDrv	!= dhsp.nFeGbDrv ||
			dhsp_old.nFeGbRun	!= dhsp.nFeGbRun ||
			dhsp_old.strFeGbPtn.compare(dhsp.strFeGbPtn) ||
			dhsp_old.nFeGwDrv	!= dhsp.nFeGwDrv ||
			dhsp_old.nFeGwRun	!= dhsp.nFeGwRun ||
			dhsp_old.strFeGwPtn.compare(dhsp.strFeGwPtn)))
		{
			break;
		}

		nSendPkt = 0;
	} while (FALSE);

	if(!nSendPkt)	return;


	SendToken.Clear();
	t_ManageHostStatusPo->SetPkt(&dhsp, SendToken);
	SendData_Mgr(G_TYPE_HOST_STATUS_PO, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();

	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrHostStatusPo::SendPkt_HostStatusPo()
{
	PDB_HOST_STATUS_PO pdhsp = t_ManageHostStatusPo->FirstItem();
	if(!pdhsp)
	{
		WriteLogE("[%s] not find host status po first item", m_strLogicName.c_str());
		return;
	}	

	SendToken.Clear();
	t_ManageHostStatusPo->SetPkt(pdhsp, SendToken);
	SendData_Mgr(G_TYPE_HOST_STATUS_PO, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();

	return;
}
//---------------------------------------------------------------------------
