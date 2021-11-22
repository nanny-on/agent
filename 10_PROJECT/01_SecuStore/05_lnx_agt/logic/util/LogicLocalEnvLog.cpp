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

#include "stdafx.h"
#include "com_struct.h"
#include "LogicLocalEnvLog.h"

//---------------------------------------------------------------------------

CLogicLocalEnvLog*	t_LogicLocalEnvLog = NULL;

CLogicLocalEnvLog::CLogicLocalEnvLog()
{
	t_ManageLocalEnvLog			= new CManageLocalEnvLog();
	t_ManageLocalEnvLog->LoadDBMS();

	m_strLogicName		= "logic event handler";
}
//---------------------------------------------------------------------------

CLogicLocalEnvLog::~CLogicLocalEnvLog()
{
	SAFE_DELETE(t_ManageLocalEnvLog);
}
//---------------------------------------------------------------------------

INT32		CLogicLocalEnvLog::ResetDocLogID()
{
	INT32 nLastID = 0;
	TListID tIDList;
	TListIDItor begin, end;
	t_ManageLogDoc->GetItemIDList(tIDList);
	for(begin; begin != end; begin++)
	{
		PDB_LOG_DOC pdld = t_ManageLogDoc->FindItem(*begin);
		if(!pdld)	continue;

		nLastID = *begin;

		if(pdld->nRemoveTime)		continue;

		break;
	}	

	if(nLastID)
	{
		PDB_LOCAL_ENV_LOG pdlel = t_ManageLocalEnvLog->FirstItem();
		if(!pdlel)	return 0;

		pdlel->nLogDocID = nLastID;
		t_ManageLocalEnvLog->EditLocalEnvLog(*pdlel);
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------