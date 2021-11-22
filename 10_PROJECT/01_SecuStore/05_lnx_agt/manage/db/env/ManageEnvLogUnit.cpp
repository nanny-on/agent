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
#include "ManageEnvLogUnit.h"

//---------------------------------------------------------------------------

CManageEnvLogUnit*	t_ManageEnvLogUnit = NULL;

//---------------------------------------------------------------------------

CManageEnvLogUnit::CManageEnvLogUnit()
{
	t_DBMgrEnvLogUnit	= new CDBMgrEnvLogUnit();
}
//---------------------------------------------------------------------------

CManageEnvLogUnit::~CManageEnvLogUnit()
{
	if(t_DBMgrEnvLogUnit)	{	delete t_DBMgrEnvLogUnit;	t_DBMgrEnvLogUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageEnvLogUnit::LoadDBMS()
{
	TListDBEnvLogUnit tDBEnvLogUnitList;
    TListDBEnvLogUnitItor begin, end;
	if(SetER(t_DBMgrEnvLogUnit->LoadExecute(&tDBEnvLogUnitList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBEnvLogUnitList.begin();	end = tDBEnvLogUnitList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::InitHash()
{	
	String strOrgValue;

	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu 			= NULL;
		{
			if( (pdeleu = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find env_log_event_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}
		}

		{
			String strOrgValueItem = SPrintf("%s,"
									"%u,%u,%u,"
									"%u,%s,"
									"%u,%s,"
									"%u,%u,%s,"
									"%u,%s,"
									"%s,%s,", 
									GetHdrHashInfo(pdeleu).c_str(),
									pdeleu->nEvtType, pdeleu->nEvtCode, pdeleu->nEvtErrCode,
									pdeleu->nSubjectType, pdeleu->strSubjectInfo.c_str(), 
									pdeleu->nTargetType, pdeleu->strTargetInfo.c_str(),
									pdeleu->nObjectType, pdeleu->nObjectCode, pdeleu->strObjectInfo.c_str(),
									pdeleu->nOperationType, pdeleu->strEvtDescr.c_str(),
									pdeleu->strSMSValue.c_str(), pdeleu->strSMSValue.c_str());

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

String					CManageEnvLogUnit::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::AddEnvLogUnit(DB_ENV_LOG_UNIT&	deleu)
{
	if(SetER(t_DBMgrEnvLogUnit->InsertExecute(&deleu)))
    {
    	return g_nErrRtn;
    }

    AddItem(deleu.tDPH.nID, deleu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::EditEnvLogUnit(DB_ENV_LOG_UNIT&	deleu)
{
	PDB_ENV_LOG_UNIT pdeleu = FindItem(deleu.tDPH.nID);
	if(!pdeleu)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrEnvLogUnit->UpdateExecute(&deleu)))
    {
    	return g_nErrRtn;
    }

    EditItem(deleu.tDPH.nID, deleu);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::DelEnvLogUnit(UINT32 nID)
{
	PDB_ENV_LOG_UNIT pdeleu = FindItem(nID);
	if(!pdeleu)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrEnvLogUnit->DeleteExecute(pdeleu->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::ApplyEnvLogUnit(DB_ENV_LOG_UNIT&	deleu)
{
	if(FindItem(deleu.tDPH.nID))
	{
		return EditEnvLogUnit(deleu);
	}
	return AddEnvLogUnit(deleu);
}
//---------------------------------------------------------------------------

String					CManageEnvLogUnit::GetName(UINT32 nID)
{
	PDB_ENV_LOG_UNIT pdeleu = FindItem(nID);
    if(!pdeleu)	return "";
    return pdeleu->tDPH.strName;
}
//---------------------------------------------------------------------------

String					CManageEnvLogUnit::GetExtInfo(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType)
{
	TMapIDStrItor find = tdata.tIDMapStrExtInfo.find(nExtInfoType);
	if(find == tdata.tIDMapStrExtInfo.end())
		return "";

	return find->second;
}
//---------------------------------------------------------------------------

UINT32					CManageEnvLogUnit::GetExtInfoInt(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType)
{
	TMapIDStrItor find = tdata.tIDMapStrExtInfo.find(nExtInfoType);
	if(find == tdata.tIDMapStrExtInfo.end())
		return 0;

	return atoi(find->second.c_str());
}
//---------------------------------------------------------------------------

UINT32					CManageEnvLogUnit::IsExistExtInfo(DB_ENV_LOG_UNIT& tdata, UINT32 nExtInfoType)
{
	TMapIDStrItor find = tdata.tIDMapStrExtInfo.find(nExtInfoType);
	if(find == tdata.tIDMapStrExtInfo.end())
		return 0;

	return 1;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::GetExtInfoByDp(DB_ENV_LOG_UNIT& tdelu, DB_LOG_DEPLOY_FILE& tdldf)
{
	INT32 nRtn = 0;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DP_END_REASON) 
		|| (tdldf.nEndReason & GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DP_END_REASON)))
		return 1;

	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::GetExtInfoByDv(DB_ENV_LOG_UNIT& tdelu, DB_LOG_DEVICE& tdld)
{
	INT32 nRtn = 0;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DV_BLOCK_TYPE) 
		|| (tdld.nBlockType == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DV_BLOCK_TYPE)))
		return 1;

/*
	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DV_DV_INST_ID) 
		|| (tdld.strDvInstanceID == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_DV_DV_INST_ID)))
		return 1;
*/
	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::GetExtInfoByPatch(DB_ENV_LOG_UNIT& tdelu, DB_LOG_PATCH& tdlp)
{
	INT32 nRtn = 0;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_ID) 
		|| (tdlp.nPatchID == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_ID)))
		return 1;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_STATUS) 
		|| (tdlp.nPatchStatus == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_STATUS)))
		return 1;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_TYPE) 
		|| (tdlp.nPatchType == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_PATCH_TYPE)))
		return 1;

	return nRtn;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::GetExtInfoByRs(DB_ENV_LOG_UNIT& tdelu, DB_LOG_RS& tdlr)
{
	INT32 nRtn = 0;

	if(!IsExistExtInfo(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_RS_BLOCK_TYPE) 
		|| (tdlr.nBlockType == GetExtInfoInt(tdelu, SS_ENV_LOG_EXT_INFO_TYPE_RS_BLOCK_TYPE)))
		return 1;

	return nRtn;
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogEvtUnit(PDB_LOG_EVENT pdle)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;	
	
	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_EVENT) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdle->nEvtErrCode))	||
			(pdeleu_chk->nSubjectType		&& (pdeleu_chk->nSubjectType != pdle->nSubjectType))	||
			(pdeleu_chk->nTargetType		&& (pdeleu_chk->nTargetType != pdle->nTargetType))		||
			(pdeleu_chk->nObjectType		&& (pdeleu_chk->nObjectType != pdle->nObjectType))		||
			(pdeleu_chk->nObjectCode		&& (pdeleu_chk->nObjectCode != pdle->nObjectCode))		||
			(pdeleu_chk->nOperationType		&& (pdeleu_chk->nOperationType != pdle->nOperationType))	)
		{
			continue;
		}

		if((pdeleu_chk->strSubjectInfo.empty() == FALSE	&& StrMatchSpec(pdle->strSubjectInfo.c_str(), pdeleu_chk->strSubjectInfo.c_str()) == 0) ||
			(pdeleu_chk->strTargetInfo.empty() == FALSE	&& StrMatchSpec(pdle->strTargetInfo.c_str(), pdeleu_chk->strTargetInfo.c_str()) == 0)	||
			(pdeleu_chk->strObjectInfo.empty() == FALSE	&& StrMatchSpec(pdle->strObjectInfo.c_str(), pdeleu_chk->strObjectInfo.c_str()) == 0)	||
			(pdeleu_chk->strEvtDescr.empty() == FALSE		&& StrMatchSpec(pdle->strEventDescr.c_str(), pdeleu_chk->strEvtDescr.c_str()) == 0))
		{
			continue;
		}


		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogDocUnit(PDB_LOG_DOC pdld)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_DOC) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdld->nEvtErrCode)) ||
			(pdeleu_chk->nOperationType		&& (pdeleu_chk->nOperationType != pdld->nOpType)))
		{
			continue;
		}

		String strSubInfo, strObjInfo, strConvertSubInfo, strConvertObjInfo;
		strSubInfo = SPrintf("%s/%s", pdld->strSubjectPath.c_str(), pdld->strSubjectName.c_str());
		strObjInfo = SPrintf("%s/%s", pdld->strObjectPath.c_str(), pdld->strObjectName.c_str());

		strConvertSubInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strSubjectInfo, 0);
		strConvertObjInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strObjectInfo, 0);

		if((pdeleu_chk->strSubjectInfo.empty() == FALSE	&& StrMatchSpec(strSubInfo.c_str(), strConvertSubInfo.c_str()) == 0) ||
			(pdeleu_chk->strObjectInfo.empty() == FALSE	&& StrMatchSpec(strObjInfo.c_str(), strConvertObjInfo.c_str()) == 0))
		{
			continue;
		}

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogSecuUnit(PDB_LOG_SECU pdls)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;	

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_SECU) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdls->nEvtErrCode)) ||
			(pdeleu_chk->nOperationType		&& (pdeleu_chk->nOperationType != pdls->nOpType)))
		{
			continue;
		}

		String strSubInfo, strObjInfo, strConvertSubInfo, strConvertObjInfo;
		strSubInfo = SPrintf("%s/%s", pdls->strSubjectPath.c_str(), pdls->strSubjectName.c_str());
		strObjInfo = SPrintf("%s/%s", pdls->strObjectPath.c_str(), pdls->strObjectName.c_str());

		strConvertSubInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strSubjectInfo, 0);
		strConvertObjInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strObjectInfo, 0);

		if((pdeleu_chk->strSubjectInfo.empty() == FALSE	&& StrMatchSpec(strSubInfo.c_str(), strConvertSubInfo.c_str()) == 0) ||
			(pdeleu_chk->strObjectInfo.empty() == FALSE	&& StrMatchSpec(strObjInfo.c_str(), strConvertObjInfo.c_str()) == 0))
		{
			continue;
		}

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogDeployFileUnit(PDB_LOG_DEPLOY_FILE pdldf)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;	

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_DEPLOY_FILE) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdldf->nEvtErrCode)))
		{
			continue;
		}

		String strObjInfo, strConvertObjInfo;
		PDB_PO_DEPLOY_FILE_UNIT pdpdfu = t_ManagePoDeployFileUnit->FindItem(pdldf->nUnitID);
		if(pdpdfu)
		{
			TListID tPkgIDList;
			t_ManagePoDeployFileUnitInfoPkg->GetItemIDListByPoID(pdpdfu->tDPH.nID, tPkgIDList);

			{
				TListIDItor begin, end;
				begin = tPkgIDList.begin();	end = tPkgIDList.end();
				for(begin; begin != end; begin++)
				{
					PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG pdpdfui = t_ManagePoDeployFileUnitInfoPkg->FindItem(*begin);
					if(!pdpdfui)	continue;

					PDB_PO_DEPLOY_FILE_INFO_UNIT pdpdfiu = t_ManagePoDeployFileInfoUnit->FindItem(pdpdfui->tDPH.nUnitID);
					if(!pdpdfu)	continue;

					strObjInfo = pdpdfiu->strFileOrName;
					break;
				}
			}
		}

		strConvertObjInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strObjectInfo, 0);

		if((pdeleu_chk->strObjectInfo.empty() == FALSE	&& StrMatchSpec(strObjInfo.c_str(), strConvertObjInfo.c_str()) == 0))
		{
			continue;
		}

		if(!GetExtInfoByDp(*pdeleu_chk, *pdldf))
			continue;

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogPatchUnit(PDB_LOG_PATCH pdlp)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_PATCH) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdlp->nEvtErrCode)) ||
			(pdeleu_chk->nOperationType		&& (pdeleu_chk->nOperationType != pdlp->nPatchStatus)))
		{
			continue;
		}

		if(!GetExtInfoByPatch(*pdeleu_chk, *pdlp))
			continue;

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogDeviceUnit(PDB_LOG_DEVICE pdld)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;	

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_DEVICE) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdld->nEvtErrCode)))
		{
			continue;
		}

		if(!GetExtInfoByDv(*pdeleu_chk, *pdld))
			continue;

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------

