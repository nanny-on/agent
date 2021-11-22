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
#include "ManagePoFePtnWL.h"

//---------------------------------------------------------------------------

CManagePoFePtnWL*	t_ManagePoFePtnWL = NULL;

//---------------------------------------------------------------------------

CManagePoFePtnWL::CManagePoFePtnWL()
{
	t_DBMgrPoFePtnWL	= new CDBMgrPoFePtnWL();
}
//---------------------------------------------------------------------------

CManagePoFePtnWL::~CManagePoFePtnWL()
{
	if(t_DBMgrPoFePtnWL)	{	delete t_DBMgrPoFePtnWL;	t_DBMgrPoFePtnWL = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFePtnWL::LoadDBMS()
{
	TListDBPoFePtnWL tDBPoFePtnWLList;
    TListDBPoFePtnWLItor begin, end;
	if(SetER(t_DBMgrPoFePtnWL->LoadExecute(&tDBPoFePtnWLList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFePtnWLList.begin();	end = tDBPoFePtnWLList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::InitHash()
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

INT32					CManagePoFePtnWL::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_PTN_WL pdpfpw 			= NULL;
	{
		if( (pdpfpw = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_ptn_wl by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,%u," 					
							"%s,%s,%s,%s,"
							"%u,%u,", 
							GetHdrHashInfo(pdpfpw).c_str(),
							pdpfpw->nPtnSchTime, pdpfpw->nPtnPkgID, pdpfpw->nPtnObjType, pdpfpw->nPtnPeBit,
							pdpfpw->tDPFI.strSaveName.c_str(), pdpfpw->tDPFI.strSavePath.c_str(), pdpfpw->tDPFI.strFileHash.c_str(), pdpfpw->tDPFI.strPtnVersion.c_str(),
							pdpfpw->tDPFI.nPtnUnitNum, pdpfpw->tDPFI.nPtnChkValue);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfpw->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::AddPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw)
{
	if(SetER(t_DBMgrPoFePtnWL->InsertExecute(&dpfpw)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpw.tDPH.nID, dpfpw);
	InitHash(dpfpw.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::EditPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw)
{
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(dpfpw.tDPH.nID);
	if(!pdpfpw)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFePtnWL->UpdateExecute(&dpfpw)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpw.tDPH.nID, dpfpw);
	InitHash(dpfpw.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::DelPoFePtnWL(UINT32 nID)
{
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(nID);
	if(!pdpfpw)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFePtnWL->DeleteExecute(pdpfpw->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::ApplyPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw)
{
	if(FindItem(dpfpw.tDPH.nID))
	{
		return EditPoFePtnWL(dpfpw);
	}
	return AddPoFePtnWL(dpfpw);
}
//---------------------------------------------------------------------------

String					CManagePoFePtnWL::GetName(UINT32 nID)
{
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(nID);
    if(!pdpfpw)	return "";
    return pdpfpw->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(nID);
	if(!pdpfpw)	return 0;

	if(pdpfpw->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;	

	return IsValidPtnFile(pdpfpw);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::IsValidPtnFile(PDB_PO_FE_PTN_WL pdpfpw)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdpfpw->tDPFI.strSaveName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE-1);
		if(_stricmp(pdpfpw->tDPFI.strFileHash.c_str(), szHash))	
			return 0;
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFePtnWLItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(nID);
	if(!pdpfpw)	return -1;

	SetPkt(pdpfpw, SendToken);
	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_WL pdpfpw = FindItem(nID);
	if(!pdpfpw)	return -1;

	SetPkt(pdpfpw, SendToken);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::SetPkt(PDB_PO_FE_PTN_WL pdpfpw, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpw->tDPH);

	SendToken.TokenAdd_32(pdpfpw->nPtnSchTime);
	SendToken.TokenAdd_32(pdpfpw->nPtnPkgID);
	SendToken.TokenAdd_32(pdpfpw->nPtnObjType);
	SendToken.TokenAdd_32(pdpfpw->nPtnPeBit);

	SendToken.TokenAdd_String(pdpfpw->tDPFI.strSaveName);
	SendToken.TokenAdd_String(pdpfpw->tDPFI.strSavePath);
	SendToken.TokenAdd_String(pdpfpw->tDPFI.strFileHash);
	SendToken.TokenAdd_String(pdpfpw->tDPFI.strPtnVersion);
	SendToken.TokenAdd_32(pdpfpw->tDPFI.nPtnUnitNum);
	SendToken.TokenAdd_32(pdpfpw->tDPFI.nPtnChkValue);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnWL::GetPkt(MemToken& RecvToken, DB_PO_FE_PTN_WL& dpfpw)
{
	if(!RecvToken.TokenDel_DPH(dpfpw.tDPH))							goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(dpfpw.nPtnSchTime))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpw.nPtnPkgID))						goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpw.nPtnObjType))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpw.nPtnPeBit))						goto INVALID_PKT;

	if( RecvToken.TokenDel_String(dpfpw.tDPFI.strSaveName) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpw.tDPFI.strSavePath) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpw.tDPFI.strFileHash) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpw.tDPFI.strPtnVersion) < 0)	goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpw.tDPFI.nPtnUnitNum))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpw.tDPFI.nPtnChkValue))			goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



