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
#include "ManagePoFaClearUnit.h"

//---------------------------------------------------------------------------

CManagePoFaClearUnit*	t_ManagePoFaClearUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaClearUnit::CManagePoFaClearUnit()
{
	t_DBMgrPoFaClearUnit	= new CDBMgrPoFaClearUnit();
}
//---------------------------------------------------------------------------

CManagePoFaClearUnit::~CManagePoFaClearUnit()
{
	if(t_DBMgrPoFaClearUnit)	{	delete t_DBMgrPoFaClearUnit;	t_DBMgrPoFaClearUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaClearUnit::LoadDBMS()
{
	TListDBPoFaClearUnit tDBPoFaClearUnitList;
    TListDBPoFaClearUnitItor begin, end;
	if(SetER(t_DBMgrPoFaClearUnit->LoadExecute(&tDBPoFaClearUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaClearUnitList.begin();	end = tDBPoFaClearUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu 			= NULL;
	{
		if( (pdpfcu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_clear_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%s,"
							"%u,%u,%u,"
							"%u,%u,%u,"
							"%u,%u,"
							"%s,%s,%s,%s,"
							"%u,%u,", 
							GetHdrHashInfo(pdpfcu).c_str(),
							pdpfcu->nFilePtnType, pdpfcu->strFilePtnLocal.c_str(), 
							pdpfcu->nSchType, pdpfcu->nSchTime, pdpfcu->nScanType,
							pdpfcu->nDelCount, pdpfcu->nLimitSize, pdpfcu->nLimitCnt,
							pdpfcu->nChkFDTType, pdpfcu->nChkFDTValue,
							pdpfcu->strExPath.c_str(), pdpfcu->strInPath.c_str(), pdpfcu->strExePath.c_str(), pdpfcu->strExeCommand.c_str(),
							pdpfcu->nNetDriveMode, pdpfcu->nDelMethod);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::AddPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu)
{
	if(SetER(t_DBMgrPoFaClearUnit->InsertExecute(&dpfcu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfcu.tDPH.nID, dpfcu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::EditPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu = FindItem(dpfcu.tDPH.nID);
	if(!pdpfcu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaClearUnit->UpdateExecute(&dpfcu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfcu.tDPH.nID, dpfcu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::DelPoFaClearUnit(UINT32 nID)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu = FindItem(nID);
	if(!pdpfcu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaClearUnit->DeleteExecute(pdpfcu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::ClearPoFaClearUnit()
{
	if(SetER(t_DBMgrPoFaClearUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}

INT32					CManagePoFaClearUnit::ApplyPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu)
{
	if(FindItem(dpfcu.tDPH.nID))
	{
		return EditPoFaClearUnit(dpfcu);
	}
	return AddPoFaClearUnit(dpfcu);
}
//---------------------------------------------------------------------------

String					CManagePoFaClearUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu = FindItem(nID);
    if(!pdpfcu)	return "";
    return pdpfcu->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::GetLastChkTime(UINT32 nID)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu = FindItem(nID);
	if(!pdpfcu)	return 0;
	return pdpfcu->nScanTime;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaClearUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_CLEAR_UNIT pdpfcu = FindItem(nID);
	if(!pdpfcu)	return -1;

	return SetPkt(pdpfcu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::SetPkt(PDB_PO_FA_CLEAR_UNIT pdpfcu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfcu->tDPH);

	SendToken.TokenAdd_32(pdpfcu->nFilePtnType);
	SendToken.TokenAdd_String(pdpfcu->strFilePtnLocal);
	SendToken.TokenAdd_32(pdpfcu->nSchType);
	SendToken.TokenAdd_32(pdpfcu->nSchTime);
	SendToken.TokenAdd_32(pdpfcu->nScanType);
	SendToken.TokenAdd_32(pdpfcu->nDelCount);
	SendToken.TokenAdd_32(pdpfcu->nChkFDTType);
	SendToken.TokenAdd_32(pdpfcu->nChkFDTValue);
	SendToken.TokenAdd_String(pdpfcu->strExPath);
	SendToken.TokenAdd_String(pdpfcu->strInPath);
	SendToken.TokenAdd_String(pdpfcu->strExePath);
	SendToken.TokenAdd_String(pdpfcu->strExeCommand);
	SendToken.TokenAdd_32(pdpfcu->nNetDriveMode);

	SendToken.TokenAdd_32(pdpfcu->nLimitSize);
	SendToken.TokenAdd_32(pdpfcu->nLimitCnt);
	SendToken.TokenAdd_32(pdpfcu->nDelMethod);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_CLEAR_UNIT& dpfcu)
{
	if (!RecvToken.TokenDel_DPH(dpfcu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfcu.nFilePtnType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfcu.strFilePtnLocal) < 0)	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nSchType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nSchTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nScanType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nDelCount))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nChkFDTType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nChkFDTValue))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfcu.strExPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfcu.strInPath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfcu.strExePath) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfcu.strExeCommand) < 0)	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nNetDriveMode))			goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfcu.nLimitSize))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nLimitCnt))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfcu.nDelMethod))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::SetPkt_Link(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaClearUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt_Link(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaClearUnit::SetPkt_Link(PDB_PO_FA_CLEAR_UNIT pdpfcu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfcu->tDPH);

	SendToken.TokenAdd_32(pdpfcu->nFilePtnType);
	SendToken.TokenAdd_String(pdpfcu->strFilePtnLocal);
	SendToken.TokenAdd_32(pdpfcu->nSchType);
	SendToken.TokenAdd_32(pdpfcu->nSchTime);
	SendToken.TokenAdd_32(pdpfcu->nScanType);
	SendToken.TokenAdd_32(pdpfcu->nDelCount);
	SendToken.TokenAdd_32(pdpfcu->nChkFDTType);
	SendToken.TokenAdd_32(pdpfcu->nChkFDTValue);
	SendToken.TokenAdd_String(pdpfcu->strExPath);
	SendToken.TokenAdd_String(pdpfcu->strInPath);
	SendToken.TokenAdd_String(pdpfcu->strExePath);
	SendToken.TokenAdd_String(pdpfcu->strExeCommand);
	SendToken.TokenAdd_32(pdpfcu->nNetDriveMode);

	SendToken.TokenAdd_32(pdpfcu->nScanTime);

	SendToken.TokenAdd_32(pdpfcu->nLimitSize);
	SendToken.TokenAdd_32(pdpfcu->nLimitCnt);
	SendToken.TokenAdd_32(pdpfcu->nDelMethod);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------