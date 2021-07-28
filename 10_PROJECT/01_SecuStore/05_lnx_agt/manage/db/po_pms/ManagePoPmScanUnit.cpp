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
#include "ManagePoPmScanUnit.h"

//---------------------------------------------------------------------------

CManagePoPmScanUnit*	t_ManagePoPmScanUnit = NULL;

//---------------------------------------------------------------------------

CManagePoPmScanUnit::CManagePoPmScanUnit()
{
	t_DBMgrPoPmScanUnit	= new CDBMgrPoPmScanUnit();
}
//---------------------------------------------------------------------------

CManagePoPmScanUnit::~CManagePoPmScanUnit()
{
	if(t_DBMgrPoPmScanUnit)	{	delete t_DBMgrPoPmScanUnit;	t_DBMgrPoPmScanUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPmScanUnit::LoadDBMS()
{
	TListDBPoPmScanUnit tDBPoPmScanUnitList;
    TListDBPoPmScanUnitItor begin, end;
	if(SetER(t_DBMgrPoPmScanUnit->LoadExecute(&tDBPoPmScanUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPmScanUnitList.begin();	end = tDBPoPmScanUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_PM_SCAN_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_pm_scan_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%llu,%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nScanType, pdata->nScanTime, pdata->nScanSvrType);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::AddPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data)
{
	if(SetER(t_DBMgrPoPmScanUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::EditPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data)
{
	PDB_PO_PM_SCAN_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPmScanUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::DelPoPmScanUnit(UINT32 nID)
{
	PDB_PO_PM_SCAN_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPmScanUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::ApplyPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoPmScanUnit(data);
	}
	return AddPoPmScanUnit(data);
}
//---------------------------------------------------------------------------

PDB_PO_PM_SCAN_UNIT		CManagePoPmScanUnit::GetDefaultData()
{
	TMapDBPoPmScanUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tDPH.nUsedMode != STATUS_MODE_ON)		continue;

		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------

String					CManagePoPmScanUnit::GetName(UINT32 nID)
{
	PDB_PO_PM_SCAN_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPmScanUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_PM_SCAN_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::SetPkt(PDB_PO_PM_SCAN_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nScanType);
	SendToken.TokenAdd_64(pdata->nScanTime);
	SendToken.TokenAdd_32(pdata->nScanSvrType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmScanUnit::GetPkt(MemToken& RecvToken, DB_PO_PM_SCAN_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanType))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nScanTime))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nScanSvrType))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



