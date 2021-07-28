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
#include "ManagePoDeployFile.h"

//---------------------------------------------------------------------------

CManagePoDeployFile*	t_ManagePoDeployFile = NULL;

//---------------------------------------------------------------------------

CManagePoDeployFile::CManagePoDeployFile()
{
	t_DBMgrPoDeployFile	= new CDBMgrPoDeployFile();
}
//---------------------------------------------------------------------------

CManagePoDeployFile::~CManagePoDeployFile()
{
	if(t_DBMgrPoDeployFile)	{	delete t_DBMgrPoDeployFile;	t_DBMgrPoDeployFile = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDeployFile::LoadDBMS()
{
	TListDBPoDeployFile tDBPoDeployFileList;
    TListDBPoDeployFileItor begin, end;
	if(SetER(t_DBMgrPoDeployFile->LoadExecute(&tDBPoDeployFileList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDeployFileList.begin();	end = tDBPoDeployFileList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::InitHash()
{
	t_ManagePoDeployFilePkg->InitPkg();
	
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

INT32					CManagePoDeployFile::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_DEPLOY_FILE pdpdf 			= NULL;
	{
		if( (pdpdf = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_deploy_file by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,", GetHdrHashInfo(pdpdf).c_str());

		{
			TMapIDItor begin, end;
			begin = pdpdf->tDPH.tSubIDMap.begin();	end = pdpdf->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				t_ManagePoDeployFilePkg->GetHash(begin->first, EmptyStr(m_strHashValue));
				strOrgValue += m_strHashValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpdf->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::AddPoDeployFile(DB_PO_DEPLOY_FILE&	dpdf)
{
	if(SetER(t_DBMgrPoDeployFile->InsertExecute(&dpdf)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpdf.tDPH.nID, dpdf);
	InitHash(dpdf.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::EditPoDeployFile(DB_PO_DEPLOY_FILE&	dpdf)
{
	PDB_PO_DEPLOY_FILE pdpdf = FindItem(dpdf.tDPH.nID);
	if(!pdpdf)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoDeployFile->UpdateExecute(&dpdf)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpdf.tDPH.nID, dpdf);
	InitHash(dpdf.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::DelPoDeployFile(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE pdpdf = FindItem(nID);
	if(!pdpdf)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDeployFile->DeleteExecute(pdpdf->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::ApplyPoDeployFile(DB_PO_DEPLOY_FILE&	dpdf)
{
	if(FindItem(dpdf.tDPH.nID))
	{
		return EditPoDeployFile(dpdf);
	}
	return AddPoDeployFile(dpdf);
}
//---------------------------------------------------------------------------

String					CManagePoDeployFile::GetName(UINT32 nID)
{
	PDB_PO_DEPLOY_FILE pdpdf = FindItem(nID);
    if(!pdpdf)	return "";
    return pdpdf->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDeployFileItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE pdpdf = FindItem(nID);
	if(!pdpdf)	return -1;

	SetPkt(pdpdf, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpdf->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpdf->tDPH.tSubIDMap.begin();	end = pdpdf->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoDeployFilePkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DEPLOY_FILE pdpdf = FindItem(nID);
	if(!pdpdf)	return -1;

	SetPkt(pdpdf, SendToken);

	{
		SendToken.TokenAdd_32(pdpdf->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpdf->tDPH.tSubIDMap.begin();	end = pdpdf->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoDeployFilePkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::SetPkt(PDB_PO_DEPLOY_FILE pdpdf, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpdf->tDPH);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDeployFile::GetPkt(MemToken& RecvToken, DB_PO_DEPLOY_FILE& dpdf)
{
	if (!RecvToken.TokenDel_DPH(dpdf.tDPH))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



