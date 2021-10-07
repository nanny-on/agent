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
#include "ManagePoInPtnSPRule.h"

//---------------------------------------------------------------------------

CManagePoInPtnSPRule*	t_ManagePoInPtnSPRule = NULL;

//---------------------------------------------------------------------------

CManagePoInPtnSPRule::CManagePoInPtnSPRule()
{
	t_DBMgrPoInPtnSPRule	= new CDBMgrPoInPtnSPRule();
}
//---------------------------------------------------------------------------

CManagePoInPtnSPRule::~CManagePoInPtnSPRule()
{
	if(t_DBMgrPoInPtnSPRule)	{	delete t_DBMgrPoInPtnSPRule;	t_DBMgrPoInPtnSPRule = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInPtnSPRule::LoadDBMS()
{
	TListDBPoInPtnSPRule tDBPoInPtnSPRuleList;
    TListDBPoInPtnSPRuleItor begin, end;
	if(SetER(t_DBMgrPoInPtnSPRule->LoadExecute(&tDBPoInPtnSPRuleList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInPtnSPRuleList.begin();	end = tDBPoInPtnSPRuleList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);

		AddPosSKeyID(begin->strFileKey, begin->tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_KEY);
		{
			String strKey;
			strKey = SPrintf("%s_%s_%s", begin->strFileKey.c_str(), begin->strFilePath.c_str(), begin->strFileName.c_str());
//			strKey.MakeLower();
			AddPosSKeyID(strKey, begin->tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY);
		}
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::InitHash()
{
	String strOrgValue;
	m_strFullHash = "";

	TListID tIDList;
	TListIDItor begin, end;
	GetItemIDList(tIDList);

	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_PTN_SP_RULE pdata 			= NULL;
		{
			if( (pdata = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find po_in_ptn_sp_rule by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}

			if(pdata->tDPH.nID >= SS_PO_IN_PTN_SP_RULE_LOCAL_START_ID)
			{
				continue;
			}
		}

		{
			String strOrgValueItem;
			strOrgValueItem = SPrintf("%s,"
									"%u,%u,%u,"
									"%u,%u,"
									"%u,%u,%u,"
									"%u,"
									"%s,%s,%s,"
									"%u,%u,",
									GetHdrHashInfo(pdata).c_str(),
									pdata->nTargetType, pdata->nTargetID, pdata->nFileID,
									pdata->nReqLevel, pdata->nReqDay,
									pdata->nConfirmType, pdata->nConfirmID, pdata->nConfirmRst,
									pdata->nUseDay, 
									pdata->strFileKey.c_str(), pdata->strFilePath.c_str(), pdata->strFileName.c_str(),
									pdata->nEvtDate, pdata->nEndDate);

			strOrgValue += strOrgValueItem;
		}
	}	
	
	if(strOrgValue.empty() == FALSE)
	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		m_strFullHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInPtnSPRule::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::AddPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data)
{
	String strKey;
	if(SetER(t_DBMgrPoInPtnSPRule->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }
	strKey = SPrintf("%s_%s_%s", data.strFileKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str());

	m_tDataMutex.Lock();
    AddItem(data.tDPH.nID, data);
	AddPosSKeyID(data.strFileKey, data.tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_KEY);
	AddPosSKeyID(strKey, data.tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY);
	m_tDataMutex.UnLock();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::EditPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data)
{
	PDB_PO_IN_PTN_SP_RULE pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInPtnSPRule->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }
	m_tDataMutex.Lock();
    EditItem(data.tDPH.nID, data);
	m_tDataMutex.UnLock();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::DelPoInPtnSPRule(UINT32 nID)
{
	String strKey;
	PDB_PO_IN_PTN_SP_RULE pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInPtnSPRule->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }
	strKey = SPrintf("%s_%s_%s", pdata->strFileKey.c_str(), pdata->strFilePath.c_str(), pdata->strFileName.c_str());
	m_tDataMutex.Lock();
	DelPosSKeyID(pdata->strFileKey, POS_KEY_STR_POSITION_SP_RULE_FE_KEY);
	DelPosSKeyID(strKey, POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY);
    DeleteItem(nID);
	m_tDataMutex.UnLock();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::ApplyPoInPtnSPRule(DB_PO_IN_PTN_SP_RULE&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInPtnSPRule(data);
	}
	return AddPoInPtnSPRule(data);
}

INT32					CManagePoInPtnSPRule::GetItemCopy(TListDBPoInPtnSPRule& tRuleList)
{
	TMapDBPoInPtnSPRuleItor begin, end;
	INT32 nCount = 0;
	m_tDataMutex.Lock();
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		tRuleList.push_back(begin->second);
	}
	nCount = tRuleList.size();
	m_tDataMutex.UnLock();
	return nCount;
}


//---------------------------------------------------------------------------

String					CManagePoInPtnSPRule::GetName(UINT32 nID)
{
	PDB_PO_IN_PTN_SP_RULE pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

UINT32					CManagePoInPtnSPRule::GetNextLocalID()
{
	UINT32 nNextID = SS_PO_IN_PTN_SP_RULE_LOCAL_START_ID;
	TMapDBPoInPtnSPRuleItor begin, end;
	m_tDataMutex.Lock();
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->first >= nNextID)		nNextID += 1;
	}
	m_tDataMutex.UnLock();
	return nNextID;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::SetPkt(MemToken& SendToken)
{
	TMapDBPoInPtnSPRuleItor begin, end;
	m_tDataMutex.Lock();
	SendToken.TokenAdd_32(m_tMap.size());
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
	m_tDataMutex.UnLock();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_PTN_SP_RULE pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::SetPkt(PDB_PO_IN_PTN_SP_RULE pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_32(pdata->nTargetID);
	SendToken.TokenAdd_32(pdata->nTargetType);
	SendToken.TokenAdd_32(pdata->nFileID);

	SendToken.TokenAdd_32(pdata->nReqLevel);
	SendToken.TokenAdd_32(pdata->nReqDay);

	SendToken.TokenAdd_32(pdata->nConfirmType);
	SendToken.TokenAdd_32(pdata->nConfirmID);
	SendToken.TokenAdd_32(pdata->nConfirmRst);

	SendToken.TokenAdd_32(pdata->nUseDay);

	SendToken.TokenAdd_String(pdata->strFileKey);
	SendToken.TokenAdd_String(pdata->strFilePath);
	SendToken.TokenAdd_String(pdata->strFileName);

	SendToken.TokenAdd_32(pdata->nEvtDate);
	SendToken.TokenAdd_32(pdata->nEndDate);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInPtnSPRule::GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_SP_RULE& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))						goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nTargetID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nTargetType))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nFileID))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nReqLevel))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nReqDay))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nConfirmType))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nConfirmID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nConfirmRst))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nUseDay))					goto INVALID_PKT;

	if( RecvToken.TokenDel_String(data.strFileKey) < 0)			goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strFilePath) < 0)		goto INVALID_PKT;
	if( RecvToken.TokenDel_String(data.strFileName) < 0)		goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(data.nEvtDate))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(data.nEndDate))					goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32			CManagePoInPtnSPRule::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBPoInPtnSPRuleItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->first < SS_PO_IN_PTN_SP_RULE_LOCAL_START_ID)			continue;

			tIDList.push_back(&(begin->second));
		}

		if(tIDList.empty())	return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------