PDB_ENV_LOG_UNIT	CManageEnvLogUnit::FindRecordLogRsUnit(PDB_LOG_RS pdata)
{
	UINT32 nRtnID = 0;
	UINT32 nAdminLevel = 0;	

	CFileUtil tFileUtil;

	TMapDBEnvLogUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_ENV_LOG_UNIT pdeleu_chk = &(begin->second);
		if(	(pdeleu_chk->nEvtType			&& ((pdeleu_chk->nEvtType & SS_ENV_LOG_TYPE_RS) == 0))	||
			(pdeleu_chk->nEvtErrCode		&& (pdeleu_chk->nEvtErrCode != pdata->nEvtErrCode)) ||
			(pdeleu_chk->nOperationType		&& (pdeleu_chk->nOperationType != pdata->nOpType)))
		{
			continue;
		}

		String strSubInfo, strObjInfo, strConvertSubInfo, strConvertObjInfo;
		strSubInfo = SPrintf("%s/%s", pdata->strSubjectPath.c_str(), pdata->strSubjectName.c_str());
		strObjInfo = SPrintf("%s/%s", pdata->strObjectPath.c_str(), pdata->strObjectName.c_str());

		strConvertSubInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strSubjectInfo, 0);
		strConvertObjInfo = tFileUtil.GetEnvPathToLocalPathSingle(pdeleu_chk->strObjectInfo, 0);

		if((pdeleu_chk->strSubjectInfo.empty() == FALSE	&& StrMatchSpec(strSubInfo.c_str(), strConvertSubInfo.c_str()) == 0) ||
			(pdeleu_chk->strObjectInfo.empty() == FALSE	&& StrMatchSpec(strObjInfo.c_str(), strConvertObjInfo.c_str()) == 0))
		{
			continue;
		}

		if(!GetExtInfoByRs(*pdeleu_chk, *pdata))
			continue;

		nRtnID = pdeleu_chk->tDPH.nID;
	}

	return FindItem(nRtnID);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBEnvLogUnitItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_ENV_LOG_UNIT pdeleu = FindItem(nID);
	if(!pdeleu)	return -1;

	return SetPkt(pdeleu, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::SetPkt(PDB_ENV_LOG_UNIT pdeleu, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdeleu->tDPH);

	SendToken.TokenAdd_32(pdeleu->nEvtType);
	SendToken.TokenAdd_32(pdeleu->nEvtCode);
	SendToken.TokenAdd_32(pdeleu->nEvtErrCode);
	SendToken.TokenAdd_32(pdeleu->nSubjectType);
	SendToken.TokenAdd_String(pdeleu->strSubjectInfo);
	SendToken.TokenAdd_32(pdeleu->nTargetType);
	SendToken.TokenAdd_String(pdeleu->strTargetInfo);
	SendToken.TokenAdd_32(pdeleu->nObjectType);
	SendToken.TokenAdd_32(pdeleu->nObjectCode);
	SendToken.TokenAdd_String(pdeleu->strObjectInfo);

	SendToken.TokenAdd_32(pdeleu->nOperationType);
	SendToken.TokenAdd_String(pdeleu->strEvtDescr);

	SendToken.TokenAdd_32(pdeleu->nRecordMode);
	SendToken.TokenAdd_String(pdeleu->strSMSValue);

	SendToken.TokenAdd_IDMapStr(pdeleu->tIDMapStrExtInfo);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageEnvLogUnit::GetPkt(MemToken& RecvToken, DB_ENV_LOG_UNIT& deleu)
{
	if (!RecvToken.TokenDel_DPH(deleu.tDPH))					goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(deleu.nEvtType))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nEvtCode))					goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nEvtErrCode))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nSubjectType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deleu.strSubjectInfo) < 0)	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nTargetType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deleu.strTargetInfo) < 0)	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nObjectType))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(deleu.nObjectCode))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deleu.strObjectInfo) < 0)	goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(deleu.nOperationType))			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deleu.strEvtDescr) < 0)		goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(deleu.nRecordMode))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(deleu.strSMSValue) < 0)		goto	INVALID_PKT;

	if ( RecvToken.TokenDel_IDMapStr(deleu.tIDMapStrExtInfo) < 0)	goto	INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



