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
#include "ManagePoFePtnBL.h"

//---------------------------------------------------------------------------

CManagePoFePtnBL*	t_ManagePoFePtnBL = NULL;

//---------------------------------------------------------------------------

CManagePoFePtnBL::CManagePoFePtnBL()
{
	t_DBMgrPoFePtnBL	= new CDBMgrPoFePtnBL();
}
//---------------------------------------------------------------------------

CManagePoFePtnBL::~CManagePoFePtnBL()
{
	if(t_DBMgrPoFePtnBL)	{	delete t_DBMgrPoFePtnBL;	t_DBMgrPoFePtnBL = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFePtnBL::LoadDBMS()
{
	TListDBPoFePtnBL tDBPoFePtnBLList;
    TListDBPoFePtnBLItor begin, end;
	if(SetER(t_DBMgrPoFePtnBL->LoadExecute(&tDBPoFePtnBLList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFePtnBLList.begin();	end = tDBPoFePtnBLList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::InitHash()
{
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

INT32					CManagePoFePtnBL::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_PTN_BL pdpfpb 			= NULL;
	{
		if( (pdpfpb = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_ptn_bl by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%s,%s,%s,"
							"%u,%u,", 
							GetHdrHashInfo(pdpfpb).c_str(),
							pdpfpb->tDPFI.strSaveName.c_str(), pdpfpb->tDPFI.strSavePath.c_str(), pdpfpb->tDPFI.strFileHash.c_str(), pdpfpb->tDPFI.strPtnVersion.c_str(),
							pdpfpb->tDPFI.nPtnUnitNum, pdpfpb->tDPFI.nPtnChkValue);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfpb->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::AddPoFePtnBL(DB_PO_FE_PTN_BL&	dpfpb)
{
	if(SetER(t_DBMgrPoFePtnBL->InsertExecute(&dpfpb)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpb.tDPH.nID, dpfpb);
	InitHash(dpfpb.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::EditPoFePtnBL(DB_PO_FE_PTN_BL&	dpfpb)
{
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(dpfpb.tDPH.nID);
	if(!pdpfpb)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFePtnBL->UpdateExecute(&dpfpb)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpb.tDPH.nID, dpfpb);
	InitHash(dpfpb.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::DelPoFePtnBL(UINT32 nID)
{
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(nID);
	if(!pdpfpb)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFePtnBL->DeleteExecute(pdpfpb->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::ApplyPoFePtnBL(DB_PO_FE_PTN_BL&	dpfpb)
{
	if(FindItem(dpfpb.tDPH.nID))
	{
		return EditPoFePtnBL(dpfpb);
	}
	return AddPoFePtnBL(dpfpb);
}
//---------------------------------------------------------------------------

String					CManagePoFePtnBL::GetName(UINT32 nID)
{
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(nID);
    if(!pdpfpb)	return "";
    return pdpfpb->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(nID);
	if(!pdpfpb)	return 0;

	if(pdpfpb->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;	

	return IsValidPtnFile(pdpfpb);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::IsValidPtnFile(PDB_PO_FE_PTN_BL pdpfpb)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdpfpb->tDPFI.strSaveName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE-1);
		if(_stricmp(pdpfpb->tDPFI.strFileHash.c_str(), szHash))
		{
			WriteLogE("po fe ptn bl invalid hash : [%s][%s]", szHash, pdpfpb->tDPFI.strFileHash.c_str());
			return 0;
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFePtnBLItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(nID);
	if(!pdpfpb)	return -1;

	SetPkt(pdpfpb, SendToken);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_BL pdpfpb = FindItem(nID);
	if(!pdpfpb)	return -1;

	SetPkt(pdpfpb, SendToken);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::SetPkt(PDB_PO_FE_PTN_BL pdpfpb, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpb->tDPH);

	SendToken.TokenAdd_String(pdpfpb->tDPFI.strSaveName);
	SendToken.TokenAdd_String(pdpfpb->tDPFI.strSavePath);
	SendToken.TokenAdd_String(pdpfpb->tDPFI.strFileHash);
	SendToken.TokenAdd_String(pdpfpb->tDPFI.strPtnVersion);
	SendToken.TokenAdd_32(pdpfpb->tDPFI.nPtnUnitNum);
	SendToken.TokenAdd_32(pdpfpb->tDPFI.nPtnChkValue);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnBL::GetPkt(MemToken& RecvToken, DB_PO_FE_PTN_BL& dpfpb)
{
	if(!RecvToken.TokenDel_DPH(dpfpb.tDPH))								goto INVALID_PKT;

	if( RecvToken.TokenDel_String(dpfpb.tDPFI.strSaveName) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpb.tDPFI.strSavePath) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpb.tDPFI.strFileHash) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpb.tDPFI.strPtnVersion) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpb.tDPFI.nPtnUnitNum))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpb.tDPFI.nPtnChkValue))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



