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
#include "ManageEnvLocal.h"

//---------------------------------------------------------------------------

CManageEnvLocal*	t_ManageEnvLocal = NULL;

//---------------------------------------------------------------------------

CManageEnvLocal::CManageEnvLocal()
{
	t_DBMgrEnvLocal	= new CDBMgrEnvLocal();
}
//---------------------------------------------------------------------------

CManageEnvLocal::~CManageEnvLocal()
{
	if(t_DBMgrEnvLocal)	{	delete t_DBMgrEnvLocal;	t_DBMgrEnvLocal = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvLocal::LoadDBMS()
{
	TListDBEnvLocal tDBEnvLocalList;
	TListDBEnvLocalItor begin, end;
	if(SetER(t_DBMgrEnvLocal->LoadExecute(&tDBEnvLocalList)))
    {
    	return g_nErrRtn;
    }
	
	begin = tDBEnvLocalList.begin();	end = tDBEnvLocalList.end();
	for(begin; begin != end; begin++)
	{		
		AddItem(*begin);
		AddKeyTypeID(begin->nChkType, begin->nChkCode, begin->nID);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLocal::AddEnvLocal(DB_ENV_LOCAL&	data)
{
	if(SetER(t_DBMgrEnvLocal->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data);
	AddKeyTypeID(data.nChkType, data.nChkCode, data.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLocal::EditEnvLocal(DB_ENV_LOCAL&	data)
{
	PDB_ENV_LOCAL pdata = FindItem(data.nID);
	if(!pdata)	return ERR_INFO_NOT_EDIT_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrEnvLocal->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    *pdata = data;
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLocal::DelEnvLocal(UINT32 nID)
{
	PDB_ENV_LOCAL pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrEnvLocal->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyTypeID(pdata->nChkType, pdata->nChkCode);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
			
PDB_ENV_LOCAL			CManageEnvLocal::GetEnvLocal(UINT32 nChkType, UINT32 nChkCode)
{
	PDB_ENV_LOCAL pdata = FindKeyTypeID_Item(nChkType, nChkCode);
	if(pdata)	return pdata;

	DB_ENV_LOCAL data;
	{
		data.nChkType	= nChkType;
		data.nChkCode	= nChkCode;
		data.nRegDate	= GetCurrentDateTimeInt();
	}
	AddEnvLocal(data);
	pdata = FindKeyTypeID_Item(nChkType, nChkCode);

	return pdata;
}
//---------------------------------------------------------------------------

void					CManageEnvLocal::SetEnvLocalNumber(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey, UINT32 nValue)
{
	PDB_ENV_LOCAL pdata = GetEnvLocal(nChkType, nChkCode);
	if(!pdata)	return;

	pdata->tNValueMap[nKey] = nValue;
	EditEnvLocal(*pdata);
	return;
}
//---------------------------------------------------------------------------

UINT32					CManageEnvLocal::GetEnvLocalNumber(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey)
{

	PDB_ENV_LOCAL pdata = GetEnvLocal(nChkType, nChkCode);
	if(!pdata)	return 0;

	TMapIDItor find = pdata->tNValueMap.find(nKey);
	if(find == pdata->tNValueMap.end())		return 0;

	return find->second;
}
//---------------------------------------------------------------------------

void					CManageEnvLocal::SetEnvLocalString(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey, String strValue)
{
	PDB_ENV_LOCAL pdata = GetEnvLocal(nChkType, nChkCode);
	if(!pdata)	return;

	pdata->tSValueMap[nKey] = strValue;
	EditEnvLocal(*pdata);
	return;
}
//---------------------------------------------------------------------------

String					CManageEnvLocal::GetEnvLocalString(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey)
{

	PDB_ENV_LOCAL pdata = GetEnvLocal(nChkType, nChkCode);
	if(!pdata)	return 0;

	TMapIDStrItor find = pdata->tSValueMap.find(nKey);
	if(find == pdata->tSValueMap.end())		return 0;

	return find->second;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLocal::IsExistLargeThenValue(UINT32 nChkType, UINT32 nChkCode, UINT32 nValue)
{
	PDB_ENV_LOCAL pdata = GetEnvLocal(nChkType, nChkCode);
	if(!pdata)	return 0;
	
	
	TMapIDItor begin, end;
	begin = pdata->tNValueMap.begin();	end = pdata->tNValueMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second <= nValue)		continue;

		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------




