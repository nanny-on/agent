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
#include "ManagePoFaEnvUnit.h"

//---------------------------------------------------------------------------

CManagePoFaEnvUnit*	t_ManagePoFaEnvUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaEnvUnit::CManagePoFaEnvUnit()
{
	t_DBMgrPoFaEnvUnit	= new CDBMgrPoFaEnvUnit();
}
//---------------------------------------------------------------------------

CManagePoFaEnvUnit::~CManagePoFaEnvUnit()
{
	if(t_DBMgrPoFaEnvUnit)	{	delete t_DBMgrPoFaEnvUnit;	t_DBMgrPoFaEnvUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaEnvUnit::LoadDBMS()
{
	TListDBPoFaEnvUnit tDBPoFaEnvUnitList;
    TListDBPoFaEnvUnitItor begin, end;
	if(SetER(t_DBMgrPoFaEnvUnit->LoadExecute(&tDBPoFaEnvUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaEnvUnitList.begin();	end = tDBPoFaEnvUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_ENV_UNIT pdpfeu 			= NULL;
	{
		if( (pdpfeu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_env_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,", 
							GetHdrHashInfo(pdpfeu).c_str(),
							pdpfeu->nFilePtnType, pdpfeu->strFilePtnGlobal.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::AddPoFaEnvUnit(DB_PO_FA_ENV_UNIT&	dpfeu)
{
	if(SetER(t_DBMgrPoFaEnvUnit->InsertExecute(&dpfeu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfeu.tDPH.nID, dpfeu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::EditPoFaEnvUnit(DB_PO_FA_ENV_UNIT&	dpfeu)
{
	PDB_PO_FA_ENV_UNIT pdpfeu = FindItem(dpfeu.tDPH.nID);
	if(!pdpfeu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaEnvUnit->UpdateExecute(&dpfeu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfeu.tDPH.nID, dpfeu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::DelPoFaEnvUnit(UINT32 nID)
{
	PDB_PO_FA_ENV_UNIT pdpfeu = FindItem(nID);
	if(!pdpfeu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaEnvUnit->DeleteExecute(pdpfeu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaEnvUnit::ClearPoFaEnvUnit()
{
	if(SetER(t_DBMgrPoFaEnvUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::ApplyPoFaEnvUnit(DB_PO_FA_ENV_UNIT&	dpfeu)
{
	if(FindItem(dpfeu.tDPH.nID))
	{
		return EditPoFaEnvUnit(dpfeu);
	}
	return AddPoFaEnvUnit(dpfeu);
}
//---------------------------------------------------------------------------

String					CManagePoFaEnvUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_ENV_UNIT pdpfeu = FindItem(nID);
    if(!pdpfeu)	return "";
    return pdpfeu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaEnvUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV_UNIT pdpfeu = FindItem(nID);
	if(!pdpfeu)	return -1;

	return SetPkt(pdpfeu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::SetPkt(PDB_PO_FA_ENV_UNIT pdpfeu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfeu->tDPH);

	SendToken.TokenAdd_32(pdpfeu->nFilePtnType);
	SendToken.TokenAdd_String(pdpfeu->strFilePtnGlobal);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnvUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_ENV_UNIT& dpfeu)
{
	if (!RecvToken.TokenDel_DPH(dpfeu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfeu.nFilePtnType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfeu.strFilePtnGlobal) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



