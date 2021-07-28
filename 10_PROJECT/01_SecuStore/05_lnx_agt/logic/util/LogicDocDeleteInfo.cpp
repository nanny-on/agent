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
#include "LogicDocDeleteInfo.h"

//---------------------------------------------------------------------------

CLogicDocDeleteInfo*	t_LogicDocDeleteInfo = NULL;

CLogicDocDeleteInfo::CLogicDocDeleteInfo()
{
	t_ManageDocDeleteInfo			= new CManageDocDeleteInfo();
	t_ManageDocDeleteInfo->LoadDBMS();

	m_strLogicName		= "logic doc delete info";
}
//---------------------------------------------------------------------------

CLogicDocDeleteInfo::~CLogicDocDeleteInfo()
{
	SAFE_DELETE(t_ManageDocDeleteInfo);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void	CLogicDocDeleteInfo::SendPkt_DocDeleteInfo_Edit()
{
	DB_DOC_DELETE_INFO data;
	t_ManageDocDeleteInfo->GetData(data);
	t_ManageDocDeleteInfo->SetPkt(&data, SendToken);
	SendData_Mgr(G_TYPE_HOST_STATUS, G_CODE_HOST_SET_DOC_DEL_INFO, SendToken);
	SendToken.Clear();
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------