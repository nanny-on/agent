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
#include "ManagePoInVulnQnaUnit.h"

//---------------------------------------------------------------------------

CManagePoInVulnQnaUnit*	t_ManagePoInVulnQnaUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnQnaUnit::CManagePoInVulnQnaUnit()
{
	t_DBMgrPoInVulnQnaUnit	= new CDBMgrPoInVulnQnaUnit();
}
//---------------------------------------------------------------------------

CManagePoInVulnQnaUnit::~CManagePoInVulnQnaUnit()
{
	if(t_DBMgrPoInVulnQnaUnit)	{	delete t_DBMgrPoInVulnQnaUnit;	t_DBMgrPoInVulnQnaUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnQnaUnit::LoadDBMS()
{
	TListDBPoInVulnQnaUnit tDBPoInVulnQnaUnitList;
	TListDBPoInVulnQnaUnitItor begin, end;
	if(SetER(t_DBMgrPoInVulnQnaUnit->LoadExecute(&tDBPoInVulnQnaUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInVulnQnaUnitList.begin();	end = tDBPoInVulnQnaUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_QNA_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_qna_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->strQuestion.c_str(), pdata->strAnswer.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::AddPoInVulnQnaUnit(DB_PO_IN_VULN_QNA_UNIT&	data)
{
	if(SetER(t_DBMgrPoInVulnQnaUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::EditPoInVulnQnaUnit(DB_PO_IN_VULN_QNA_UNIT&	data)
{
	PDB_PO_IN_VULN_QNA_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnQnaUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::DelPoInVulnQnaUnit(UINT32 nID)
{
	PDB_PO_IN_VULN_QNA_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnQnaUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::ApplyPoInVulnQnaUnit(DB_PO_IN_VULN_QNA_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnQnaUnit(data);
	}
	return AddPoInVulnQnaUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnQnaUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_QNA_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnQnaUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_QNA_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::SetPkt(PDB_PO_IN_VULN_QNA_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->strQuestion);
	SendToken.TokenAdd_String(pdata->strAnswer);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnQnaUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_QNA_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (RecvToken.TokenDel_String(data.strQuestion)<0)				goto	INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strAnswer)<0)				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



