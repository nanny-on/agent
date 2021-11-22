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
#include "ManageEnvLog.h"

//---------------------------------------------------------------------------

CManageEnvLog*	t_ManageEnvLog = NULL;

//---------------------------------------------------------------------------

CManageEnvLog::CManageEnvLog()
{
	t_DBMgrEnvLog	= new CDBMgrEnvLog();
}
//---------------------------------------------------------------------------

CManageEnvLog::~CManageEnvLog()
{
	if(t_DBMgrEnvLog)	{	delete t_DBMgrEnvLog;	t_DBMgrEnvLog = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvLog::LoadDBMS()
{
	DB_ENV_LOG dele;
	TListDBEnvLog tDBEnvLogList;
    TListDBEnvLogItor begin, end;
	
	if(SetER(t_DBMgrEnvLog->LoadExecute(&tDBEnvLogList)))
    {
		if(tDBEnvLogList.size() == 0)
		{
			memset(&dele, 0, sizeof(DB_ENV_LOG));
			dele.tDPH.nID = 1;
			dele.tDPH.nUsedFlag = USED_FLAG_TYPE_USED;
			AddEnvLog(dele);
		}
    	return g_nErrRtn;
    }

    begin = tDBEnvLogList.begin();	end = tDBEnvLogList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::InitHash()
{
	String strOrgValue;

	PDB_ENV_LOG pdele 			= NULL;
	{
		if( (pdele = FirstItem()) == NULL)
		{
//			WriteLogE("not find env_log_event by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strSvrLoadMode, strSvrLoadNum, strAgtLoadMode, strAgtLoadNum;
		MapToHex(pdele->tSvrLoadModeMap, strSvrLoadMode,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		MapToHex(pdele->tSvrLoadNumMap, strSvrLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		MapToHex(pdele->tAgtLoadModeMap, strAgtLoadMode, SS_ENV_LOG_INDEX_TOTAL_NUMBER);
		MapToHex(pdele->tAgtLoadNumMap, strAgtLoadNum,	SS_ENV_LOG_INDEX_TOTAL_NUMBER);

		strOrgValue = SPrintf("%s,%u,"
							"%s,%s,"
							"%s,%s,"
							"%u,%u,",
							GetHdrHashInfo(pdele).c_str(),
							pdele->nRecordMode,
							strSvrLoadMode.c_str(), strSvrLoadNum.c_str(),
							strAgtLoadMode.c_str(), strAgtLoadNum.c_str(),
							pdele->nLogClearStTime, pdele->nLogClearUseTime);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		m_strFullHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

String					CManageEnvLog::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::AddEnvLog(DB_ENV_LOG&	dele)
{
	if(SetER(t_DBMgrEnvLog->InsertExecute(&dele)))
    {
    	return g_nErrRtn;
    }

    AddItem(dele.tDPH.nID, dele);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::EditEnvLog(DB_ENV_LOG&	dele)
{
	PDB_ENV_LOG pdele = FindItem(dele.tDPH.nID);
	if(!pdele)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvLog->UpdateExecute(&dele)))
    {
    	return g_nErrRtn;
    }

    EditItem(dele.tDPH.nID, dele);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::DelEnvLog(UINT32 nID)
{
	PDB_ENV_LOG pdele = FindItem(nID);
	if(!pdele)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvLog->DeleteExecute(pdele->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::ApplyEnvLog(DB_ENV_LOG&	dele)
{
	if(FindItem(dele.tDPH.nID))
	{
		return EditEnvLog(dele);
	}
	return AddEnvLog(dele);
}
//---------------------------------------------------------------------------

String					CManageEnvLog::GetName(UINT32 nID)
{
	PDB_ENV_LOG pdele = FindItem(nID);
    if(!pdele)	return "";
    return pdele->tDPH.strName;
}
//---------------------------------------------------------------------------

UINT32					CManageEnvLog::GetLogValue(TMapID& tValueMap, UINT32 nIdx)
{
	TMapIDItor find = tValueMap.find(nIdx);
	if(find == tValueMap.end())
	{
		return 0;
	}
	return find->second;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::IsRecordLogEvt(PDB_LOG_EVENT pdle, DB_ENV_LOG_UNIT& deleu)
{
	PDB_ENV_LOG pdele = GetDefaultItem();
	if(!pdele)	return -1;

	PDB_ENV_LOG_UNIT pdeleu = t_ManageEnvLogUnit->FindRecordLogEvtUnit(pdle);
	if(!pdeleu)
	{
		deleu.tDPH = pdele->tDPH;
		deleu.nRecordMode = pdele->nRecordMode;
	}
	else
	{
		deleu = *pdeleu;
	}
	return (deleu.tDPH.nUsedMode ? 0 : 1);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::IsRecordLogDoc(PDB_LOG_DOC pdld, DB_ENV_LOG_UNIT& deleu)
{
	PDB_ENV_LOG pdele = GetDefaultItem();
	if(!pdele)	return -1;

	PDB_ENV_LOG_UNIT pdeleu = t_ManageEnvLogUnit->FindRecordLogDocUnit(pdld);
	if(!pdeleu)
	{
		deleu.tDPH = pdele->tDPH;
		deleu.nRecordMode = pdele->nRecordMode;
	}
	else
	{
		deleu = *pdeleu;
	}
	return (deleu.tDPH.nUsedMode ? 0 : 1);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::IsRecordLogSecu(PDB_LOG_SECU pdls, DB_ENV_LOG_UNIT& deleu)
{
	PDB_ENV_LOG pdele = GetDefaultItem();
	if(!pdele)	return -1;

	PDB_ENV_LOG_UNIT pdeleu = t_ManageEnvLogUnit->FindRecordLogSecuUnit(pdls);
	if(!pdeleu)
	{
		deleu.tDPH = pdele->tDPH;
		deleu.nRecordMode = pdele->nRecordMode;
	}
	else
	{
		deleu = *pdeleu;
	}
	return (deleu.tDPH.nUsedMode ? 0 : 1);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::IsRecordLogDeployFile(PDB_LOG_DEPLOY_FILE pdldf, DB_ENV_LOG_UNIT& deleu)
{
	PDB_ENV_LOG pdele = GetDefaultItem();
	if(!pdele)	return -1;

	PDB_ENV_LOG_UNIT pdeleu = t_ManageEnvLogUnit->FindRecordLogDeployFileUnit(pdldf);
	if(!pdeleu)
	{
		deleu.tDPH = pdele->tDPH;
		deleu.nRecordMode = pdele->nRecordMode;
	}
	else
	{
		deleu = *pdeleu;
	}
	return (deleu.tDPH.nUsedMode ? 0 : 1);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::IsRecordLogPatch(PDB_LOG_PATCH pdlp, DB_ENV_LOG_UNIT& deleu)
{
	PDB_ENV_LOG pdele = GetDefaultItem();
	if(!pdele)	return -1;

	PDB_ENV_LOG_UNIT pdeleu = t_ManageEnvLogUnit->FindRecordLogPatchUnit(pdlp);
	if(!pdeleu)
	{
		deleu.tDPH = pdele->tDPH;
		deleu.nRecordMode = pdele->nRecordMode;
	}
	else
	{
		deleu = *pdeleu;
	}
	return (deleu.tDPH.nUsedMode ? 0 : 1);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvLog::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvLogItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_LOG pdele = FindItem(nID);
	if(!pdele)	return -1;

	return SetPkt(pdele, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::SetPkt(PDB_ENV_LOG pdele, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdele->tDPH);

	SendToken.TokenAdd_32(pdele->nRecordMode);
	SendToken.TokenAdd_IDMap(pdele->tSvrLoadModeMap);
	SendToken.TokenAdd_IDMap(pdele->tSvrLoadNumMap);
	SendToken.TokenAdd_IDMap(pdele->tAgtLoadModeMap);
	SendToken.TokenAdd_IDMap(pdele->tAgtLoadNumMap);

	SendToken.TokenAdd_32(pdele->nLogClearStTime);
	SendToken.TokenAdd_32(pdele->nLogClearUseTime);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLog::GetPkt(MemToken& RecvToken, DB_ENV_LOG& dele)
{
	if (!RecvToken.TokenDel_DPH(dele.tDPH))							goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dele.nRecordMode))					goto	INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dele.tSvrLoadModeMap) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dele.tSvrLoadNumMap) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dele.tAgtLoadModeMap) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_IDMap(dele.tAgtLoadNumMap) < 0)			goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dele.nLogClearStTime))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dele.nLogClearUseTime))				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



