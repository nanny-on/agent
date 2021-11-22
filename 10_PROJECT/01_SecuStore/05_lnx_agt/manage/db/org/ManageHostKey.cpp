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
#include "ManageHostKey.h"

//---------------------------------------------------------------------------

CManageHostKey*	t_ManageHostKey = NULL;

//---------------------------------------------------------------------------

CManageHostKey::CManageHostKey()
{
	t_DBMgrHostKey	= new CDBMgrHostKey();
}
//---------------------------------------------------------------------------

CManageHostKey::~CManageHostKey()
{
	if(t_DBMgrHostKey)		{	delete t_DBMgrHostKey;		t_DBMgrHostKey = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostKey::LoadDBMS()
{
	DB_HOST_KEY data;
	TListDBHostKey tDBHostKeyList;
    TListDBHostKeyItor begin, end;
	
	if(SetER(t_DBMgrHostKey->LoadExecute(&tDBHostKeyList)))
    {
		if(tDBHostKeyList.size() == 0)
		{
			memset(&data, 0, sizeof(DB_HOST_KEY));
			data.strBkKey = GetEncBkKey(t_EnvInfo->m_strHostKey);
			AddHostKey(data);
		}
    	return g_nErrRtn;
    }

    begin = tDBHostKeyList.begin();	end = tDBHostKeyList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }

	if(tDBHostKeyList.size() == 0)
	{
		memset(&data, 0, sizeof(DB_HOST_KEY));
		data.strBkKey = GetEncBkKey(t_EnvInfo->m_strHostKey);
		AddHostKey(data);
	}
    return 0;
}
//---------------------------------------------------------------------------

String			CManageHostKey::GetHash()
{
	String strOrgValue, strHash;

	PDB_HOST_KEY pdata 			= NULL;
	{
		if( (pdata = FirstItem()) == NULL)
		{
			WriteLogE("not find host key by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return strHash;
		}
	}

	{
		strOrgValue = SPrintf("%s,%s,",pdata->strBkKey.c_str(), pdata->strCrKey.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		strHash = pszHashValue;
	}
	return strHash;
}
//---------------------------------------------------------------------------

INT32					CManageHostKey::AddHostKey(DB_HOST_KEY&	data)
{
	if(SetER(t_DBMgrHostKey->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	data.strRcKey = data.strBkKey;
    AddItem(data.nID, data);     
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostKey::EditHostKey(DB_HOST_KEY&	data)
{
    if(t_DBMgrHostKey->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	data.strRcKey = data.strBkKey;
	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostKey::DelHostKey(UINT32 nID)
{
	PDB_HOST_KEY pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostKey->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

String			CManageHostKey::GetEncBkKey(String strDecKey)
{
	CEncodeUtil tEncodeUtil;
	BYTE    pbOTPKey[16] = {0x11, 0x13, 0x36, 0x69, 0x3A, 0x3B, 0xFC, 0x1F, 0x5A, 0x4C, 0x13, 0xD3, 0xF2, 0xD3, 0x26, 0x94};

	BYTE*   lpChBuf = NULL; 
	INT32   nOutLen = 0;
	String strKey = GetGUID();

	t_SeedDLLUtil->SetKey(pbOTPKey);
	if(t_SeedDLLUtil->EncData((PBYTE)strDecKey.c_str(), strDecKey.length(), &lpChBuf, nOutLen))
	{
		return "";
	}
	String strEncKey = tEncodeUtil.EncBase(lpChBuf, nOutLen);
	t_SeedDLLUtil->FreeBuffer(lpChBuf);

	return strEncKey;
}
//---------------------------------------------------------------------------

String			CManageHostKey::GetDecBkKey(String strEncKey)
{
	CEncodeUtil tEncodeUtil;
	BYTE    pbOTPKey[16] = {0x11, 0x13, 0x36, 0x69, 0x3A, 0x3B, 0xFC, 0x1F, 0x5A, 0x4C, 0x13, 0xD3, 0xF2, 0xD3, 0x26, 0x94};

	if(strEncKey.empty())
	{
		PDB_HOST_KEY pdata = FirstItem();
		if(!pdata)	return "";

		if(pdata->strBkKey.empty())	strEncKey = GetEncBkKey(t_EnvInfo->m_strHostKey);
		else							strEncKey = pdata->strBkKey;
	}
	
	BYTE*   lpChBuf = NULL;
	BYTE pDecBase[100] = {0, };
	INT32 nDecBaseLen = 100;
	INT32   nOutLen = 0;
	tEncodeUtil.DecBase(strEncKey, pDecBase, nDecBaseLen);

	t_SeedDLLUtil->SetKey(pbOTPKey);
	if(t_SeedDLLUtil->DecData(pDecBase, &lpChBuf, nOutLen))
	{
		return "";
	}
	String strDecKey = (PCHAR)lpChBuf;
	t_SeedDLLUtil->FreeBuffer(lpChBuf);

	return strDecKey;
}
//---------------------------------------------------------------------------

String			CManageHostKey::GetDecRcKey(String strEncKey)
{
	CEncodeUtil tEncodeUtil;
	BYTE    pbOTPKey[16] = {0x11, 0x13, 0x36, 0x69, 0x3A, 0x3B, 0xFC, 0x1F, 0x5A, 0x4C, 0x13, 0xD3, 0xF2, 0xD3, 0x26, 0x94};

	if(strEncKey.empty())
	{
		PDB_HOST_KEY pdata = FirstItem();
		if(!pdata || pdata->strRcKey.empty())	return "";

		strEncKey = pdata->strRcKey;
	}

	BYTE*   lpChBuf = NULL;
	BYTE pDecBase[100] = {0, };
	INT32 nDecBaseLen = 100;
	INT32   nOutLen = 0;
	tEncodeUtil.DecBase(strEncKey, pDecBase, nDecBaseLen);

	t_SeedDLLUtil->SetKey(pbOTPKey);
	if(t_SeedDLLUtil->DecData(pDecBase, &lpChBuf, nOutLen))
	{
		return "";
	}
	String strDecKey = (PCHAR)lpChBuf;
	t_SeedDLLUtil->FreeBuffer(lpChBuf);

	return strDecKey;
}
//---------------------------------------------------------------------------

INT32			CManageHostKey::ClearHostKey_RcKey()
{
	PDB_HOST_KEY pdata = FirstItem();
	if(!pdata)	return 0;

	pdata->strRcKey = "";
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String			CManageHostKey::GetEncCrKey(String strDecKey)
{
	CEncodeUtil tEncodeUtil;
	BYTE    pbCrKey[16] = {0x11, 0x13, 0x34, 0x69, 0xE4, 0x3B, 0xCC, 0x1F, 0x85, 0x4C, 0x27, 0xD3, 0xC2, 0xDF, 0x26, 0x34};

	BYTE*   lpChBuf = NULL; 
	INT32   nOutLen = 0;
	String strKey = GetGUID();

	t_SeedDLLUtil->SetKey(pbCrKey);
	if(t_SeedDLLUtil->EncData((PBYTE)strDecKey.c_str(), strDecKey.length(), &lpChBuf, nOutLen))
	{
		return "";
	}
	String strEncKey = tEncodeUtil.EncBase(lpChBuf, nOutLen);
	t_SeedDLLUtil->FreeBuffer(lpChBuf);

	return strEncKey;
}
//---------------------------------------------------------------------------

String			CManageHostKey::GetDecCrKey(String strEncKey)
{
	CEncodeUtil tEncodeUtil;
	BYTE    pbCrKey[16] = {0x11, 0x13, 0x34, 0x69, 0xE4, 0x3B, 0xCC, 0x1F, 0x85, 0x4C, 0x27, 0xD3, 0xC2, 0xDF, 0x26, 0x34};

	if(strEncKey.empty())
	{
		PDB_HOST_KEY pdata = FirstItem();
		if(!pdata)	
			return "";

		if(pdata->strCrKey.empty())
			return "";

		strEncKey = pdata->strCrKey;
	}

	BYTE*   lpChBuf = NULL;
	BYTE pDecBase[100] = {0, };
	INT32 nDecBaseLen = 100;
	INT32   nOutLen = 0;
	tEncodeUtil.DecBase(strEncKey, pDecBase, nDecBaseLen);

	t_SeedDLLUtil->SetKey(pbCrKey);
	if(t_SeedDLLUtil->DecData(pDecBase, &lpChBuf, nOutLen))
	{
		return "";
	}
	String strDecKey = (PCHAR)lpChBuf;
	t_SeedDLLUtil->FreeBuffer(lpChBuf);

	return strDecKey;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManageHostKey::GetPkt(MemToken& RecvToken, DB_HOST_KEY& data, UINT32 nPktType)
{
	if ( RecvToken.TokenDel_String(data.strBkKey) < 0)				goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strCrKey) < 0)				goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManageHostKey::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBHostKeyItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_KEY pdata = &(begin->second);
		if(!pdata)  	continue;

		SetPkt(pdata, SendToken);
	}
	return 0;
}
//-------------------------------------------------------------------------------------

INT32		CManageHostKey::SetPkt(PDB_HOST_KEY pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_String(pdata->strBkKey);
	SendToken.TokenAdd_String(pdata->strCrKey);

	SendToken.TokenSet_Block();
    
 	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------- 



