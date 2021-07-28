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
#include "ManagePoFePtnLo.h"

//---------------------------------------------------------------------------

CManagePoFePtnLo*	t_ManagePoFePtnLo = NULL;

//---------------------------------------------------------------------------

CManagePoFePtnLo::CManagePoFePtnLo()
{
	t_DBMgrPoFePtnLo	= new CDBMgrPoFePtnLo();
}
//---------------------------------------------------------------------------

CManagePoFePtnLo::~CManagePoFePtnLo()
{
	if(t_DBMgrPoFePtnLo)	{	delete t_DBMgrPoFePtnLo;	t_DBMgrPoFePtnLo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFePtnLo::LoadDBMS()
{
	TListDBPoFePtnLo tDBPoFePtnLoList;
    TListDBPoFePtnLoItor begin, end;
	if(SetER(t_DBMgrPoFePtnLo->LoadExecute(&tDBPoFePtnLoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFePtnLoList.begin();	end = tDBPoFePtnLoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::InitHash()
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

INT32					CManagePoFePtnLo::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_PTN_LO pdpfpl 			= NULL;
	{
		if( (pdpfpl = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_ptn_lo by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,"
							"%u,", 
							GetHdrHashInfo(pdpfpl).c_str(),
							pdpfpl->nPtnRangeType, pdpfpl->nPtnObjType,
							pdpfpl->nNewFileSendType);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfpl->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::AddPoFePtnLo(DB_PO_FE_PTN_LO&	dpfpl)
{
	if(SetER(t_DBMgrPoFePtnLo->InsertExecute(&dpfpl)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpl.tDPH.nID, dpfpl);
	InitHash(dpfpl.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::EditPoFePtnLo(DB_PO_FE_PTN_LO&	dpfpl)
{
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(dpfpl.tDPH.nID);
	if(!pdpfpl)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFePtnLo->UpdateExecute(&dpfpl)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpl.tDPH.nID, dpfpl);
	InitHash(dpfpl.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::DelPoFePtnLo(UINT32 nID)
{
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(nID);
	if(!pdpfpl)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFePtnLo->DeleteExecute(pdpfpl->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::ApplyPoFePtnLo(DB_PO_FE_PTN_LO&	dpfpl)
{
	if(FindItem(dpfpl.tDPH.nID))
	{
		return EditPoFePtnLo(dpfpl);
	}
	return AddPoFePtnLo(dpfpl);
}
//---------------------------------------------------------------------------

String					CManagePoFePtnLo::GetName(UINT32 nID)
{
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(nID);
    if(!pdpfpl)	return "";
    return pdpfpl->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(nID);
	if(!pdpfpl)	return 0;

	if(pdpfpl->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;
	
	return IsValidPtnFile(pdpfpl);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::IsValidPtnFile(PDB_PO_FE_PTN_LO pdpfpl)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), pdpfpl->strSavePath.c_str(), pdpfpl->strSaveName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE-1);
		if(_stricmp(pdpfpl->strFileHash.c_str(), szHash))
			return 0;
	}	
	return 1;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFePtnLoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(nID);
	if(!pdpfpl)	return -1;

	return SetPkt(pdpfpl, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::SetPkt(PDB_PO_FE_PTN_LO pdpfpl, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpl->tDPH);

	SendToken.TokenAdd_32(pdpfpl->nPtnRangeType);
	SendToken.TokenAdd_32(pdpfpl->nPtnObjType);
	SendToken.TokenAdd_32(pdpfpl->nNewFileSendType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::GetPkt(MemToken& RecvToken, DB_PO_FE_PTN_LO& dpfpl)
{
	if (!RecvToken.TokenDel_DPH(dpfpl.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfpl.nPtnRangeType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpl.nPtnObjType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpl.nNewFileSendType))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_LO pdpfpl = FindItem(nID);
	if(!pdpfpl)	return -1;

	return SetPktHost(pdpfpl, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::SetPktHost(PDB_PO_FE_PTN_LO pdpfpl, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpl->tDPH);

	SendToken.TokenAdd_32(pdpfpl->nPtnRangeType);
	SendToken.TokenAdd_32(pdpfpl->nPtnObjType);
	SendToken.TokenAdd_32(pdpfpl->nNewFileSendType);

	SendToken.TokenAdd_String(pdpfpl->strSaveName);
	SendToken.TokenAdd_String(pdpfpl->strSavePath);
	SendToken.TokenAdd_String(pdpfpl->strFileHash);
	SendToken.TokenAdd_String(pdpfpl->strPtnVersion);
	SendToken.TokenAdd_32(pdpfpl->nPtnUnitNum);
	SendToken.TokenAdd_32(pdpfpl->nPtnChkValue);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnLo::GetPktHost(MemToken& RecvToken, DB_PO_FE_PTN_LO& dpfpl)
{
	if (!RecvToken.TokenDel_DPH(dpfpl.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfpl.nPtnRangeType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpl.nPtnObjType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpl.nNewFileSendType))			goto	INVALID_PKT;

	if( RecvToken.TokenDel_String(dpfpl.strSaveName) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpl.strSavePath) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpl.strFileHash) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(dpfpl.strPtnVersion) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpl.nPtnUnitNum))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(dpfpl.nPtnChkValue))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------



