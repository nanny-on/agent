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
#include "ManagePoInVulnAxUnit.h"

//---------------------------------------------------------------------------

CManagePoInVulnAxUnit*	t_ManagePoInVulnAxUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnAxUnit::CManagePoInVulnAxUnit()
{
	t_DBMgrPoInVulnAxUnit	= new CDBMgrPoInVulnAxUnit();
}
//---------------------------------------------------------------------------

CManagePoInVulnAxUnit::~CManagePoInVulnAxUnit()
{
	if(t_DBMgrPoInVulnAxUnit)	{	delete t_DBMgrPoInVulnAxUnit;	t_DBMgrPoInVulnAxUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnAxUnit::LoadDBMS()
{
	TListDBPoInVulnAxUnit tDBPoInVulnAxUnitList;
	TListDBPoInVulnAxUnitItor begin, end;
	if(SetER(t_DBMgrPoInVulnAxUnit->LoadExecute(&tDBPoInVulnAxUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInVulnAxUnitList.begin();	end = tDBPoInVulnAxUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_AX_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_ax_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,%s,%s,%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nFilter, pdata->strName.c_str(), pdata->strClsId.c_str(), pdata->strDownloadedSite.c_str(), pdata->strPublisher.c_str(), pdata->strFileName.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::AddPoInVulnAxUnit(DB_PO_IN_VULN_AX_UNIT&	data)
{
	if(SetER(t_DBMgrPoInVulnAxUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::EditPoInVulnAxUnit(DB_PO_IN_VULN_AX_UNIT&	data)
{
	PDB_PO_IN_VULN_AX_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnAxUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::DelPoInVulnAxUnit(UINT32 nID)
{
	PDB_PO_IN_VULN_AX_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnAxUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::ApplyPoInVulnAxUnit(DB_PO_IN_VULN_AX_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnAxUnit(data);
	}
	return AddPoInVulnAxUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnAxUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_AX_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnAxUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_AX_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::SetPkt(PDB_PO_IN_VULN_AX_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nFilter);
	SendToken.TokenAdd_String(pdata->strName);
	SendToken.TokenAdd_String(pdata->strClsId);
	SendToken.TokenAdd_String(pdata->strDownloadedSite);
	SendToken.TokenAdd_String(pdata->strPublisher);
	SendToken.TokenAdd_String(pdata->strFileName);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnAxUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_AX_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nFilter))						goto	INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strName) < 0)				goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strClsId) < 0)				goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strDownloadedSite) < 0)		goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strPublisher) < 0)			goto INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strFileName) < 0)			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



