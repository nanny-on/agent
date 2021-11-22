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
#include "ManagePoFaOpObjUnit.h"

//---------------------------------------------------------------------------

CManagePoFaOpObjUnit*	t_ManagePoFaOpObjUnit = NULL;

//---------------------------------------------------------------------------

CManagePoFaOpObjUnit::CManagePoFaOpObjUnit()
{
	t_DBMgrPoFaOpObjUnit	= new CDBMgrPoFaOpObjUnit();
}
//---------------------------------------------------------------------------

CManagePoFaOpObjUnit::~CManagePoFaOpObjUnit()
{
	if(t_DBMgrPoFaOpObjUnit)	{	delete t_DBMgrPoFaOpObjUnit;	t_DBMgrPoFaOpObjUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaOpObjUnit::LoadDBMS()
{
	TListDBPoFaOpObjUnit tDBPoFaOpObjUnitList;
    TListDBPoFaOpObjUnitItor begin, end;
	if(SetER(t_DBMgrPoFaOpObjUnit->LoadExecute(&tDBPoFaOpObjUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaOpObjUnitList.begin();	end = tDBPoFaOpObjUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou 			= NULL;
	{
		if( (pdpfoou = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_op_obj_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		m_strHashValue = SPrintf("%s,"
							"%s,%u,%u,", 
							GetHdrHashInfo(pdpfoou).c_str(),
							pdpfoou->strFilePath.c_str(), pdpfoou->nChkAcPath, pdpfoou->nChkSelfPath);

		strOrgValue += m_strHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::AddPoFaOpObjUnit(DB_PO_FA_OP_OBJ_UNIT&	dpfoou)
{
	if(SetER(t_DBMgrPoFaOpObjUnit->InsertExecute(&dpfoou)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfoou.tDPH.nID, dpfoou);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::EditPoFaOpObjUnit(DB_PO_FA_OP_OBJ_UNIT&	dpfoou)
{
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou = FindItem(dpfoou.tDPH.nID);
	if(!pdpfoou)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaOpObjUnit->UpdateExecute(&dpfoou)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfoou.tDPH.nID, dpfoou);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::DelPoFaOpObjUnit(UINT32 nID)
{
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou = FindItem(nID);
	if(!pdpfoou)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaOpObjUnit->DeleteExecute(pdpfoou->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::ClearPoFaOpObjUnit()
{
	if(SetER(t_DBMgrPoFaOpObjUnit->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------


INT32					CManagePoFaOpObjUnit::ApplyPoFaOpObjUnit(DB_PO_FA_OP_OBJ_UNIT&	dpfoou)
{
	if(FindItem(dpfoou.tDPH.nID))
	{
		return EditPoFaOpObjUnit(dpfoou);
	}
	return AddPoFaOpObjUnit(dpfoou);
}
//---------------------------------------------------------------------------

String					CManagePoFaOpObjUnit::GetName(UINT32 nID)
{
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou = FindItem(nID);
    if(!pdpfoou)	return "";
    return pdpfoou->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaOpObjUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou = FindItem(nID);
	if(!pdpfoou)	return -1;

	return SetPkt(pdpfoou, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::SetPkt(PDB_PO_FA_OP_OBJ_UNIT pdpfoou, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfoou->tDPH);

	SendToken.TokenAdd_String(pdpfoou->strFilePath);
	SendToken.TokenAdd_32(pdpfoou->nChkAcPath);
	SendToken.TokenAdd_32(pdpfoou->nChkSelfPath);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaOpObjUnit::GetPkt(MemToken& RecvToken, DB_PO_FA_OP_OBJ_UNIT& dpfoou)
{
	if (!RecvToken.TokenDel_DPH(dpfoou.tDPH))					goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dpfoou.strFilePath) < 0)		goto	INVALID_PKT;
	if ( dpfoou.strFilePath.length() < 1)						goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfoou.nChkAcPath))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfoou.nChkSelfPath))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


INT32					CManagePoFaOpObjUnit::SetInotifyObjUnit(UINT32 nPolID, UINT32 nExtOption, UINT32 nUnitID)
{
	INT32 nRetVal = 0;
	UINT32 nSubDir = 0;
	String strOutPath;
	PDB_PO_FA_OP_OBJ_UNIT pdpfoou = FindItem(nUnitID);
	if(!pdpfoou || t_ThreadPoFaOp == NULL)
		return -10001;

	if(pdpfoou->tDPH.nExtOption == 1 && nExtOption == 1)
		nSubDir = 1;

	if(pdpfoou->strFilePath.length() < 1)
		return -10004;

	if(pdpfoou->strFilePath.at(0) == '<')
	{
		if(t_ComFuncUtil->ConvertEnvPathToLocalPath(pdpfoou->strFilePath, strOutPath) != 0)
			return -10002;
		if(strOutPath.length() == 0)
			return -10003;
	}
	else
	{
		strOutPath = pdpfoou->strFilePath;
	}

	CHAR chTok = GetToken(strOutPath, ';', ',');
	CTokenString Token(strOutPath.c_str(), strOutPath.length(), chTok);
	while(Token.IsNextToken())
	{
		String strPath = Token.NextToken();
		if(strPath.length() > 0)
		{
			nRetVal = t_ThreadPoFaOp->AddWatchNotify(nPolID, nSubDir, (char *)strPath.c_str());
			if(nRetVal != 0)
			{
				nRetVal -= 10100;
				return nRetVal;
			}
		}
	}

	return 0;
}