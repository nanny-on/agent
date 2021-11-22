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
#include "ManagePoInVulnPatchExceptionUnit.h"

//---------------------------------------------------------------------------

CManagePoInVulnPatchExceptionUnit*	t_ManagePoInVulnPatchExceptionUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnPatchExceptionUnit::CManagePoInVulnPatchExceptionUnit()
{
	t_DBMgrPoInVulnPatchExceptionUnit	= new CDBMgrPoInVulnPatchExceptionUnit();
}
//---------------------------------------------------------------------------

CManagePoInVulnPatchExceptionUnit::~CManagePoInVulnPatchExceptionUnit()
{
	if(t_DBMgrPoInVulnPatchExceptionUnit)	{	delete t_DBMgrPoInVulnPatchExceptionUnit;	t_DBMgrPoInVulnPatchExceptionUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnPatchExceptionUnit::LoadDBMS()
{
	TListDBPoInVulnPatchExceptionUnit tDBPoInVulnPatchExceptionUnitList;
	TListDBPoInVulnPatchExceptionUnitItor begin, end;
	if(SetER(t_DBMgrPoInVulnPatchExceptionUnit->LoadExecute(&tDBPoInVulnPatchExceptionUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInVulnPatchExceptionUnitList.begin();	end = tDBPoInVulnPatchExceptionUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_patch_exception_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->strPatchInfo.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::AddPoInVulnPatchExceptionUnit(DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT&	data)
{
	if(SetER(t_DBMgrPoInVulnPatchExceptionUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::EditPoInVulnPatchExceptionUnit(DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT&	data)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnPatchExceptionUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::DelPoInVulnPatchExceptionUnit(UINT32 nID)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnPatchExceptionUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::ApplyPoInVulnPatchExceptionUnit(DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnPatchExceptionUnit(data);
	}
	return AddPoInVulnPatchExceptionUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnPatchExceptionUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnPatchExceptionUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::SetPkt(PDB_PO_IN_VULN_PATCH_EXCEPTION_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);
	SendToken.TokenAdd_String(pdata->strPatchInfo);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnPatchExceptionUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPatchInfo) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



