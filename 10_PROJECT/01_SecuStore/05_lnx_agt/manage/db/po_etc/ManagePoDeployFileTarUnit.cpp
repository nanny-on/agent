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
#include "ManagePoDeployFileTarUnit.h"

//---------------------------------------------------------------------------

CManagePoDeployFileTarUnit*	t_ManagePoDeployFileTarUnit = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileTarUnit::CManagePoDeployFileTarUnit()
{
	t_DBMgrPoDeployFileTarUnit	= new CDBMgrPoDeployFileTarUnit();
}
//---------------------------------------------------------------------------

CManagePoDeployFileTarUnit::~CManagePoDeployFileTarUnit()
{
	if(t_DBMgrPoDeployFileTarUnit)	{	delete t_DBMgrPoDeployFileTarUnit;	t_DBMgrPoDeployFileTarUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileTarUnit::LoadDBMS()
{
	TListDBPoDeployFileTarUnit tDBPoDeployFileTarUnitList;
    TListDBPoDeployFileTarUnitItor begin, end;
	if(SetER(t_DBMgrPoDeployFileTarUnit->LoadExecute(&tDBPoDeployFileTarUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileTarUnitList.begin();	end = tDBPoDeployFileTarUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu 			= NULL;
	{
		if( (pdpdfcu = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_tar_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,"
							"%llu,%llu,", 
							GetHdrHashInfo(pdpdfcu).c_str(),
							pdpdfcu->nSysMode, pdpdfcu->nSysInfo);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::AddPoDeployFileTarUnit(DB_PO_DEPLOY_FILE_TAR_UNIT&	dpdfcu)
{
	if(SetER(t_DBMgrPoDeployFileTarUnit->InsertExecute(&dpdfcu)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpdfcu.tDPH.nID, dpdfcu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::EditPoDeployFileTarUnit(DB_PO_DEPLOY_FILE_TAR_UNIT&	dpdfcu)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu = FindItem(dpdfcu.tDPH.nID);
	if(!pdpdfcu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileTarUnit->UpdateExecute(&dpdfcu)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfcu.tDPH.nID, dpdfcu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::DelPoDeployFileTarUnit(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu = FindItem(nID);
	if(!pdpdfcu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileTarUnit->DeleteExecute(pdpdfcu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::ApplyPoDeployFileTarUnit(DB_PO_DEPLOY_FILE_TAR_UNIT&	dpdfcu)
{
	if(FindItem(dpdfcu.tDPH.nID))
	{
		return EditPoDeployFileTarUnit(dpdfcu);
	}
	return AddPoDeployFileTarUnit(dpdfcu);
}
//---------------------------------------------------------------------------


String					CManagePoDeployFileTarUnit::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu = FindItem(nID);
    if(!pdpdfcu)	return "";
    return pdpdfcu->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileTarUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu = FindItem(nID);
	if(!pdpdfcu)	return -1;

	return SetPkt(pdpdfcu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::SetPkt(PDB_PO_DEPLOY_FILE_TAR_UNIT pdpdfcu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfcu->tDPH);

	SendToken.TokenAdd_64(pdpdfcu->nSysMode);
	SendToken.TokenAdd_64(pdpdfcu->nSysInfo);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileTarUnit::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_TAR_UNIT& dpdfcu)
{
	if (!RecvToken.TokenDel_DPH(dpdfcu.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(dpdfcu.nSysMode))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_64(dpdfcu.nSysInfo))					goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



