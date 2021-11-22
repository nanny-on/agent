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
#include "ManagePoDeployFileUnit.h"

//---------------------------------------------------------------------------

CManagePoDeployFileUnit*	t_ManagePoDeployFileUnit = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileUnit::CManagePoDeployFileUnit()
{
	t_DBMgrPoDeployFileUnit	= new CDBMgrPoDeployFileUnit();
}
//---------------------------------------------------------------------------

CManagePoDeployFileUnit::~CManagePoDeployFileUnit()
{
	if(t_DBMgrPoDeployFileUnit)	{	delete t_DBMgrPoDeployFileUnit;	t_DBMgrPoDeployFileUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileUnit::LoadDBMS()
{
	TListDBPoDeployFileUnit tDBPoDeployFileUnitList;
    TListDBPoDeployFileUnitItor begin, end;
	if(SetER(t_DBMgrPoDeployFileUnit->LoadExecute(&tDBPoDeployFileUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileUnitList.begin();	end = tDBPoDeployFileUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu 			= NULL;
	{
		if( (pdpdfu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,", 
								GetHdrHashInfo(pdpdfu).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpdfu->tDPH.tSubIDMap.begin();	end = pdpdfu->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				UINT8 nKey = pdpdfu->tDPH._get_id_to_key(begin->first);
				UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(begin->first);

				switch(nKey)
				{
					case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO:		t_ManagePoDeployFileUnitInfoPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_CHK:		t_ManagePoDeployFileUnitChkPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR:		t_ManagePoDeployFileUnitTarPkg->GetHash(nSubID, strPkgValue);		break;
					case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH:		t_ManagePoDeployFileUnitSchPkg->GetHash(nSubID, strPkgValue);		break;
				}
				
				m_strHashValue += strPkgValue;
			}
			strOrgValue += m_strHashValue;
		}			
	}
	return 0;
}
//---------------------------------------------------------------------------


INT32					CManagePoDeployFileUnit::AddPoDeployFileUnit(DB_PO_DEPLOY_FILE_UNIT&	dpdfu)
{
	if(SetER(t_DBMgrPoDeployFileUnit->InsertExecute(&dpdfu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpdfu.tDPH.nID, dpdfu);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::EditPoDeployFileUnit(DB_PO_DEPLOY_FILE_UNIT&	dpdfu)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(dpdfu.tDPH.nID);
	if(!pdpdfu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnit->UpdateExecute(&dpdfu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfu.tDPH.nID, dpdfu);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::DelPoDeployFileUnit(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(nID);
	if(!pdpdfu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileUnit->DeleteExecute(pdpdfu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::DelPoDeployFileUnitWithSubPkg(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(nID);
	if(!pdpdfu)
		return ERR_DBMS_DELETE_FAIL;

	TMapIDItor begin, end;
	begin = pdpdfu->tDPH.tSubIDMap.begin();	end = pdpdfu->tDPH.tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		String strPkgValue;
		UINT8 nKey = pdpdfu->tDPH._get_id_to_key(begin->first);
//		UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(begin->first);

		switch(nKey)
		{
			case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO:
				t_ManagePoDeployFileUnitInfoPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_CHK:
				t_ManagePoDeployFileUnitChkPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR:
				t_ManagePoDeployFileUnitTarPkg->ClearPkgUnitByPolicyID(nID);
				break;
			case SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH:
				t_ManagePoDeployFileUnitSchPkg->ClearPkgUnitByPolicyID(nID);
				break;
		}
	}
	
	DelPoDeployFileUnit(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::ApplyPoDeployFileUnit(DB_PO_DEPLOY_FILE_UNIT&	dpdfu)
{
	if(FindItem(dpdfu.tDPH.nID))
	{
		return EditPoDeployFileUnit(dpdfu);
	}
	return AddPoDeployFileUnit(dpdfu);
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileUnit::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(nID);
    if(!pdpdfu)	return "";
    return pdpdfu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(nID);
	if(!pdpdfu)	return -1;

	SetPkt(pdpdfu, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpdfu->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpdfu->tDPH.tSubIDMap.begin();	end = pdpdfu->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoDeployFilePkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_UNIT pdpdfu = FindItem(nID);
	if(!pdpdfu)	return -1;

	{
		SetPkt(pdpdfu, SendToken);
	}

	{
		TListID tIDList;
		TListIDItor begin, end;

		{
			pdpdfu->tDPH._get_id_list_by_key(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_INFO, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(*begin);
				t_ManagePoDeployFileUnitInfoPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpdfu->tDPH._get_id_list_by_key(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_CHK, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(*begin);
				t_ManagePoDeployFileUnitChkPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpdfu->tDPH._get_id_list_by_key(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_TAR, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(*begin);
				t_ManagePoDeployFileUnitTarPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		{
			pdpdfu->tDPH._get_id_list_by_key(SS_PO_DEPLOY_FILE_UNIT_KEY_TYPE_SCH, tIDList);
			SendToken.TokenAdd_32(tIDList.size());
			begin = tIDList.begin();	end = tIDList.end();
			for(begin; begin != end; begin++)
			{
				UINT32 nSubID = pdpdfu->tDPH._get_id_to_subid(*begin);
				t_ManagePoDeployFileUnitSchPkg->SetPktHost(nSubID, SendToken);
			}
			SendToken.TokenSet_Block();
			tIDList.clear();
		}
		SendToken.TokenSet_Block();
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::SetPkt(PDB_PO_DEPLOY_FILE_UNIT pdpdfu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfu->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileUnit::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_UNIT& dpdfu)
{
	if (!RecvToken.TokenDel_DPH(dpdfu.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------




