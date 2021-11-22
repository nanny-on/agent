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
#include "ManagePoFeExcludeUnit.h"

//---------------------------------------------------------------------------

CManagePoFeExcludeUnit*	t_ManagePoFeExcludeUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFeExcludeUnit::CManagePoFeExcludeUnit()
{
	t_DBMgrPoFeExcludeUnit	= new CDBMgrPoFeExcludeUnit();
}
//---------------------------------------------------------------------------

CManagePoFeExcludeUnit::~CManagePoFeExcludeUnit()
{
	if(t_DBMgrPoFeExcludeUnit)	{	delete t_DBMgrPoFeExcludeUnit;	t_DBMgrPoFeExcludeUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFeExcludeUnit::LoadDBMS()
{
	TListDBPoFeExcludeUnit tDBPoFeExcludeUnitList;
    TListDBPoFeExcludeUnitItor begin, end;
	if(SetER(t_DBMgrPoFeExcludeUnit->LoadExecute(&tDBPoFeExcludeUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFeExcludeUnitList.begin();	end = tDBPoFeExcludeUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FE_EXCLUDE_UNIT pdpfeu 			= NULL;
	{
		if( (pdpfeu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_exclude_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,"
							"%s,%s,%s,%s,"
							"%s,%s,"
							"%s,", 
							GetHdrHashInfo(pdpfeu).c_str(),
							pdpfeu->nUnitType, pdpfeu->nBlockMode, pdpfeu->nChkLevel,
							pdpfeu->strFilePath.c_str(), pdpfeu->strFileComp.c_str(), pdpfeu->strFileDesc.c_str(), pdpfeu->strFileHash.c_str(), 
							pdpfeu->strFileSign.c_str(), pdpfeu->strFileSignComp.c_str(),
							pdpfeu->strUserName.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::AddPoFeExcludeUnit(DB_PO_FE_EXCLUDE_UNIT&	dpfeu)
{
	if(SetER(t_DBMgrPoFeExcludeUnit->InsertExecute(&dpfeu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfeu.tDPH.nID, dpfeu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::EditPoFeExcludeUnit(DB_PO_FE_EXCLUDE_UNIT&	dpfeu)
{
	PDB_PO_FE_EXCLUDE_UNIT pdpfeu = FindItem(dpfeu.tDPH.nID);
	if(!pdpfeu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFeExcludeUnit->UpdateExecute(&dpfeu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfeu.tDPH.nID, dpfeu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::DelPoFeExcludeUnit(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE_UNIT pdpfeu = FindItem(nID);
	if(!pdpfeu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFeExcludeUnit->DeleteExecute(pdpfeu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::ApplyPoFeExcludeUnit(DB_PO_FE_EXCLUDE_UNIT&	dpfeu)
{
	if(FindItem(dpfeu.tDPH.nID))
	{
		return EditPoFeExcludeUnit(dpfeu);
	}
	return AddPoFeExcludeUnit(dpfeu);
}
//---------------------------------------------------------------------------

String					CManagePoFeExcludeUnit::GetName(UINT32 nID)
{
	PDB_PO_FE_EXCLUDE_UNIT pdpfeu = FindItem(nID);
    if(!pdpfeu)	return "";
    return pdpfeu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFeExcludeUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_EXCLUDE_UNIT pdpfeu = FindItem(nID);
	if(!pdpfeu)	return -1;

	return SetPkt(pdpfeu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::SetPkt(PDB_PO_FE_EXCLUDE_UNIT pdpfeu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfeu->tDPH);

	SendToken.TokenAdd_32(pdpfeu->nUnitType);
	SendToken.TokenAdd_32(pdpfeu->nBlockMode);
	SendToken.TokenAdd_32(pdpfeu->nChkLevel);
	SendToken.TokenAdd_String(pdpfeu->strFilePath);
	SendToken.TokenAdd_String(pdpfeu->strFileComp);
	SendToken.TokenAdd_String(pdpfeu->strFileDesc);
	SendToken.TokenAdd_String(pdpfeu->strFileHash);
	SendToken.TokenAdd_String(pdpfeu->strFileSign);
	SendToken.TokenAdd_String(pdpfeu->strFileSignComp);
	SendToken.TokenAdd_String(pdpfeu->strUserName);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFeExcludeUnit::GetPkt(MemToken& RecvToken, DB_PO_FE_EXCLUDE_UNIT& dpfeu)
{
	if (!RecvToken.TokenDel_DPH(dpfeu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfeu.nUnitType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfeu.nBlockMode))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfeu.nChkLevel))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFilePath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFileComp) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFileDesc) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFileHash) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFileSign) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFileSignComp) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strUserName) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



