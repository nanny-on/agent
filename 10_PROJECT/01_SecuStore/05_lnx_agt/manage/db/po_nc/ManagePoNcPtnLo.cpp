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
#include "ManagePoNcPtnLo.h"

//---------------------------------------------------------------------------

CManagePoNcPtnLo*	t_ManagePoNcPtnLo = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnLo::CManagePoNcPtnLo()
{
	t_DBMgrPoNcPtnLo	= new CDBMgrPoNcPtnLo();
}
//---------------------------------------------------------------------------

CManagePoNcPtnLo::~CManagePoNcPtnLo()
{
	if(t_DBMgrPoNcPtnLo)	{	delete t_DBMgrPoNcPtnLo;	t_DBMgrPoNcPtnLo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnLo::LoadDBMS()
{
	TListDBPoNcPtnLo tDBPoNcPtnLoList;
    TListDBPoNcPtnLoItor begin, end;
	if(SetER(t_DBMgrPoNcPtnLo->LoadExecute(&tDBPoNcPtnLoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnLoList.begin();	end = tDBPoNcPtnLoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::InitHash()
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

INT32					CManagePoNcPtnLo::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_NC_PTN_LO pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_nc_ptn_lo by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,%u,%u,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nPtnRangeType, pdata->nPtnObjType, pdata->nUserEditPtn);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::AddPoNcPtnLo(DB_PO_NC_PTN_LO&	data)
{
	if(SetER(t_DBMgrPoNcPtnLo->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::EditPoNcPtnLo(DB_PO_NC_PTN_LO&	data)
{
	PDB_PO_NC_PTN_LO pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnLo->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::DelPoNcPtnLo(UINT32 nID)
{
	PDB_PO_NC_PTN_LO pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnLo->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::ApplyPoNcPtnLo(DB_PO_NC_PTN_LO&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoNcPtnLo(data);
	}
	return AddPoNcPtnLo(data);
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnLo::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_LO pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_NC_PTN_LO pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;
	
	return IsValidPtnFile(pdata);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::IsValidPtnFile(PDB_PO_NC_PTN_LO pdata)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->strSaveName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE);
		if(_stricmp(pdata->strFileHash.c_str(), szHash))	return 0;
	}	
	return 1;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnLoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_LO pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::SetPkt(PDB_PO_NC_PTN_LO pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nPtnRangeType);
	SendToken.TokenAdd_32(pdata->nPtnObjType);
	SendToken.TokenAdd_32(pdata->nUserEditPtn);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_LO& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPtnRangeType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnObjType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUserEditPtn))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_LO pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPktHost(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::SetPktHost(PDB_PO_NC_PTN_LO pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nPtnRangeType);
	SendToken.TokenAdd_32(pdata->nPtnObjType);
	SendToken.TokenAdd_32(pdata->nUserEditPtn);

	SendToken.TokenAdd_String(pdata->strSaveName);
	SendToken.TokenAdd_String(pdata->strSavePath);
	SendToken.TokenAdd_String(pdata->strFileHash);
	SendToken.TokenAdd_String(pdata->strPtnVersion);
	SendToken.TokenAdd_32(pdata->nPtnUnitNum);
	SendToken.TokenAdd_32(pdata->nPtnChkValue);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnLo::GetPktHost(MemToken& RecvToken, DB_PO_NC_PTN_LO& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPtnRangeType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnObjType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUserEditPtn))				goto INVALID_PKT;

	if( RecvToken.TokenDel_String(data.strSaveName) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strSavePath) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strFileHash) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strPtnVersion) < 0)		goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nPtnUnitNum))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nPtnChkValue))				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------



