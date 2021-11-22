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
#include "ManageEnvNotifyInfo.h"

//---------------------------------------------------------------------------

CManageEnvNotifyInfo*	t_ManageEnvNotifyInfo = NULL;

//---------------------------------------------------------------------------

CManageEnvNotifyInfo::CManageEnvNotifyInfo()
{
	t_DBMgrEnvNotifyInfo	= new CDBMgrEnvNotifyInfo();
}
//---------------------------------------------------------------------------

CManageEnvNotifyInfo::~CManageEnvNotifyInfo()
{
	if(t_DBMgrEnvNotifyInfo)	{	delete t_DBMgrEnvNotifyInfo;	t_DBMgrEnvNotifyInfo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvNotifyInfo::LoadDBMS()
{
	TListDBEnvNotifyInfo tDBEnvNotifyInfoList;
    TListDBEnvNotifyInfoItor begin, end;
	if(SetER(t_DBMgrEnvNotifyInfo->LoadExecute(&tDBEnvNotifyInfoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBEnvNotifyInfoList.begin();	end = tDBEnvNotifyInfoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::InitHash()
{
	String strOrgValue;

	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_NOTIFY_INFO pdeni 			= NULL;
		{
			if( (pdeni = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find env_notify_info by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}
		}

		{
			String strOrgValueItem = SPrintf("%s,"
								"%llu,%llu,%u,"
								"%u,%u,%u,"
								"%u,%s,"
								"%u,"
								"%s,"
								"%u,",
								GetHdrHashInfo(pdeni).c_str(),
								pdeni->nPolicyType, pdeni->nPolicyCode, pdeni->nPosType, 
								pdeni->nShowSize, pdeni->nShowPos, pdeni->nShowTime,
								pdeni->nMsgFmtType, pdeni->strMsgInfo.c_str(), 
								pdeni->nNotifyBeforeDay,
								pdeni->strMatchInfo.c_str(),
								pdeni->nNotifySkipDay);

			strOrgValue += strOrgValueItem;
		}
	}	
	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		m_strFullHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvNotifyInfo::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_ENV_NOTIFY_INFO pdeni 			= NULL;
	{
		if( (pdeni = FindItem(nID)) == NULL)
		{
			WriteLogE("not find env_notify_info by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
						"%llu,%llu,%u,"
						"%u,%u,%u,"
						"%u,%s,"
						"%u,"
						"%s,"
						"%u,",
						GetHdrHashInfo(pdeni).c_str(),
						pdeni->nPolicyType, pdeni->nPolicyCode, pdeni->nPosType, 
						pdeni->nShowSize, pdeni->nShowPos, pdeni->nShowTime,
						pdeni->nMsgFmtType, pdeni->strMsgInfo.c_str(), 
						pdeni->nNotifyBeforeDay,
						pdeni->strMatchInfo.c_str(),
						pdeni->nNotifySkipDay);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::AddEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni)
{
	if(SetER(t_DBMgrEnvNotifyInfo->InsertExecute(&deni)))
    {
    	return g_nErrRtn;
    }

    AddItem(deni.tDPH.nID, deni);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::EditEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni)
{
	PDB_ENV_NOTIFY_INFO pdeni = FindItem(deni.tDPH.nID);
	if(!pdeni)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvNotifyInfo->UpdateExecute(&deni)))
    {
    	return g_nErrRtn;
    }

    EditItem(deni.tDPH.nID, deni);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::DelEnvNotifyInfo(UINT32 nID)
{
	PDB_ENV_NOTIFY_INFO pdeni = FindItem(nID);
	if(!pdeni)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvNotifyInfo->DeleteExecute(pdeni->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::ApplyEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni)
{
	if(FindItem(deni.tDPH.nID))
	{
		return EditEnvNotifyInfo(deni);
	}
	return AddEnvNotifyInfo(deni);
}
//---------------------------------------------------------------------------

void					CManageEnvNotifyInfo::AddEnvNotifyInfoNotiTime(UINT32 nPos, UINT32 nID, UINT32 nNotifyID, UINT32 nTimeStamp)
{
	String strSection, strKey;
	strSection = SPrintf("%d", nPos);
	strKey = SPrintf("%d:%d", nID, nNotifyID);

	t_EnvInfoOp->SetIni_NotifyInfo_TimeStamp(strSection, strKey, nTimeStamp);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::GetEnvNotifyInfoNotiTime(UINT32 nPos, UINT32 nID, UINT32 nNotifyID)
{
	String strSection, strKey;
	strSection = SPrintf("%d", nPos);
	strKey = SPrintf("%d:%d", nID, nNotifyID);

	return t_EnvInfoOp->GetIni_NotifyInfo_TimeStamp(strSection, strKey);
}

//---------------------------------------------------------------------------

String					CManageEnvNotifyInfo::GetName(UINT32 nID)
{
	PDB_ENV_NOTIFY_INFO pdeni = FindItem(nID);
    if(!pdeni)	return "";
    return pdeni->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvNotifyInfoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_NOTIFY_INFO pdeni = FindItem(nID);
	if(!pdeni)	return -1;

	return SetPkt(pdeni, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::SetPkt(PDB_ENV_NOTIFY_INFO pdeni, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdeni->tDPH);

	SendToken.TokenAdd_64(pdeni->nPolicyType);
	SendToken.TokenAdd_64(pdeni->nPolicyCode);
	SendToken.TokenAdd_32(pdeni->nPosType);
	SendToken.TokenAdd_32(pdeni->nShowSize);
	SendToken.TokenAdd_32(pdeni->nShowPos);
	SendToken.TokenAdd_32(pdeni->nShowTime);
	SendToken.TokenAdd_32(pdeni->nMsgFmtType);
	SendToken.TokenAdd_String(pdeni->strMsgInfo);
	SendToken.TokenAdd_32(pdeni->nNotifyBeforeDay);

	SendToken.TokenAdd_String(pdeni->strMatchInfo);
	SendToken.TokenAdd_32(pdeni->nNotifySkipDay);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::GetPkt(MemToken& RecvToken, DB_ENV_NOTIFY_INFO& deni)
{
	if (!RecvToken.TokenDel_DPH(deni.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(deni.nPolicyType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_64(deni.nPolicyCode))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nPosType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nShowSize))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nShowPos))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nShowTime))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nMsgFmtType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deni.strMsgInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nNotifyBeforeDay))			goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(deni.strMatchInfo) < 0)		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deni.nNotifySkipDay))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::IsExcludeNotifySchdule(UINT32 nPoType, UINT32 nPoUnitID)
{
	INT32 nRtn = 0;

	switch(nPoType)
	{
	case SS_POLICY_TYPE_FA_OP:
		{
			PDB_PO_FA_OP pdpfo = (PDB_PO_FA_OP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_OP);
			if(!pdpfo)
				break;

			if(pdpfo->tDPH.nExtOption & SS_PO_FA_OP_OPTION_FLAG_NOTIFY_EXCEPT_SCH_INFO)
				nRtn = 1;

			break;
		}
	case SS_POLICY_TYPE_FA_CLEAR:
		{
			PDB_PO_FA_CLEAR_UNIT pdpfcu = t_ManagePoFaClearUnit->FindItem(nPoUnitID);
			if(!pdpfcu)
				break;

			// 20200728 add jhjung
			if(!(t_EnvInfo->m_nHostSysType & pdpfcu->tDPH.nTargetOS))
			{
				break;				
			}

			if(pdpfcu->tDPH.nExtOption & SS_PO_FA_CLEAR_UNIT_OPTION_FLAG_NOTIFY_EXCEPT_SCH_INFO)
				nRtn = 1;

			break;
		}
	}

	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CManageEnvNotifyInfo::GetIDLIstByNotifyPolType(UINT32 nNotifyPolType, TListID& tIDList)
{
	switch(nNotifyPolType)
	{
	case SS_POLICY_TYPE_FA_NOTIFY:
		{
			PDB_PO_FA_NOTIFY ppfn = (PDB_PO_FA_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_NOTIFY);
			if(!ppfn)
			{
				WriteLogE("get notify_info list fail : not find cur policy [po_fa_notify]");
				return -1;
			}

			if(ppfn->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
				return -1;

			t_ManagePoFaNotifyPkg->GetKeyIDList(ppfn->tDPH.nID, tIDList);
			break;
		}
	case SS_POLICY_TYPE_FE_NOTIFY:
		{
			PDB_PO_FE_NOTIFY ppfen = (PDB_PO_FE_NOTIFY)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FE_NOTIFY);
			if(!ppfen)
			{
				WriteLogE("get notify_info list fail : not find cur policy [po_fe_notify]");
				return -1;
			}

			if(ppfen->tDPH.nUsedMode == STATUS_USED_MODE_OFF)
				return -1;

			t_ManagePoFeNotifyPkg->GetKeyIDList(ppfen->tDPH.nID, tIDList);

			break;
		}
	default:
		{
			WriteLogE("get notify_info list fail by invalid notify_type [%d]", nNotifyPolType);
			return -1;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------


