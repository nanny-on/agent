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
#include "ManagePoDeployFileInfoUnit.h"

//---------------------------------------------------------------------------

CManagePoDeployFileInfoUnit*	t_ManagePoDeployFileInfoUnit = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFileInfoUnit::CManagePoDeployFileInfoUnit()
{
	t_DBMgrPoDeployFileInfoUnit	= new CDBMgrPoDeployFileInfoUnit();
}
//---------------------------------------------------------------------------

CManagePoDeployFileInfoUnit::~CManagePoDeployFileInfoUnit()
{
	if(t_DBMgrPoDeployFileInfoUnit)	{	delete t_DBMgrPoDeployFileInfoUnit;	t_DBMgrPoDeployFileInfoUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFileInfoUnit::LoadDBMS()
{
	TListDBPoDeployFileInfoUnit tDBPoDeployFileInfoUnitList;
    TListDBPoDeployFileInfoUnitItor begin, end;
	if(SetER(t_DBMgrPoDeployFileInfoUnit->LoadExecute(&tDBPoDeployFileInfoUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileInfoUnitList.begin();	end = tDBPoDeployFileInfoUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui 			= NULL;
	{
		if( (pdpdfui = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file_info_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		
		m_strHashValue = SPrintf("%s,"
							"%u,%u,%u,"
							"%s,%s,%s,%s,"
							"%s,%u,%u,", 
							GetHdrHashInfo(pdpdfui).c_str(),
							pdpdfui->nExeUserType, pdpdfui->nFileType, pdpdfui->nFileEncType, 
							pdpdfui->strFileOrName.c_str(), pdpdfui->strFileSaveName.c_str(), pdpdfui->strFileHash.c_str(), pdpdfui->strFileCommand.c_str(),
							pdpdfui->strTargetPath.c_str(), pdpdfui->nExeNumType, pdpdfui->nSchUsedMode);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::AddPoDeployFileInfoUnit(DB_PO_DEPLOY_FILE_INFO_UNIT&	dpdfui)
{
	if(SetER(t_DBMgrPoDeployFileInfoUnit->InsertExecute(&dpdfui)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpdfui.tDPH.nID, dpdfui);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::EditPoDeployFileInfoUnit(DB_PO_DEPLOY_FILE_INFO_UNIT&	dpdfui)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui = FindItem(dpdfui.tDPH.nID);
	if(!pdpdfui)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFileInfoUnit->UpdateExecute(&dpdfui)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdfui.tDPH.nID, dpdfui);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::DelPoDeployFileInfoUnit(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui = FindItem(nID);
	if(!pdpdfui)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFileInfoUnit->DeleteExecute(pdpdfui->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::ApplyPoDeployFileInfoUnit(DB_PO_DEPLOY_FILE_INFO_UNIT&	dpdfui)
{
	if(FindItem(dpdfui.tDPH.nID))
	{
		return EditPoDeployFileInfoUnit(dpdfui);
	}
	return AddPoDeployFileInfoUnit(dpdfui);
}
//---------------------------------------------------------------------------

PDB_PO_DEPLOY_FILE_INFO_UNIT	CManagePoDeployFileInfoUnit::FindItemByPoUnitID(UINT32 nPoUnitID)
{
	TListID tIDList;
	t_ManagePoDeployFileUnitInfoPkg->GetKeyIDList(nPoUnitID, tIDList);
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = FindItem(*begin);
		if(!pdpdfiu)	continue;

		return pdpdfiu;
	}
	return NULL;
}
//---------------------------------------------------------------------------

String					CManagePoDeployFileInfoUnit::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui = FindItem(nID);
    if(!pdpdfui)	return "";
    return pdpdfui->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileInfoUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui = FindItem(nID);
	if(!pdpdfui)	return -1;

	return SetPkt(pdpdfui, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui = FindItem(nID);
	if(!pdpdfui)	return -1;

	SetPkt(pdpdfui, SendToken);

	TListID tChkUnitIDList;

	{
		TMapIDItor begin, end;
		begin = pdpdfui->tDPH.tSubIDMap.begin();	end = pdpdfui->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			switch(LOWORD(begin->first))
			{
				case 0:	tChkUnitIDList.push_back(HIWORD(begin->first));		break;
			}
		}
	}
	
	{
		SendToken.TokenAdd_32(tChkUnitIDList.size());

		TListIDItor begin, end;
		begin = tChkUnitIDList.begin();	end = tChkUnitIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoDeployFileUnitChkPkg->SetPktHost(*begin, SendToken);			
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::SetPkt(PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfui, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdfui->tDPH);

	SendToken.TokenAdd_32(pdpdfui->nExeUserType);
	SendToken.TokenAdd_32(pdpdfui->nFileType);
	SendToken.TokenAdd_32(pdpdfui->nFileEncType);
	SendToken.TokenAdd_String(pdpdfui->strFileOrName);
	SendToken.TokenAdd_String(pdpdfui->strFileSaveName);
	SendToken.TokenAdd_String(pdpdfui->strFileHash);
	SendToken.TokenAdd_String(pdpdfui->strFileCommand);

	SendToken.TokenAdd_String(pdpdfui->strTargetPath);
	SendToken.TokenAdd_32(pdpdfui->nExeNumType);
	SendToken.TokenAdd_32(pdpdfui->nSchUsedMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFileInfoUnit::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE_INFO_UNIT& dpdfui)
{
	if (!RecvToken.TokenDel_DPH(dpdfui.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpdfui.nExeUserType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpdfui.nFileType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpdfui.nFileEncType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpdfui.strFileOrName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpdfui.strFileSaveName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpdfui.strFileHash) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(dpdfui.strFileCommand) < 0)		goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dpdfui.strTargetPath) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpdfui.nExeNumType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpdfui.nSchUsedMode))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



