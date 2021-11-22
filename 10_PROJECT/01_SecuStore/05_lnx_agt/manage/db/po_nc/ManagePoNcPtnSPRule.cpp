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
#include "ManagePoNcPtnSPRule.h"

//---------------------------------------------------------------------------

CManagePoNcPtnSPRule*	t_ManagePoNcPtnSPRule = NULL;

//---------------------------------------------------------------------------

CManagePoNcPtnSPRule::CManagePoNcPtnSPRule()
{
	t_DBMgrPoNcPtnSPRule	= new CDBMgrPoNcPtnSPRule();
}
//---------------------------------------------------------------------------

CManagePoNcPtnSPRule::~CManagePoNcPtnSPRule()
{
	if(t_DBMgrPoNcPtnSPRule)	{	delete t_DBMgrPoNcPtnSPRule;	t_DBMgrPoNcPtnSPRule = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoNcPtnSPRule::LoadDBMS()
{
	TListDBPoNcPtnSPRule tDBPoNcPtnSPRuleList;
    TListDBPoNcPtnSPRuleItor begin, end;
	if(SetER(t_DBMgrPoNcPtnSPRule->LoadExecute(&tDBPoNcPtnSPRuleList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoNcPtnSPRuleList.begin();	end = tDBPoNcPtnSPRuleList.end();
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

INT32					CManagePoNcPtnSPRule::InitHash()
{
	String strOrgValue;
	m_strFullHash = "";

	TListID tIDList;
	TListIDItor begin, end;
	GetItemIDList(tIDList);

	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_NC_PTN_SP_RULE pdata 			= NULL;
		{
			if( (pdata = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find po_nc_ptn_sp_rule by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}

			if(pdata->tDPH.nID >= SS_PO_NC_PTN_SP_RULE_LOCAL_START_ID)
			{
				continue;
			}
		}

		{
			String strOrgValueItem;
			strOrgValueItem = SPrintf( "%s,"
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

String					CManagePoNcPtnSPRule::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::AddPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data)
{
	if(SetER(t_DBMgrPoNcPtnSPRule->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

    AddItem(data.tDPH.nID, data);
	AddPosSKeyID(data.strFileKey, data.tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_KEY);
	{
		String strKey;
		strKey = SPrintf("%s_%s_%s", data.strFileKey.c_str(), data.strFilePath.c_str(), data.strFileName.c_str());
//		strKey.MakeLower();
		AddPosSKeyID(strKey, data.tDPH.nID, POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY);
	}

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::EditPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data)
{
	PDB_PO_NC_PTN_SP_RULE pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoNcPtnSPRule->UpdateExecute(&data)))
    {
    	return g_nErrRtn;
    }

    EditItem(data.tDPH.nID, data);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::DelPoNcPtnSPRule(UINT32 nID)
{
	PDB_PO_NC_PTN_SP_RULE pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoNcPtnSPRule->DeleteExecute(pdata->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

	DelPosSKeyID(pdata->strFileKey, POS_KEY_STR_POSITION_SP_RULE_FE_KEY);
	{
		String strKey;
		strKey = SPrintf("%s_%s_%s", pdata->strFileKey.c_str(), pdata->strFilePath.c_str(), pdata->strFileName.c_str());
//		strKey.MakeLower();
		DelPosSKeyID(strKey, POS_KEY_STR_POSITION_SP_RULE_FE_FILE_PATH_KEY);
	}

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::ApplyPoNcPtnSPRule(DB_PO_NC_PTN_SP_RULE&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoNcPtnSPRule(data);
	}
	return AddPoNcPtnSPRule(data);
}
//---------------------------------------------------------------------------

String					CManagePoNcPtnSPRule::GetName(UINT32 nID)
{
	PDB_PO_NC_PTN_SP_RULE pdata = FindItem(nID);
    if(!pdata)	return "";
    return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------

UINT32					CManagePoNcPtnSPRule::GetNextLocalID()
{
	UINT32 nNextID = SS_PO_NC_PTN_SP_RULE_LOCAL_START_ID;
	TMapDBPoNcPtnSPRuleItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->first >= nNextID)		nNextID += 1;
	}
	return nNextID;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoNcPtnSPRuleItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_NC_PTN_SP_RULE pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoNcPtnSPRule::SetPkt(PDB_PO_NC_PTN_SP_RULE pdata, MemToken& SendToken)
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

INT32					CManagePoNcPtnSPRule::GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_SP_RULE& data)
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

INT32			CManagePoNcPtnSPRule::SetPktSync(TListPVOID& tIDList)
{
	{
		TMapDBPoNcPtnSPRuleItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->first < SS_PO_NC_PTN_SP_RULE_LOCAL_START_ID)			continue;

			tIDList.push_back(&(begin->second));
		}

		if(tIDList.empty())	return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------



