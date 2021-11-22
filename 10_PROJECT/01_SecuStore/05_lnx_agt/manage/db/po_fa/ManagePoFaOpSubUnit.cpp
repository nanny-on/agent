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
#include "ManagePoFaOpSubUnit.h"

//---------------------------------------------------------------------------

CManagePoFaOpSubUnit*	t_ManagePoFaOpSubUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpSubUnit::CManagePoFaOpSubUnit()
{
	t_DBMgrPoFaOpSubUnit	= new CDBMgrPoFaOpSubUnit();
}
//---------------------------------------------------------------------------

CManagePoFaOpSubUnit::~CManagePoFaOpSubUnit()
{
	if(t_DBMgrPoFaOpSubUnit)	{	delete t_DBMgrPoFaOpSubUnit;	t_DBMgrPoFaOpSubUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpSubUnit::LoadDBMS()
{
	TListDBPoFaOpSubUnit tDBPoFaOpSubUnitList;
    TListDBPoFaOpSubUnitItor begin, end;
	if(SetER(t_DBMgrPoFaOpSubUnit->LoadExecute(&tDBPoFaOpSubUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpSubUnitList.begin();	end = tDBPoFaOpSubUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_SUB_UNIT pdpfosu 			= NULL;
	{
		if( (pdpfosu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_op_sub_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strLogMode;
		MapToHex_64(pdpfosu->tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
		
		m_strHashValue = SPrintf("%s,"
							"%u,%u,"
							"%s,%s,%s,%s,"
							"%s,", 
							GetHdrHashInfo(pdpfosu).c_str(),
							pdpfosu->nSbType, pdpfosu->nSbLinkID, 
							pdpfosu->strSbUnicode.c_str(), pdpfosu->strSbDomain.c_str(), pdpfosu->strSbName.c_str(), pdpfosu->strSbProc.c_str(),
							strLogMode.c_str());

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::AddPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu)
{
	if(SetER(t_DBMgrPoFaOpSubUnit->InsertExecute(&dpfosu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfosu.tDPH.nID, dpfosu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::EditPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu)
{
	PDB_PO_FA_OP_SUB_UNIT pdpfosu = FindItem(dpfosu.tDPH.nID);
	if(!pdpfosu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpSubUnit->UpdateExecute(&dpfosu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfosu.tDPH.nID, dpfosu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::DelPoFaOpSubUnit(UINT32 nID)
{
	PDB_PO_FA_OP_SUB_UNIT pdpfosu = FindItem(nID);
	if(!pdpfosu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpSubUnit->DeleteExecute(pdpfosu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::ClearPoFaOpSubUnit()
{
	if(SetER(t_DBMgrPoFaOpSubUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------


INT32					CManagePoFaOpSubUnit::ApplyPoFaOpSubUnit(DB_PO_FA_OP_SUB_UNIT&	dpfosu)
{
	if(FindItem(dpfosu.tDPH.nID))
	{
		return EditPoFaOpSubUnit(dpfosu);
	}
	return AddPoFaOpSubUnit(dpfosu);
}
//---------------------------------------------------------------------------

String					CManagePoFaOpSubUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_SUB_UNIT pdpfosu = FindItem(nID);
    if(!pdpfosu)	return "";
    return pdpfosu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpSubUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_SUB_UNIT pdpfosu = FindItem(nID);
	if(!pdpfosu)	return -1;

	return SetPkt(pdpfosu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::SetPkt(PDB_PO_FA_OP_SUB_UNIT pdpfosu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfosu->tDPH);

	SendToken.TokenAdd_32(pdpfosu->nSbType);
	SendToken.TokenAdd_32(pdpfosu->nSbLinkID);
	SendToken.TokenAdd_String(pdpfosu->strSbUnicode);
	SendToken.TokenAdd_String(pdpfosu->strSbDomain);
	SendToken.TokenAdd_String(pdpfosu->strSbName);
	SendToken.TokenAdd_String(pdpfosu->strSbProc);
	SendToken.TokenAdd_ID64Map(pdpfosu->tLogModeMap);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpSubUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_SUB_UNIT& dpfosu)
{
	if (!RecvToken.TokenDel_DPH(dpfosu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfosu.nSbType))						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfosu.nSbLinkID))					goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfosu.strSbUnicode) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfosu.strSbDomain) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfosu.strSbName) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpfosu.strSbProc) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(dpfosu.tLogModeMap) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



