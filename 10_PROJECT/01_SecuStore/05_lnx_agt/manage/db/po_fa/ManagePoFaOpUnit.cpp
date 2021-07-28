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
#include "ManagePoFaOpUnit.h"


//---------------------------------------------------------------------------

CManagePoFaOpUnit*	t_ManagePoFaOpUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpUnit::CManagePoFaOpUnit()
{
	t_DBMgrPoFaOpUnit	= new CDBMgrPoFaOpUnit();
}
//---------------------------------------------------------------------------

CManagePoFaOpUnit::~CManagePoFaOpUnit()
{
	if(t_DBMgrPoFaOpUnit)	{	delete t_DBMgrPoFaOpUnit;	t_DBMgrPoFaOpUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpUnit::LoadDBMS()
{
	TListDBPoFaOpUnit tDBPoFaOpUnitList;
    TListDBPoFaOpUnitItor begin, end;
	if(SetER(t_DBMgrPoFaOpUnit->LoadExecute(&tDBPoFaOpUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpUnitList.begin();	end = tDBPoFaOpUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_UNIT pdpfpu 			= NULL;
	String strLogMode;
	{
		if( (pdpfpu = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		
		MapToHex_64(pdpfpu->tLogModeMap, strLogMode, SS_GLOBAL_LOG_REC_MODE_INDEX_TOTAL_NUMBER);
		
		m_strHashValue = SPrintf("%s,"
							"%s,%u,", 
							GetHdrHashInfo(pdpfpu).c_str(),
							strLogMode.c_str(), pdpfpu->nUnitType);

		{
			TMapIDItor begin, end;
			begin = pdpfpu->tDPH.tSubIDMap.begin();	end = pdpfpu->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				UINT8 nKey = pdpfpu->tDPH._get_id_to_key(begin->first);
				UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(begin->first);

				switch(nKey)
				{
					case SS_PO_FA_OP_UNIT_KEY_TYPE_OBJECT:		t_ManagePoFaOpUnitObjPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_FA_OP_UNIT_KEY_TYPE_SUBJECT:		t_ManagePoFaOpUnitSubPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_FA_OP_UNIT_KEY_TYPE_SCHEDULE:	t_ManagePoFaOpUnitSchPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_FA_OP_UNIT_KEY_TYPE_RULE:		t_ManagePoFaOpUnitRulPkg->GetHash(nSubID, strPkgValue);		break;
				}

				m_strHashValue += strPkgValue;
			}
		}

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::AddPoFaOpUnit(DB_PO_FA_OP_UNIT&	dpfpu)
{
	if(SetER(t_DBMgrPoFaOpUnit->InsertExecute(&dpfpu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpu.tDPH.nID, dpfpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::EditPoFaOpUnit(DB_PO_FA_OP_UNIT&	dpfpu)
{
	PDB_PO_FA_OP_UNIT pdpfpu = FindItem(dpfpu.tDPH.nID);
	if(!pdpfpu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnit->UpdateExecute(&dpfpu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpu.tDPH.nID, dpfpu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::DelPoFaOpUnit(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT pdpfpu = FindItem(nID);
	if(!pdpfpu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpUnit->DeleteExecute(pdpfpu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::ClearPoFaOpUnit()
{
	if(SetER(t_DBMgrPoFaOpUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::DelPoFaOpUnitWithSubPkg(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT pdpfpu = FindItem(nID);
	if(!pdpfpu)	return ERR_DBMS_DELETE_FAIL;
	
	TMapIDItor begin, end;
	begin = pdpfpu->tDPH.tSubIDMap.begin();	end = pdpfpu->tDPH.tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		String strPkgValue;
		UINT8 nKey = pdpfpu->tDPH._get_id_to_key(begin->first);
//		UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(begin->first);

		switch(nKey)
		{
			case SS_PO_FA_OP_UNIT_KEY_TYPE_OBJECT:
				t_ManagePoFaOpUnitObjPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_FA_OP_UNIT_KEY_TYPE_SUBJECT:
				t_ManagePoFaOpUnitSubPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_FA_OP_UNIT_KEY_TYPE_SCHEDULE:
				t_ManagePoFaOpUnitSchPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_FA_OP_UNIT_KEY_TYPE_RULE:
				t_ManagePoFaOpUnitRulPkg->ClearPkgUnitByPolicyID(nID);
				break;
		}
	}

	DelPoFaOpUnit(nID);
	return 0;
}


INT32					CManagePoFaOpUnit::ClearPoFaOpUnitWithSubPkg()
{
	INT32 nRetVal = 0;
	nRetVal = t_ManagePoFaOpUnitObjPkg->ClearPkgUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	nRetVal = t_ManagePoFaOpUnitSubPkg->ClearPkgUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 2000;
		return nRetVal;
	}
	nRetVal = t_ManagePoFaOpUnitSchPkg->ClearPkgUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 3000;
		return nRetVal;
	}
	nRetVal = t_ManagePoFaOpUnitRulPkg->ClearPkgUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 4000;
		return nRetVal;
	}
	nRetVal = ClearPoFaOpUnit();
	if(nRetVal < 0)
	{
		nRetVal -= 5000;
		return nRetVal;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::ApplyPoFaOpUnit(DB_PO_FA_OP_UNIT&	dpfpu)
{
	if(FindItem(dpfpu.tDPH.nID))
	{
		return EditPoFaOpUnit(dpfpu);
	}
	return AddPoFaOpUnit(dpfpu);
}
//---------------------------------------------------------------------------

String					CManagePoFaOpUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_UNIT pdpfpu = FindItem(nID);
    if(!pdpfpu)	return "";
    return pdpfpu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT pdpfpu = FindItem(nID);
	if(!pdpfpu)	return -1;

	return SetPkt(pdpfpu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::SetPkt(PDB_PO_FA_OP_UNIT pdpfpu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpu->tDPH);

	SendToken.TokenAdd_ID64Map(pdpfpu->tLogModeMap);
	SendToken.TokenAdd_32(pdpfpu->nUnitType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_UNIT pdpfpu 			= NULL;
	{
		if( (pdpfpu = FindItem(nID)) == NULL)
		{
			if(nID != 0)
				WriteLogE("not find po_fa_op_unit by pkt : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		SetPkt(pdpfpu, SendToken);
	}

	{
		TListID tIDList;
		TListIDItor begin, end;

		{
			pdpfpu->tDPH._get_id_list_by_key(SS_PO_FA_OP_UNIT_KEY_TYPE_SUBJECT, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(*begin);
				t_ManagePoFaOpUnitSubPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpfpu->tDPH._get_id_list_by_key(SS_PO_FA_OP_UNIT_KEY_TYPE_OBJECT, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(*begin);
				t_ManagePoFaOpUnitObjPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpfpu->tDPH._get_id_list_by_key(SS_PO_FA_OP_UNIT_KEY_TYPE_SCHEDULE, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(*begin);
				t_ManagePoFaOpUnitSchPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpfpu->tDPH._get_id_list_by_key(SS_PO_FA_OP_UNIT_KEY_TYPE_RULE, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpfpu->tDPH._get_id_to_subid(*begin);
				t_ManagePoFaOpUnitRulPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		SendToken.TokenSet_Block();
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT& dpfpu)
{
	if (!RecvToken.TokenDel_DPH(dpfpu.tDPH))					goto	INVALID_PKT;

	if ( RecvToken.TokenDel_ID64Map(dpfpu.tLogModeMap) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpu.nUnitType))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32					CManagePoFaOpUnit::SetInotifyUnit(UINT32 nUnitID)
{
	PDB_PO_FA_OP_UNIT pdpfpu 			= NULL;
	UINT32 nExtOption = 0;
	UINT32 nSubID = 0;
	INT32 nRetVal = 0;
	if( (pdpfpu = FindItem(nUnitID)) == NULL)
	{
		if(nUnitID != 0)
			WriteLogE("not find po_fa_op_unit by inotify : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
		return -101;
	}

	nExtOption = pdpfpu->tDPH.nExtOption;

	{
		TListID tIDList;
		TListIDItor begin, end;
		{
			pdpfpu->tDPH._get_id_list_by_key(SS_PO_FA_OP_UNIT_KEY_TYPE_OBJECT, tIDList);
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				nSubID = pdpfpu->tDPH._get_id_to_subid(*begin);
				nRetVal = t_ManagePoFaOpUnitObjPkg->SetInotifyObjPkg(pdpfpu->tDPH.nID, nExtOption, nSubID);
				if(nRetVal != 0)
				{
					WriteLogE("fail to set po_fa_op_unit by inotify : [%d]", nRetVal);
				}
			}
			tIDList.clear();
		}
	}

	return 0;
}



