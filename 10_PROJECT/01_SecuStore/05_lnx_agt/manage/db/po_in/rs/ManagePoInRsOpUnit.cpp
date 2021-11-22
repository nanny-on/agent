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
#include "ManagePoInRsOpUnit.h"

//---------------------------------------------------------------------------

CManagePoInRsOpUnit*	t_ManagePoInRsOpUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInRsOpUnit::CManagePoInRsOpUnit()
{
	t_DBMgrPoInRsOpUnit	= new CDBMgrPoInRsOpUnit();
}
//---------------------------------------------------------------------------

CManagePoInRsOpUnit::~CManagePoInRsOpUnit()
{
	if(t_DBMgrPoInRsOpUnit)	{	delete t_DBMgrPoInRsOpUnit;	t_DBMgrPoInRsOpUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInRsOpUnit::LoadDBMS()
{
	TListDBPoInRsOpUnit tDBPoInRsOpUnitList;
    TListDBPoInRsOpUnitItor begin, end;
/*
	if(SetER(t_DBMgrPoInRsOpUnit->LoadExecute(&tDBPoInRsOpUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInRsOpUnitList.begin();	end = tDBPoInRsOpUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
*/
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_RS_OP_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strLogMode;
		MapToHex_64(pdata->tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);

		m_strHashValue = SPrintf("%s,"
								"%u,%s,", 
								GetHdrHashInfo(pdata).c_str(),
								pdata->nAcMode, strLogMode.c_str());

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				UINT8 nKey = pdata->tDPH._get_id_to_key(begin->first);
				UINT32 nSubID = pdata->tDPH._get_id_to_subid(begin->first);

				switch(nKey)
				{
					case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT:		t_ManagePoInRsOpUnitObjPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT:		t_ManagePoInRsOpUnitSubPkg->GetHash(nSubID, strPkgValue);		break;					
				}

				m_strHashValue += strPkgValue;
			}
		}

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::AddPoInRsOpUnit(DB_PO_IN_RS_OP_UNIT&	data)
{
	if(SetER(t_DBMgrPoInRsOpUnit->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::EditPoInRsOpUnit(DB_PO_IN_RS_OP_UNIT&	data)
{
	PDB_PO_IN_RS_OP_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnit->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::DelPoInRsOpUnit(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInRsOpUnit->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::DelPoInRsOpUnitWithSubPkg(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;
	
	TMapIDItor begin, end;
	begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		String strPkgValue;
		UINT8 nKey = pdata->tDPH._get_id_to_key(begin->first);
//		UINT32 nSubID = pdata->tDPH._get_id_to_subid(begin->first);

		switch(nKey)
		{
			case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT:		t_ManagePoInRsOpUnitObjPkg->ClearPkgUnitByPolicyID(nID);		break;
			case SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT:		t_ManagePoInRsOpUnitSubPkg->ClearPkgUnitByPolicyID(nID);		break;
		}
	}

	DelPoInRsOpUnit(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::ApplyPoInRsOpUnit(DB_PO_IN_RS_OP_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInRsOpUnit(data);
	}
	return AddPoInRsOpUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInRsOpUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_RS_OP_UNIT pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInRsOpUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::SetPkt(PDB_PO_IN_RS_OP_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nAcMode);
	SendToken.TokenAdd_ID64Map(pdata->tLogModeMap);


	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_RS_OP_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_rs_op_unit by pkt : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		SetPkt(pdata, SendToken);
	}

	{
		TListID tIDList;
		TListIDItor begin, end;

		{
			pdata->tDPH._get_id_list_by_key(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_SUBJECT, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdata->tDPH._get_id_to_subid(*begin);
				t_ManagePoInRsOpUnitSubPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdata->tDPH._get_id_list_by_key(SS_PO_IN_RS_OP_UNIT_KEY_TYPE_OBJECT, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdata->tDPH._get_id_to_subid(*begin);
				t_ManagePoInRsOpUnitObjPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		SendToken.TokenSet_Block();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInRsOpUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nAcMode))				goto INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(data.tLogModeMap) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



