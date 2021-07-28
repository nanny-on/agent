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
#include "ManagePoHostWebSock.h"

//---------------------------------------------------------------------------

CManagePoHostWebSock*	t_ManagePoHostWebSock = NULL;

//---------------------------------------------------------------------------

CManagePoHostWebSock::CManagePoHostWebSock()
{
	t_DBMgrPoHostWebSock	= new CDBMgrPoHostWebSock();
}
//---------------------------------------------------------------------------

CManagePoHostWebSock::~CManagePoHostWebSock()
{
	if(t_DBMgrPoHostWebSock)	{	delete t_DBMgrPoHostWebSock;	t_DBMgrPoHostWebSock = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostWebSock::LoadDBMS()
{
	TListDBPoHostWebSock tDBPoHostWebSockList;
    TListDBPoHostWebSockItor begin, end;
	if(SetER(t_DBMgrPoHostWebSock->LoadExecute(&tDBPoHostWebSockList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostWebSockList.begin();	end = tDBPoHostWebSockList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::InitHash()
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

INT32					CManagePoHostWebSock::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_HOST_WEB_SOCK pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_web_sock by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strPolicyType, strPolicyOpt;
		{
			MapToHex_64(pdata->tPolicyTypeMap,	strPolicyType,	SS_ADMIN_RIGHT_TYPE_CLASS_NUM_POLICY);
			MapToHex_64(pdata->tPolicyOptMap,	strPolicyOpt,	SS_POLICY_INDEX_TOTAL_NUMBER);
		}

		strOrgValue = SPrintf("%s,"
							"%u,%s,"
							"%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->nSvrPort, pdata->strWebKey.c_str(), 
							strPolicyType.c_str(), strPolicyOpt.c_str());
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdata->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::AddPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data)
{
	if(SetER(t_DBMgrPoHostWebSock->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::EditPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data)
{
	PDB_PO_HOST_WEB_SOCK pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostWebSock->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);
	InitHash(data.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::DelPoHostWebSock(UINT32 nID)
{
	PDB_PO_HOST_WEB_SOCK pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostWebSock->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::ApplyPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoHostWebSock(data);
	}
	return AddPoHostWebSock(data);
}
//---------------------------------------------------------------------------

String					CManagePoHostWebSock::GetName(UINT32 nID)
{
	PDB_PO_HOST_WEB_SOCK pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

UINT32					CManagePoHostWebSock::SetPolicyType(TMapID64& tTypeMap, UINT64 nPolicy)
{
	UINT32 nPolPos = GET_RIGHT_POS(nPolicy);
	UINT64 nPolValue = GET_RIGHT_VALUE(nPolicy);

	TMapID64Itor find = tTypeMap.find(nPolPos);
	if(find == tTypeMap.end())
	{
		tTypeMap[nPolPos] = 0;
		find = tTypeMap.find(nPolPos);
	}

	find->second |= nPolValue;
	return 0;
}
//---------------------------------------------------------------------------

UINT32					CManagePoHostWebSock::DelPolicyType(TMapID64& tTypeMap, UINT64 nPolicy)
{
	UINT32 nPolPos = GET_RIGHT_POS(nPolicy);
	UINT64 nPolValue = GET_RIGHT_VALUE(nPolicy);

	TMapID64Itor find = tTypeMap.find(nPolPos);
	if(find == tTypeMap.end())
	{
		return 0;
	}

	if(find->second & nPolValue)	find->second -= nPolValue;

	return 0;
}
//---------------------------------------------------------------------------

UINT32					CManagePoHostWebSock::GetPolicyType(TMapID64& tTypeMap, UINT64 nPolicy)
{
	UINT32 nPolPos = GET_RIGHT_POS(nPolicy);
	UINT64 nPolValue = GET_RIGHT_VALUE(nPolicy);

	TMapID64Itor find = tTypeMap.find(nPolPos);
	if(find == tTypeMap.end())
	{
		return STATUS_USED_MODE_OFF;
	}

	if(find->second & nPolValue)	return STATUS_USED_MODE_ON;

	return STATUS_USED_MODE_OFF;
}
//---------------------------------------------------------------------------


UINT64					CManagePoHostWebSock::GetPolicyOption(TMapID64& tOptMap, UINT64 nPolicy)
{
//	UINT32 nPolIndex = t_DeployPolicyUtil->GetPoIdxFromPoType(nPolicy);
	TMapID64Itor find = tOptMap.find(nPolicy);
	if(find == tOptMap.end())	return 0;

	return find->second;
}
//---------------------------------------------------------------------------

UINT64					CManagePoHostWebSock::SetPolicyOption(TMapID64& tOptMap, UINT64 nPolicy, UINT64 nOpt)
{
	UINT32 nPolIndex = t_DeployPolicyUtil->GetPoIdxFromPoType(nPolicy);
	TMapID64Itor find = tOptMap.find(nPolicy);
	if(find == tOptMap.end())
	{
		tOptMap[nPolIndex] = nOpt;
		return 0;
	}

	find->second = nOpt;

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::AddConWeb(UINT32 nCliPort)
{
	m_tConWebMap[nCliPort] = GetCurrentDateTimeInt();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::DelConWeb(UINT32 nCliPort)
{
	m_tConWebMap.erase(nCliPort);
	return m_tConWebMap.size();
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::CntConWeb()
{	
	return m_tConWebMap.size();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostWebSockItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_WEB_SOCK pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::SetPkt(PDB_PO_HOST_WEB_SOCK pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nSvrPort);
	SendToken.TokenAdd_String(pdata->strWebKey);

	SendToken.TokenAdd_ID64Map(pdata->tPolicyTypeMap);
	SendToken.TokenAdd_ID64Map(pdata->tPolicyOptMap);	

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostWebSock::GetPkt(MemToken& RecvToken, DB_PO_HOST_WEB_SOCK& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nSvrPort))					goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strWebKey) < 0)			goto	INVALID_PKT;

	if ( RecvToken.TokenDel_ID64Map(data.tPolicyTypeMap) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(data.tPolicyOptMap) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



