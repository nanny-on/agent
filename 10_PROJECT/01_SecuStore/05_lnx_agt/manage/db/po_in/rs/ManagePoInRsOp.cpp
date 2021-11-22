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
#include "ManagePoInRsOp.h"

//---------------------------------------------------------------------------

CManagePoInRsOp*	t_ManagePoInRsOp = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOp::CManagePoInRsOp()
{
	t_DBMgrPoInRsOp	= new CDBMgrPoInRsOp();
}
//---------------------------------------------------------------------------

CManagePoInRsOp::~CManagePoInRsOp()
{
	if(t_DBMgrPoInRsOp)	{	delete t_DBMgrPoInRsOp;	t_DBMgrPoInRsOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOp::LoadDBMS()
{
	TListDBPoInRsOp tDBPoInRsOpList;
    TListDBPoInRsOpItor begin, end;
	if(SetER(t_DBMgrPoInRsOp->LoadExecute(&tDBPoInRsOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpList.begin();	end = tDBPoInRsOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::InitHash()
{
	t_ManagePoInRsOpPkg->InitPkg();
	
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_RS_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", 
			GetHdrHashInfo(pdata).c_str());

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				t_ManagePoInRsOpPkg->GetHash(begin->first, EmptyStr(m_strHashValue));
				strOrgValue += m_strHashValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::AddPoInRsOp(DB_PO_IN_RS_OP&	data)
{
	if(SetER(t_DBMgrPoInRsOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::EditPoInRsOp(DB_PO_IN_RS_OP&	data)
{
	PDB_PO_IN_RS_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::DelPoInRsOp(UINT32 nID)
{
	PDB_PO_IN_RS_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::ApplyPoInRsOp(DB_PO_IN_RS_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsOp(data);
	}
	return AddPoInRsOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsOp::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

String					CManagePoInRsOp::GetObjExtList(UINT32 nID, TListStr& tExtList)
{
	PDB_PO_IN_RS_OP pdata = FindItem(nID);
	if(!pdata)		return "";

	String strRtn;

	TMapIDItor begin, end;
	begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_RS_OP_UNIT pdata_unit = t_ManagePoInRsOpUnit->FindItem(begin->second);
		if(!pdata_unit)	continue;


		TMapIDItor begin_unit, end_unit;
		begin_unit = pdata_unit->tDPH.tSubIDMap.begin();	end_unit = pdata_unit->tDPH.tSubIDMap.end();
		for(begin_unit; begin_unit != end_unit; begin_unit++)
		{
			String strPkgValue;
			UINT8 nKey = pdata_unit->tDPH._get_id_to_key(begin_unit->first);

			switch(nKey)
			{
				case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT:
				{
					PDB_PO_IN_RS_OP_OBJ_UNIT pdata_obj = t_ManagePoInRsOpObjUnit->FindItem(begin_unit->second);
					if(!pdata_obj)	break;

					tExtList.push_back(pdata_obj->strChkExt);
					strRtn += pdata_obj->strChkExt + ",";
					break;
				}
				case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT:		break;					
			}
		}
	}
	return strRtn;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInRsOpPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP pdata = FindItem(nID);
	if(!pdata)	return -1;
	
	SetPkt(pdata, SendToken);

	{		
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInRsOpPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::SetPkt(PDB_PO_IN_RS_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOp::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



