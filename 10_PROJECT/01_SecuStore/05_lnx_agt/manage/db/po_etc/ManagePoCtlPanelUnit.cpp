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
#include "ManagePoCtlPanelUnit.h"

//---------------------------------------------------------------------------

CManagePoCtlPanelUnit*	t_ManagePoCtlPanelUnit = NULL;

//---------------------------------------------------------------------------

CManagePoCtlPanelUnit::CManagePoCtlPanelUnit()
{
	t_DBMgrPoCtlPanelUnit	= new CDBMgrPoCtlPanelUnit();
}
//---------------------------------------------------------------------------

CManagePoCtlPanelUnit::~CManagePoCtlPanelUnit()
{
	if(t_DBMgrPoCtlPanelUnit)	{	delete t_DBMgrPoCtlPanelUnit;	t_DBMgrPoCtlPanelUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlPanelUnit::LoadDBMS()
{
	TListDBPoCtlPanelUnit tDBPoCtlPanelUnitList;
    TListDBPoCtlPanelUnitItor begin, end;
	if(SetER(t_DBMgrPoCtlPanelUnit->LoadExecute(&tDBPoCtlPanelUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlPanelUnitList.begin();	end = tDBPoCtlPanelUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_CTL_PANEL_UNIT pdpcpu 			= NULL;
	{
		if( (pdpcpu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_ctl_panel_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,%s,%s,", 
							GetHdrHashInfo(pdpcpu).c_str(),pdpcpu->nControlType, pdpcpu->strCtlRegInfo.c_str(), pdpcpu->strCtlFileInfo.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::AddPoCtlPanelUnit(DB_PO_CTL_PANEL_UNIT&	dpcpu)
{
	if(SetER(t_DBMgrPoCtlPanelUnit->InsertExecute(&dpcpu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpcpu.tDPH.nID, dpcpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::EditPoCtlPanelUnit(DB_PO_CTL_PANEL_UNIT&	dpcpu)
{
	PDB_PO_CTL_PANEL_UNIT pdpcpu = FindItem(dpcpu.tDPH.nID);
	if(!pdpcpu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlPanelUnit->UpdateExecute(&dpcpu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcpu.tDPH.nID, dpcpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::DelPoCtlPanelUnit(UINT32 nID)
{
	PDB_PO_CTL_PANEL_UNIT pdpcpu = FindItem(nID);
	if(!pdpcpu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlPanelUnit->DeleteExecute(pdpcpu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::ApplyPoCtlPanelUnit(DB_PO_CTL_PANEL_UNIT&	dpcpu)
{
	if(FindItem(dpcpu.tDPH.nID))
	{
		return EditPoCtlPanelUnit(dpcpu);
	}
	return AddPoCtlPanelUnit(dpcpu);
}
//---------------------------------------------------------------------------

String					CManagePoCtlPanelUnit::GetName(UINT32 nID)
{
	PDB_PO_CTL_PANEL_UNIT pdpcpu = FindItem(nID);
    if(!pdpcpu)	return "";
    return pdpcpu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlPanelUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL_UNIT pdpcpu = FindItem(nID);
	if(!pdpcpu)	return -1;

	return SetPkt(pdpcpu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::SetPkt(PDB_PO_CTL_PANEL_UNIT pdpcpu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcpu->tDPH);

	SendToken.TokenAdd_32(pdpcpu->nControlType);
	SendToken.TokenAdd_String(pdpcpu->strCtlRegInfo);
	SendToken.TokenAdd_String(pdpcpu->strCtlFileInfo);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanelUnit::GetPkt(MemToken& RecvToken, DB_PO_CTL_PANEL_UNIT& dpcpu)
{
	if (!RecvToken.TokenDel_DPH(dpcpu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpcpu.nControlType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strCtlRegInfo) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpcpu.strCtlFileInfo) < 0)	goto	INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



