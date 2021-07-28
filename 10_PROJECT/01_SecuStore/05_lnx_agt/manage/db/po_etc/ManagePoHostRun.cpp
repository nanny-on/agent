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
#include "ManagePoHostRun.h"

//---------------------------------------------------------------------------

CManagePoHostRun*	t_ManagePoHostRun = NULL;

//---------------------------------------------------------------------------

CManagePoHostRun::CManagePoHostRun()
{
	t_DBMgrPoHostRun	= new CDBMgrPoHostRun();
}
//---------------------------------------------------------------------------

CManagePoHostRun::~CManagePoHostRun()
{
	if(t_DBMgrPoHostRun)	{	delete t_DBMgrPoHostRun;	t_DBMgrPoHostRun = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoHostRun::LoadDBMS()
{
	TListDBPoHostRun tDBPoHostRunList;
    TListDBPoHostRunItor begin, end;
	if(SetER(t_DBMgrPoHostRun->LoadExecute(&tDBPoHostRunList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoHostRunList.begin();	end = tDBPoHostRunList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::InitHash()
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

INT32					CManagePoHostRun::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_HOST_RUN pdphr 			= NULL;
	{
		if( (pdphr = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_host_run by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		String strMSOPkg, strMSOCls;
		{
			MapToHex_64(pdphr->tMgrShowOrderPkgMap,	strMSOPkg,	SS_PO_HOST_RUN_MRG_SHOW_ORDER_PKG_NUM);
			MapToHex_64(pdphr->tMgrShowOrderClsMap,	strMSOCls,	SS_PO_HOST_RUN_MRG_SHOW_ORDER_CLS_NUM);
		}

		strOrgValue = SPrintf("%s,"
							"%u,%u,"
							"%u,%u,%u,%u,"
							"%u,%u,"
							"%u,"
							"%s,%s,"
							"%u,%u,"
							"%u,%u,%u,%u,"
							"%u,%u,", 
							GetHdrHashInfo(pdphr).c_str(),
							pdphr->nRunOption, pdphr->nRmLogDay, 
							pdphr->nLgnConPeriod, pdphr->nMgrShowType, pdphr->nShowLogDay, pdphr->nBootChkType,
							pdphr->nIntegrityMode, pdphr->nCrossAuthMode,
                            pdphr->nOncePktNum,
							strMSOPkg.c_str(), strMSOCls.c_str(),
							pdphr->nConnManage, pdphr->nLogReset,
							pdphr->nLnxRunOption, pdphr->nLnxRmLogDay, pdphr->nLnxLgnConPeriod, pdphr->nLnxBootChkType,
							pdphr->nLnxOncePktNum, pdphr->nLnxConnManage);
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdphr->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::AddPoHostRun(DB_PO_HOST_RUN&	dphr)
{
	if(SetER(t_DBMgrPoHostRun->InsertExecute(&dphr)))
    {
    	return g_nErrRtn;
    }

    AddItem(dphr.tDPH.nID, dphr);
	InitHash(dphr.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::EditPoHostRun(DB_PO_HOST_RUN&	dphr)
{
	PDB_PO_HOST_RUN pdphr = FindItem(dphr.tDPH.nID);
	if(!pdphr)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoHostRun->UpdateExecute(&dphr)))
    {
    	return g_nErrRtn;
    }

    EditItem(dphr.tDPH.nID, dphr);
	InitHash(dphr.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::DelPoHostRun(UINT32 nID)
{
	PDB_PO_HOST_RUN pdphr = FindItem(nID);
	if(!pdphr)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoHostRun->DeleteExecute(pdphr->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::ClearPoHostRun()
{
	if(SetER(t_DBMgrPoHostRun->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::ApplyPoHostRun(DB_PO_HOST_RUN&	dphr)
{
	if(FindItem(dphr.tDPH.nID))
	{
		return EditPoHostRun(dphr);
	}
	return AddPoHostRun(dphr);
}
//---------------------------------------------------------------------------

String					CManagePoHostRun::GetName(UINT32 nID)
{
	PDB_PO_HOST_RUN pdphr = FindItem(nID);
    if(!pdphr)	return "";
    return pdphr->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::IsApplyPolicyFromBootTime()
{
	PDB_PO_HOST_RUN pCurPolicy = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
	if(pCurPolicy && pCurPolicy->nLnxRunOption & SS_PO_HOST_RUN_OPTION_FLAG_SUPPORT_BOOT_APPLY)
	{
		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoHostRunItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_HOST_RUN pdphr = FindItem(nID);
	if(!pdphr)	return -1;

	return SetPkt(pdphr, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::SetPkt(PDB_PO_HOST_RUN pdphr, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdphr->tDPH);

	SendToken.TokenAdd_32(pdphr->nRunOption);
	SendToken.TokenAdd_32(pdphr->nRmLogDay);
	SendToken.TokenAdd_32(pdphr->nLgnConPeriod);
	SendToken.TokenAdd_32(pdphr->nMgrShowType);
	SendToken.TokenAdd_32(pdphr->nShowLogDay);
	SendToken.TokenAdd_32(pdphr->nBootChkType);
	SendToken.TokenAdd_32(pdphr->nIntegrityMode);
	SendToken.TokenAdd_32(pdphr->nCrossAuthMode);
	SendToken.TokenAdd_32(pdphr->nOncePktNum);

	SendToken.TokenAdd_ID64Map(pdphr->tMgrShowOrderPkgMap);
	SendToken.TokenAdd_ID64Map(pdphr->tMgrShowOrderClsMap);

	SendToken.TokenAdd_32(pdphr->nConnManage);
	SendToken.TokenAdd_32(pdphr->nLogReset);
	
	// linux
	SendToken.TokenAdd_32(pdphr->nLnxRunOption);
	SendToken.TokenAdd_32(pdphr->nLnxRmLogDay);
	SendToken.TokenAdd_32(pdphr->nLnxLgnConPeriod);
	SendToken.TokenAdd_32(pdphr->nLnxBootChkType);
	SendToken.TokenAdd_32(pdphr->nLnxOncePktNum);
	SendToken.TokenAdd_32(pdphr->nLnxConnManage);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoHostRun::GetPkt(MemToken& RecvToken, DB_PO_HOST_RUN& dphr)
{
	if (!RecvToken.TokenDel_DPH(dphr.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dphr.nRunOption))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nRmLogDay))			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLgnConPeriod))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nMgrShowType))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nShowLogDay))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nBootChkType))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nIntegrityMode))	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nCrossAuthMode))	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nOncePktNum))		goto	INVALID_PKT;

	if ( RecvToken.TokenDel_ID64Map(dphr.tMgrShowOrderPkgMap) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_ID64Map(dphr.tMgrShowOrderClsMap) < 0)	goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dphr.nConnManage))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLogReset))			goto	INVALID_PKT;


	// linux
	if (!RecvToken.TokenDel_32(dphr.nLnxRunOption))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLnxRmLogDay))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLnxLgnConPeriod))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLnxBootChkType))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLnxOncePktNum))		goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dphr.nLnxConnManage))		goto	INVALID_PKT;


	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



