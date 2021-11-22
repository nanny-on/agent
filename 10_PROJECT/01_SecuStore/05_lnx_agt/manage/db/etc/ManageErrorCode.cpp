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
#include "ManageErrorCode.h"

//---------------------------------------------------------------------------

CManageErrorCode*	t_ManageErrorCode = NULL;

//---------------------------------------------------------------------------

CManageErrorCode::CManageErrorCode()
{
	t_DBMgrErrorCode	= new CDBMgrErrorCode();    
}
//---------------------------------------------------------------------------

CManageErrorCode::~CManageErrorCode()
{
	if(t_DBMgrErrorCode)
	{
		delete t_DBMgrErrorCode;
		t_DBMgrErrorCode = NULL;
	}
}
//---------------------------------------------------------------------------

INT32		CManageErrorCode::LoadDBMS()
{
	TListDBErrorCode tDBErrorCodeList;
    TListDBErrorCodeItor begin, end;
	if(SetER(t_DBMgrErrorCode->LoadExecute(&tDBErrorCodeList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBErrorCodeList.begin();	end = tDBErrorCodeList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageErrorCode::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBErrorCodeItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageErrorCode::SetPkt(PDB_ERROR_CODE pdee, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdee->nID);
    SendToken.TokenAdd_String(pdee->lan_kor.strType);
    SendToken.TokenAdd_String(pdee->lan_kor.strDescr);
    SendToken.TokenAdd_String(pdee->lan_eng.strType);
	SendToken.TokenAdd_String(pdee->lan_eng.strDescr);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------


