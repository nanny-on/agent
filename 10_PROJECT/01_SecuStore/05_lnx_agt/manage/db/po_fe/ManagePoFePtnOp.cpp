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
#include "ManagePoFePtnOp.h"

//---------------------------------------------------------------------------

CManagePoFePtnOp*	t_ManagePoFePtnOp = NULL;

//---------------------------------------------------------------------------

CManagePoFePtnOp::CManagePoFePtnOp()
{
	t_DBMgrPoFePtnOp	= new CDBMgrPoFePtnOp();
}
//---------------------------------------------------------------------------

CManagePoFePtnOp::~CManagePoFePtnOp()
{
	if(t_DBMgrPoFePtnOp)	{	delete t_DBMgrPoFePtnOp;	t_DBMgrPoFePtnOp = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFePtnOp::LoadDBMS()
{
	TListDBPoFePtnOp tDBPoFePtnOpList;
    TListDBPoFePtnOpItor begin, end;
	if(SetER(t_DBMgrPoFePtnOp->LoadExecute(&tDBPoFePtnOpList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFePtnOpList.begin();	end = tDBPoFePtnOpList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::InitHash()
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

INT32					CManagePoFePtnOp::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FE_PTN_OP pdpfpo 			= NULL;
	{
		if( (pdpfpo = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fe_ptn_op by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%u,"
							"%u,%u,"
							"%u,%u,"
							"%u,%u,", 
							GetHdrHashInfo(pdpfpo).c_str(),
							pdpfpo->nBlockMode,
							pdpfpo->nPtnPkgID, pdpfpo->nPtnSchTime,
							pdpfpo->nPtnRangeType, pdpfpo->nPtnObjType,
							pdpfpo->nNewFileSendType, pdpfpo->nNewFileConfirmMode);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfpo->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::AddPoFePtnOp(DB_PO_FE_PTN_OP&	dpfpo)
{
	if(SetER(t_DBMgrPoFePtnOp->InsertExecute(&dpfpo)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfpo.tDPH.nID, dpfpo);
	InitHash(dpfpo.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::EditPoFePtnOp(DB_PO_FE_PTN_OP&	dpfpo)
{
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(dpfpo.tDPH.nID);
	if(!pdpfpo)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFePtnOp->UpdateExecute(&dpfpo)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfpo.tDPH.nID, dpfpo);
	InitHash(dpfpo.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::DelPoFePtnOp(UINT32 nID)
{
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(nID);
	if(!pdpfpo)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFePtnOp->DeleteExecute(pdpfpo->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::ApplyPoFePtnOp(DB_PO_FE_PTN_OP&	dpfpo)
{
	if(FindItem(dpfpo.tDPH.nID))
	{
		return EditPoFePtnOp(dpfpo);
	}
	return AddPoFePtnOp(dpfpo);
}
//---------------------------------------------------------------------------

String					CManagePoFePtnOp::GetName(UINT32 nID)
{
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(nID);
    if(!pdpfpo)	return "";
    return pdpfpo->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::IsValidPtnFile(UINT32 nID)
{	
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(nID);
	if(!pdpfpo)	return 0;

	if(pdpfpo->tDPH.nUsedMode == STATUS_USED_MODE_OFF)		return 0;

	return 1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFePtnOpItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(nID);
	if(!pdpfpo)	return -1;

	return SetPkt(pdpfpo, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::SetPkt(PDB_PO_FE_PTN_OP pdpfpo, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpo->tDPH);

	SendToken.TokenAdd_32(pdpfpo->nBlockMode);
	SendToken.TokenAdd_32(pdpfpo->nPtnPkgID);
	SendToken.TokenAdd_32(pdpfpo->nPtnSchTime);
	SendToken.TokenAdd_32(pdpfpo->nPtnRangeType);
	SendToken.TokenAdd_32(pdpfpo->nPtnObjType);
	SendToken.TokenAdd_32(pdpfpo->nNewFileSendType);
	SendToken.TokenAdd_32(pdpfpo->nNewFileConfirmMode);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::GetPkt(MemToken& RecvToken, DB_PO_FE_PTN_OP& dpfpo)
{
	if (!RecvToken.TokenDel_DPH(dpfpo.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfpo.nBlockMode))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnPkgID))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnSchTime))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnRangeType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnObjType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nNewFileSendType))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nNewFileConfirmMode))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FE_PTN_OP pdpfpo = FindItem(nID);
	if(!pdpfpo)	return -1;

	return SetPktHost(pdpfpo, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::SetPktHost(PDB_PO_FE_PTN_OP pdpfpo, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfpo->tDPH);

	SendToken.TokenAdd_32(pdpfpo->nBlockMode);
	SendToken.TokenAdd_32(pdpfpo->nPtnPkgID);
	SendToken.TokenAdd_32(pdpfpo->nPtnSchTime);
	SendToken.TokenAdd_32(pdpfpo->nPtnScanTime);
	SendToken.TokenAdd_32(pdpfpo->nPtnRangeType);
	SendToken.TokenAdd_32(pdpfpo->nPtnObjType);
	SendToken.TokenAdd_32(pdpfpo->nNewFileSendType);
	SendToken.TokenAdd_32(pdpfpo->nNewFileConfirmMode);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFePtnOp::GetPktHost(MemToken& RecvToken, DB_PO_FE_PTN_OP& dpfpo)
{
	if (!RecvToken.TokenDel_DPH(dpfpo.tDPH))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfpo.nBlockMode))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnPkgID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnSchTime))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnScanTime))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnRangeType))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nPtnObjType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nNewFileSendType))			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfpo.nNewFileConfirmMode))		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------



