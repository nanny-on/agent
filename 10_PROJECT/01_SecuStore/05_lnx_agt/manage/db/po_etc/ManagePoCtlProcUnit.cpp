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
#include "ManagePoCtlProcUnit.h"

//---------------------------------------------------------------------------

CManagePoCtlProcUnit*	t_ManagePoCtlProcUnit = NULL;

//---------------------------------------------------------------------------

CManagePoCtlProcUnit::CManagePoCtlProcUnit()
{
	t_DBMgrPoCtlProcUnit	= new CDBMgrPoCtlProcUnit();
}
//---------------------------------------------------------------------------

CManagePoCtlProcUnit::~CManagePoCtlProcUnit()
{
	if(t_DBMgrPoCtlProcUnit)	{	delete t_DBMgrPoCtlProcUnit;	t_DBMgrPoCtlProcUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlProcUnit::LoadDBMS()
{
	TListDBPoCtlProcUnit tDBPoCtlProcUnitList;
    TListDBPoCtlProcUnitItor begin, end;
	if(SetER(t_DBMgrPoCtlProcUnit->LoadExecute(&tDBPoCtlProcUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlProcUnitList.begin();	end = tDBPoCtlProcUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_CTL_PROC_UNIT pdpcpu 			= NULL;
	{
		if( (pdpcpu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_ctl_proc_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,%s,%s,%s,"
							"%s,%s,", 
							GetHdrHashInfo(pdpcpu).c_str(),
							pdpcpu->nBlockMode, pdpcpu->strFilePath.c_str(), pdpcpu->strFileComp.c_str(), pdpcpu->strFileDesc.c_str(), pdpcpu->strFileHash.c_str(), 
							pdpcpu->strFileSign.c_str(), pdpcpu->strFileSignComp.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::AddPoCtlProcUnit(DB_PO_CTL_PROC_UNIT&	dpcpu)
{
	if(SetER(t_DBMgrPoCtlProcUnit->InsertExecute(&dpcpu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpcpu.tDPH.nID, dpcpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::EditPoCtlProcUnit(DB_PO_CTL_PROC_UNIT&	dpcpu)
{
	PDB_PO_CTL_PROC_UNIT pdpcpu = FindItem(dpcpu.tDPH.nID);
	if(!pdpcpu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlProcUnit->UpdateExecute(&dpcpu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcpu.tDPH.nID, dpcpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::DelPoCtlProcUnit(UINT32 nID)
{
	PDB_PO_CTL_PROC_UNIT pdpcpu = FindItem(nID);
	if(!pdpcpu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlProcUnit->DeleteExecute(pdpcpu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::ApplyPoCtlProcUnit(DB_PO_CTL_PROC_UNIT&	dpcpu)
{
	if(FindItem(dpcpu.tDPH.nID))
	{
		return EditPoCtlProcUnit(dpcpu);
	}
	return AddPoCtlProcUnit(dpcpu);
}
//---------------------------------------------------------------------------

String					CManagePoCtlProcUnit::GetName(UINT32 nID)
{
	PDB_PO_CTL_PROC_UNIT pdpcpu = FindItem(nID);
    if(!pdpcpu)	return "";
    return pdpcpu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlProcUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PROC_UNIT pdpcpu = FindItem(nID);
	if(!pdpcpu)	return -1;

	return SetPkt(pdpcpu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::SetPkt(PDB_PO_CTL_PROC_UNIT pdpcpu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcpu->tDPH);

	SendToken.TokenAdd_32(pdpcpu->nBlockMode);
	SendToken.TokenAdd_String(pdpcpu->strFilePath);
	SendToken.TokenAdd_String(pdpcpu->strFileComp);
	SendToken.TokenAdd_String(pdpcpu->strFileDesc);
	SendToken.TokenAdd_String(pdpcpu->strFileHash);
	SendToken.TokenAdd_String(pdpcpu->strFileSign);
	SendToken.TokenAdd_String(pdpcpu->strFileSignComp);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlProcUnit::GetPkt(MemToken& RecvToken, DB_PO_CTL_PROC_UNIT& dpcpu)
{
	if (!RecvToken.TokenDel_DPH(dpcpu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpcpu.nBlockMode))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFilePath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFileComp) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFileDesc) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFileHash) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFileSign) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strFileSignComp) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



