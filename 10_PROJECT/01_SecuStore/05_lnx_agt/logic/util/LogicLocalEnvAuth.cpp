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

#include "stdafx.h"
#include "com_struct.h"
#include "LogicLocalEnvAuth.h"

//---------------------------------------------------------------------------

CLogicLocalEnvAuth*	t_LogicLocalEnvAuth = NULL;

CLogicLocalEnvAuth::CLogicLocalEnvAuth()
{
	t_ManageLocalEnvAuth			= new CManageLocalEnvAuth();
	t_ManageLocalEnvAuth->LoadDBMS();

	m_strLogicName		= "logic local env auth";
}
//---------------------------------------------------------------------------

CLogicLocalEnvAuth::~CLogicLocalEnvAuth()
{
	SAFE_DELETE(t_ManageLocalEnvAuth);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void	CLogicLocalEnvAuth::SendPkt_LocalEnvAuth_Edit()
{
	t_ManageLocalEnvAuth->SetPkt(SendToken);
	SendData_Link(G_TYPE_LOCAL_ENV_AUTH, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------