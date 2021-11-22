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
#include "ManagePoInVulnEditAppUnit.h"

//---------------------------------------------------------------------------

CManagePoInVulnEditAppUnit*	t_ManagePoInVulnEditAppUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnEditAppUnit::CManagePoInVulnEditAppUnit()
{
	t_DBMgrPoInVulnEditAppUnit	= new CDBMgrPoInVulnEditAppUnit();
}
//---------------------------------------------------------------------------

CManagePoInVulnEditAppUnit::~CManagePoInVulnEditAppUnit()
{
	if(t_DBMgrPoInVulnEditAppUnit)	{	delete t_DBMgrPoInVulnEditAppUnit;	t_DBMgrPoInVulnEditAppUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnEditAppUnit::LoadDBMS()
{
	TListDBPoInVulnEditAppUnit tDBPoInVulnEditAppUnitList;
	TListDBPoInVulnEditAppUnitItor begin, end;
	if(SetER(t_DBMgrPoInVulnEditAppUnit->LoadExecute(&tDBPoInVulnEditAppUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInVulnEditAppUnitList.begin();	end = tDBPoInVulnEditAppUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_edit_app_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->strClassName.c_str(), pdata->strName.c_str(), pdata->strRegKey.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::AddPoInVulnEditAppUnit(DB_PO_IN_VULN_EDIT_APP_UNIT&	data)
{
	if(SetER(t_DBMgrPoInVulnEditAppUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::EditPoInVulnEditAppUnit(DB_PO_IN_VULN_EDIT_APP_UNIT&	data)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnEditAppUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::DelPoInVulnEditAppUnit(UINT32 nID)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnEditAppUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::ApplyPoInVulnEditAppUnit(DB_PO_IN_VULN_EDIT_APP_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnEditAppUnit(data);
	}
	return AddPoInVulnEditAppUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnEditAppUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnEditAppUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_EDIT_APP_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::SetPkt(PDB_PO_IN_VULN_EDIT_APP_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->strClassName);
	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strRegKey);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnEditAppUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_EDIT_APP_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto INVALID_PKT;

	if (RecvToken.TokenDel_String(data.strClassName) < 0)			goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strName) < 0)				goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strRegKey) < 0)				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



