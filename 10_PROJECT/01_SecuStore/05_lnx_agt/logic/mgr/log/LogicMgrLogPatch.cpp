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
#include "LogicMgrLogPatch.h"

//---------------------------------------------------------------------------

CLogicMgrLogPatch*		t_LogicMgrLogPatch = NULL;

//---------------------------------------------------------------------------

CLogicMgrLogPatch::CLogicMgrLogPatch()
{		
	t_ManageLogPatch	= new CManageLogPatch();
	t_LogicLogPatch		= new CLogicLogPatch();

	t_ManageLogPatch->LoadDBMS();

	m_strLogicName		= "mgr log patch";

	m_nControlType		= SS_CONTROL_TYPE_LOG_PATCH;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_DEPLOY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_DEPLOY_PATCH;
}
//---------------------------------------------------------------------------

CLogicMgrLogPatch::~CLogicMgrLogPatch()
{
	SAFE_DELETE(t_LogicLogPatch);
	SAFE_DELETE(t_ManageLogPatch);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:				nRtn = AnalyzePkt_FromMgr_Ext_Sync();				break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::AnalyzePkt_FromMgr_Add_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::AnalyzePkt_FromMgr_Edit_Ext()
{
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::AnalyzePkt_FromMgr_Del_Ext()
{
	
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

INT32		CLogicMgrLogPatch::AnalyzePkt_FromMgr_Ext_Sync()
{	
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
//---------------------------------------------------------------------------

void		CLogicMgrLogPatch::SetLogPatch(DB_LOG_PATCH& data)
{
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

void		CLogicMgrLogPatch::SetLogPatch(TListDBLogPatch& tLogList)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::StartDownLoad(TListDBLogPatch& tLogList, UINT32 nPoUnitID)
{	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::AddDpDownInfoPatch(PDB_LOG_PATCH pdata, UINT32 nPoUnitID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrLogPatch::ApplyExecuteEnd(UINT32 nWorkID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32	CLogicMgrLogPatch::IsWaitBootByPatch(UINT32 nID)
{
	return 0;
}
//---------------------------------------------------------------------------

