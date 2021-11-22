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
#include "ManagePoSchUnit.h"

//---------------------------------------------------------------------------

CManagePoSchUnit*	t_ManagePoSchUnit = NULL;

//---------------------------------------------------------------------------

CManagePoSchUnit::CManagePoSchUnit()
{
	t_DBMgrPoSchUnit	= new CDBMgrPoSchUnit();
}
//---------------------------------------------------------------------------

CManagePoSchUnit::~CManagePoSchUnit()
{
	if(t_DBMgrPoSchUnit)	{	delete t_DBMgrPoSchUnit;	t_DBMgrPoSchUnit = NULL;	}
}
//---------------------------------------------------------------------------
//----------------------------------------------------------------------

INT32		CManagePoSchUnit::LoadDBMS()
{
	TListDBPoSchUnit tDBPoSchUnitList;
	TListDBPoSchUnitItor begin, end;
	if(SetER(t_DBMgrPoSchUnit->LoadExecute(&tDBPoSchUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoSchUnitList.begin();	end = tDBPoSchUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_SCH_UNIT pdphpsu 			= NULL;
	{
		if( (pdphpsu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_po_sch_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strPolicyInfo;
		MapToHex_64(pdphpsu->tPoInfoMap, strPolicyInfo, 1);

		String strItemHash;
		strItemHash = SPrintf("%s,"
			"%llu,%s,"
			"%u,%u,", 
			GetHdrHashInfo(pdphpsu).c_str(),
			pdphpsu->nPackageType, strPolicyInfo.c_str(), 
			pdphpsu->nStDate, pdphpsu->nEdDate);

		strOrgValue += strItemHash;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::AddPoSchUnit(DB_PO_SCH_UNIT&	dphpsu)
{
	if(SetER(t_DBMgrPoSchUnit->InsertExecute(&dphpsu)))
	{
		return g_nErrRtn;
	}

	AddItem(dphpsu.tDPH.nID, dphpsu);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::EditPoSchUnit(DB_PO_SCH_UNIT&	dphpsu)
{
	PDB_PO_SCH_UNIT pdphpsu = FindItem(dphpsu.tDPH.nID);
	if(!pdphpsu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoSchUnit->UpdateExecute(&dphpsu)))
	{
		return g_nErrRtn;
	}

	EditItem(dphpsu.tDPH.nID, dphpsu);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::DelPoSchUnit(UINT32 nID)
{
	PDB_PO_SCH_UNIT pdphpsu = FindItem(nID);
	if(!pdphpsu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoSchUnit->DeleteExecute(pdphpsu->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::ApplyPoSchUnit(DB_PO_SCH_UNIT&	dphpsu)
{
	if(FindItem(dphpsu.tDPH.nID))
	{
		return EditPoSchUnit(dphpsu);
	}
	return AddPoSchUnit(dphpsu);
}
//---------------------------------------------------------------------------

String					CManagePoSchUnit::GetName(UINT32 nID)
{
	PDB_PO_SCH_UNIT pdphpsu = FindItem(nID);
	if(!pdphpsu)	return "";
	return pdphpsu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoSchUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_SCH_UNIT pdphpsu = FindItem(nID);
	if(!pdphpsu)	return -1;

	return SetPkt(pdphpsu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::SetPkt(PDB_PO_SCH_UNIT pdphpsu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphpsu->tDPH);

	SendToken.TokenAdd_64(pdphpsu->nPackageType);
	SendToken.TokenAdd_ID64Map(pdphpsu->tPoInfoMap);
	SendToken.TokenAdd_32(pdphpsu->nStDate);
	SendToken.TokenAdd_32(pdphpsu->nEdDate);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoSchUnit::GetPkt(MemToken& RecvToken, DB_PO_SCH_UNIT& dphpsu)
{
	if (!RecvToken.TokenDel_DPH(dphpsu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(dphpsu.nPackageType))			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(dphpsu.tPoInfoMap) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphpsu.nStDate))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphpsu.nEdDate))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



