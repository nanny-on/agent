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
#include "ManagePoInPtnBL.h"

//---------------------------------------------------------------------------

CManagePoInPtnBL*	t_ManagePoInPtnBL = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnBL::CManagePoInPtnBL()
{
	t_DBMgrPoInPtnBL	= new CDBMgrPoInPtnBL();
}
//---------------------------------------------------------------------------

CManagePoInPtnBL::~CManagePoInPtnBL()
{
	if(t_DBMgrPoInPtnBL)	{	delete t_DBMgrPoInPtnBL;	t_DBMgrPoInPtnBL = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnBL::LoadDBMS()
{
	TListDBPoInPtnBL tDBPoInPtnBLList;
    TListDBPoInPtnBLItor begin, end;
	if(SetER(t_DBMgrPoInPtnBL->LoadExecute(&tDBPoInPtnBLList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnBLList.begin();	end = tDBPoInPtnBLList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::InitHash()
{
	t_ManagePoInPtnBLPkg->InitPkg();
	
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

INT32					CManagePoInPtnBL::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_IN_PTN_BL pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_ptn_bl by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%s,%s,%s,"
							"%u,%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->tDPFI.strSaveName.c_str(), pdata->tDPFI.strSavePath.c_str(), pdata->tDPFI.strFileHash.c_str(), pdata->tDPFI.strPtnVersion.c_str(),
							pdata->tDPFI.nPtnUnitNum, pdata->tDPFI.nPtnChkValue);

		{
			TMapIDItor begin, end;
			begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoInPtnBLPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
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

INT32					CManagePoInPtnBL::AddPoInPtnBL(DB_PO_IN_PTN_BL&	data)
{
	if(SetER(t_DBMgrPoInPtnBL->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::EditPoInPtnBL(DB_PO_IN_PTN_BL&	data)
{
	PDB_PO_IN_PTN_BL pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnBL->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::DelPoInPtnBL(UINT32 nID)
{
	PDB_PO_IN_PTN_BL pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnBL->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::ApplyPoInPtnBL(DB_PO_IN_PTN_BL&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInPtnBL(data);
	}
	return AddPoInPtnBL(data);
}
//---------------------------------------------------------------------------

String					CManagePoInPtnBL::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_BL pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_IN_PTN_BL pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;	

	return IsValidPtnFile(pdata);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::IsValidPtnFile(PDB_PO_IN_PTN_BL pdata)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->tDPFI.strSaveName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE-1);
		if(_stricmp(pdata->tDPFI.strFileHash.c_str(), szHash))
		{
			WriteLogE("po in ptn bl invalid hash : [%s][%s]", szHash, pdata->tDPFI.strFileHash.c_str());
			return 0;
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInPtnBLItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_BL pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);
	
	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInPtnBLPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_BL pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

	{
		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInPtnBLPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::SetPkt(PDB_PO_IN_PTN_BL pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->tDPFI.strSaveName);
	SendToken.TokenAdd_String(pdata->tDPFI.strSavePath);
	SendToken.TokenAdd_String(pdata->tDPFI.strFileHash);
	SendToken.TokenAdd_String(pdata->tDPFI.strPtnVersion);
	SendToken.TokenAdd_32(pdata->tDPFI.nPtnUnitNum);
	SendToken.TokenAdd_32(pdata->tDPFI.nPtnChkValue);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnBL::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_BL& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))								goto INVALID_PKT;

	if( RecvToken.TokenDel_String(data.tDPFI.strSaveName) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.tDPFI.strSavePath) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.tDPFI.strFileHash) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.tDPFI.strPtnVersion) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.tDPFI.nPtnUnitNum))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.tDPFI.nPtnChkValue))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


UINT32					CManagePoInPtnBL::GetUsedMode(UINT32 nID)
{
	PDB_PO_IN_PTN_BL pdata = NULL;

	if(nID)		pdata = FindItem(nID);
	else		pdata = FirstItem();

	if(!pdata)	return STATUS_USED_MODE_OFF;

	return pdata->tDPH.nUsedMode;
}
