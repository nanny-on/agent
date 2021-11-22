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
#include "ManagePoDeployFileSchUnit.h"

//---------------------------------------------------------------------------

CManagePoDeployFileSchUnit*	t_ManagePoDeployFileSchUnit = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileSchUnit::CManagePoDeployFileSchUnit()
{
	t_DBMgrPoDeployFileSchUnit	= new CDBMgrPoDeployFileSchUnit();
}
//---------------------------------------------------------------------------

CManagePoDeployFileSchUnit::~CManagePoDeployFileSchUnit()
{
	if(t_DBMgrPoDeployFileSchUnit)	{	delete t_DBMgrPoDeployFileSchUnit;	t_DBMgrPoDeployFileSchUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileSchUnit::LoadDBMS()
{
	TListDBPoDeployFileSchUnit tDBPoDeployFileSchUnitList;
    TListDBPoDeployFileSchUnitItor begin, end;
	if(SetER(t_DBMgrPoDeployFileSchUnit->LoadExecute(&tDBPoDeployFileSchUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileSchUnitList.begin();	end = tDBPoDeployFileSchUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu 			= NULL;
	{
		if( (pdpdfsu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_sch_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,%u,%u,", 
							GetHdrHashInfo(pdpdfsu).c_str(),
							pdpdfsu->nSchType, pdpdfsu->nSchTime);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::AddPoDeployFileSchUnit(DB_PO_DEPLOY_FILE_SCH_UNIT&	dpdfsu)
{
	if(SetER(t_DBMgrPoDeployFileSchUnit->InsertExecute(&dpdfsu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpdfsu.tDPH.nID, dpdfsu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::EditPoDeployFileSchUnit(DB_PO_DEPLOY_FILE_SCH_UNIT&	dpdfsu)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu = FindItem(dpdfsu.tDPH.nID);
	if(!pdpdfsu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileSchUnit->UpdateExecute(&dpdfsu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfsu.tDPH.nID, dpdfsu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::DelPoDeployFileSchUnit(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu = FindItem(nID);
	if(!pdpdfsu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileSchUnit->DeleteExecute(pdpdfsu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::ApplyPoDeployFileSchUnit(DB_PO_DEPLOY_FILE_SCH_UNIT&	dpdfsu)
{
	if(FindItem(dpdfsu.tDPH.nID))
	{
		return EditPoDeployFileSchUnit(dpdfsu);
	}
	return AddPoDeployFileSchUnit(dpdfsu);
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileSchUnit::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu = FindItem(nID);
    if(!pdpdfsu)	return "";
    return pdpdfsu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileSchUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu = FindItem(nID);
	if(!pdpdfsu)	return -1;

	return SetPkt(pdpdfsu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::SetPkt(PDB_PO_DEPLOY_FILE_SCH_UNIT pdpdfsu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfsu->tDPH);

	SendToken.TokenAdd_32(pdpdfsu->nSchType);
	SendToken.TokenAdd_32(pdpdfsu->nSchTime);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileSchUnit::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_SCH_UNIT& dpdfsu)
{
	if (!RecvToken.TokenDel_DPH(dpdfsu.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpdfsu.nSchType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpdfsu.nSchTime))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



