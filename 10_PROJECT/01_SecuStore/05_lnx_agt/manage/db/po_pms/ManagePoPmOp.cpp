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
#include "ManagePoPmOp.h"

//---------------------------------------------------------------------------

CManagePoPmOp*	t_ManagePoPmOp = NULL;

//---------------------------------------------------------------------------

CManagePoPmOp::CManagePoPmOp()
{
	t_DBMgrPoPmOp	= new CDBMgrPoPmOp();
}
//---------------------------------------------------------------------------

CManagePoPmOp::~CManagePoPmOp()
{
	if(t_DBMgrPoPmOp)	{	delete t_DBMgrPoPmOp;	t_DBMgrPoPmOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoPmOp::LoadDBMS()
{
	TListDBPoPmOp tDBPoPmOpList;
    TListDBPoPmOpItor begin, end;
	if(SetER(t_DBMgrPoPmOp->LoadExecute(&tDBPoPmOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoPmOpList.begin();	end = tDBPoPmOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::InitHash()
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

INT32					CManagePoPmOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_PM_OP pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_pm_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,"
							"%u,%llu,%u,"
							"%s,%s,%s,%s,"
							"%u,%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nLogSyncMode,
							pdata->nDnTryNum, pdata->nMonRmPatch, pdata->nAutoPatchType,
							pdata->strPtnFileSvName.c_str(), pdata->strPtnVersion.c_str(), pdata->strPtnFileName.c_str(), pdata->strPtnFileHash.c_str(),
							pdata->nPtnDnFileType, pdata->strPtnDnFileName.c_str(), pdata->strPtnDnFileHash.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::AddPoPmOp(DB_PO_PM_OP&	data)
{
	if(SetER(t_DBMgrPoPmOp->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::EditPoPmOp(DB_PO_PM_OP&	data)
{
	PDB_PO_PM_OP pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoPmOp->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::DelPoPmOp(UINT32 nID)
{
	PDB_PO_PM_OP pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoPmOp->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::ApplyPoPmOp(DB_PO_PM_OP&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoPmOp(data);
	}
	return AddPoPmOp(data);
}
//---------------------------------------------------------------------------

String					CManagePoPmOp::GetName(UINT32 nID)
{
	PDB_PO_PM_OP pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_PM_OP pdata = FindItem(nID);
	if(!pdata)	return 0;

	if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;	

	return IsValidPtnFile(pdata);
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::IsValidPtnFile(PDB_PO_PM_OP pdata)
{
	CFileUtil tFileUtil;	
	String strFullFile = "";
	strFullFile = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_PTN_PATCH_FILE, pdata->strPtnFileName.c_str());

	if(tFileUtil.FileExists(strFullFile.c_str()) == 0)			
		return 0;

	{
		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullFile.c_str(), szHash, CHAR_MAX_SIZE-1);
		if(_stricmp(pdata->strPtnFileHash.c_str(), szHash))
		{
			WriteLogE("po pm ptn op invalid hash : [%s][%s]", szHash, pdata->strPtnFileHash.c_str());
			return 0;
		}
	}	
	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoPmOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_PM_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::SetPkt(PDB_PO_PM_OP pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nLogSyncMode);
	SendToken.TokenAdd_32(pdata->nDnTryNum);
	SendToken.TokenAdd_64(pdata->nMonRmPatch);
	SendToken.TokenAdd_32(pdata->nAutoPatchType);


	SendToken.TokenAdd_String(pdata->strPtnFileSvName);

	SendToken.TokenAdd_String(pdata->strPtnVersion);
	SendToken.TokenAdd_String(pdata->strPtnFileName);
	SendToken.TokenAdd_String(pdata->strPtnFileHash);

	SendToken.TokenAdd_32(pdata->nPtnDnFileType);
	SendToken.TokenAdd_String(pdata->strPtnDnFileName);
	SendToken.TokenAdd_String(pdata->strPtnDnFileHash);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::GetPkt(MemToken& RecvToken, DB_PO_PM_OP& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nLogSyncMode))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nDnTryNum))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_64(data.nMonRmPatch))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nAutoPatchType))			goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strPtnFileSvName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnVersion) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnFileName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnFileHash) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nPtnDnFileType))			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnDnFileName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strPtnDnFileHash) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoPmOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_PM_OP pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt(pdata, SendToken);

// 	{
// 		SendToken.TokenAdd_32(pdata->tDPH.tSubIDMap.size());
// 
// 		TMapIDItor begin, end;
// 		begin = pdata->tDPH.tSubIDMap.begin();	end = pdata->tDPH.tSubIDMap.end();
// 		for(begin; begin != end; begin++)
// 		{
// 			t_ManagePoPmScanPkg->SetPktHost(begin->first, SendToken);
// 		}
// 	}

	return 0;
}
//---------------------------------------------------------------------------



