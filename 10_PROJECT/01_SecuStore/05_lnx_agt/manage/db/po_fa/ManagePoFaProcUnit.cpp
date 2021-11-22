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
#include "ManagePoFaProcUnit.h"

//---------------------------------------------------------------------------

CManagePoFaProcUnit*	t_ManagePoFaProcUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaProcUnit::CManagePoFaProcUnit()
{
	t_DBMgrPoFaProcUnit	= new CDBMgrPoFaProcUnit();
}
//---------------------------------------------------------------------------

CManagePoFaProcUnit::~CManagePoFaProcUnit()
{
	if(t_DBMgrPoFaProcUnit)	{	delete t_DBMgrPoFaProcUnit;	t_DBMgrPoFaProcUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaProcUnit::LoadDBMS()
{
	TListDBPoFaProcUnit tDBPoFaProcUnitList;
    TListDBPoFaProcUnitItor begin, end;
	if(SetER(t_DBMgrPoFaProcUnit->LoadExecute(&tDBPoFaProcUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaProcUnitList.begin();	end = tDBPoFaProcUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_PROC_UNIT pdpfpu 			= NULL;
	{
		if( (pdpfpu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_proc_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,%s,%s,%s,"
							"%s,%s,", 
							GetHdrHashInfo(pdpfpu).c_str(),
							pdpfpu->nBlockMode, 
							pdpfpu->strFilePath.c_str(), pdpfpu->strFileComp.c_str(), pdpfpu->strFileDesc.c_str(), pdpfpu->strFileHash.c_str(), 
							pdpfpu->strFileSign.c_str(), pdpfpu->strFileSignComp.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::AddPoFaProcUnit(DB_PO_FA_PROC_UNIT&	dpfpu)
{
	if(SetER(t_DBMgrPoFaProcUnit->InsertExecute(&dpfpu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpu.tDPH.nID, dpfpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::EditPoFaProcUnit(DB_PO_FA_PROC_UNIT&	dpfpu)
{
	PDB_PO_FA_PROC_UNIT pdpfpu = FindItem(dpfpu.tDPH.nID);
	if(!pdpfpu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaProcUnit->UpdateExecute(&dpfpu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpu.tDPH.nID, dpfpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::DelPoFaProcUnit(UINT32 nID)
{
	PDB_PO_FA_PROC_UNIT pdpfpu = FindItem(nID);
	if(!pdpfpu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaProcUnit->DeleteExecute(pdpfpu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::ApplyPoFaProcUnit(DB_PO_FA_PROC_UNIT&	dpfpu)
{
	if(FindItem(dpfpu.tDPH.nID))
	{
		return EditPoFaProcUnit(dpfpu);
	}
	return AddPoFaProcUnit(dpfpu);
}
//---------------------------------------------------------------------------

String					CManagePoFaProcUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_PROC_UNIT pdpfpu = FindItem(nID);
    if(!pdpfpu)	return "";
    return pdpfpu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaProcUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_PROC_UNIT pdpfpu = FindItem(nID);
	if(!pdpfpu)	return -1;

	return SetPkt(pdpfpu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::SetPkt(PDB_PO_FA_PROC_UNIT pdpfpu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpu->tDPH);

	SendToken.TokenAdd_32(pdpfpu->nBlockMode);
	SendToken.TokenAdd_String(pdpfpu->strFilePath);
	SendToken.TokenAdd_String(pdpfpu->strFileComp);
	SendToken.TokenAdd_String(pdpfpu->strFileDesc);
	SendToken.TokenAdd_String(pdpfpu->strFileHash);
	SendToken.TokenAdd_String(pdpfpu->strFileSign);
	SendToken.TokenAdd_String(pdpfpu->strFileSignComp);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaProcUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_PROC_UNIT& dpfpu)
{
	if (!RecvToken.TokenDel_DPH(dpfpu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfpu.nBlockMode))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFilePath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFileComp) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFileDesc) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFileHash) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFileSign) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfpu.strFileSignComp) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



