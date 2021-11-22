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
#include "ManageOsType.h"

//---------------------------------------------------------------------------

CManageOsType*	t_ManageOsType = NULL;

//---------------------------------------------------------------------------

CManageOsType::CManageOsType()
{
	t_DBMgrOsType	= new CDBMgrOsType();
}
//---------------------------------------------------------------------------

CManageOsType::~CManageOsType()
{
	if(t_DBMgrOsType)		{	delete t_DBMgrOsType;		t_DBMgrOsType = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageOsType::LoadDBMS()
{
	TListDBOsType tDBOsTypeList;
    TListDBOsTypeItor begin, end;
	if(SetER(t_DBMgrOsType->LoadExecute(&tDBOsTypeList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBOsTypeList.begin();	end = tDBOsTypeList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageOsType::IsServerType(UINT32 nID)
{
	PDB_OS_TYPE pdot = FindItem(nID);
    if(!pdot)	return 0;
    return pdot->nType;
}
//---------------------------------------------------------------------------   

String				CManageOsType::GetOsName(UINT32 nID)
{
	PDB_OS_TYPE pdot = FindItem(nID);
	if(!pdot)	return 0;

	return pdot->strName;
}
//---------------------------------------------------------------------------

String				CManageOsType::GetOsNameSP(UINT32 nID, UINT32 nSP)
{
	String strName;
	if(nSP)		strName = SPrintf("%s[%d]", GetOsName(nID).c_str(), nSP);
	else		strName = GetOsName(nID);
	return strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32				CManageOsType::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBOsTypeItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32				CManageOsType::SetPkt(PDB_OS_TYPE pdot, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdot->nID);
    SendToken.TokenAdd_String(pdot->strName);
	SendToken.TokenAdd_32(pdot->nType);

	SendToken.TokenSet_Block();
    
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
