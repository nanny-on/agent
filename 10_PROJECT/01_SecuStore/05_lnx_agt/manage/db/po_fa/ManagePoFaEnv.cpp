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
#include "ManagePoFaEnv.h"

//---------------------------------------------------------------------------

CManagePoFaEnv*	t_ManagePoFaEnv = NULL;

//---------------------------------------------------------------------------

CManagePoFaEnv::CManagePoFaEnv()
{
	t_DBMgrPoFaEnv	= new CDBMgrPoFaEnv();
}
//---------------------------------------------------------------------------

CManagePoFaEnv::~CManagePoFaEnv()
{
	if(t_DBMgrPoFaEnv)	{	delete t_DBMgrPoFaEnv;	t_DBMgrPoFaEnv = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaEnv::LoadDBMS()
{
	TListDBPoFaEnv tDBPoFaEnvList;
    TListDBPoFaEnvItor begin, end;
	if(SetER(t_DBMgrPoFaEnv->LoadExecute(&tDBPoFaEnvList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoFaEnvList.begin();	end = tDBPoFaEnvList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::InitHash()
{
	t_ManagePoFaEnvPkg->InitPkg();
	
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

INT32					CManagePoFaEnv::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_FA_ENV pdpfe 			= NULL;
	{
		if( (pdpfe = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_fa_env by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%d,%d,"
							"%d", 
							GetHdrHashInfo(pdpfe).c_str(),
							pdpfe->nSysOffMaxWorkTime, pdpfe->nNotifyInfoID,
							pdpfe->nLnxSysOffMaxWorkTime);

		{
			TMapIDItor begin, end;
			begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoFaEnvPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpfe->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::AddPoFaEnv(DB_PO_FA_ENV&	dpfe)
{
	if(SetER(t_DBMgrPoFaEnv->InsertExecute(&dpfe)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpfe.tDPH.nID, dpfe);
	InitHash(dpfe.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::EditPoFaEnv(DB_PO_FA_ENV&	dpfe)
{
	PDB_PO_FA_ENV pdpfe = FindItem(dpfe.tDPH.nID);
	if(!pdpfe)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoFaEnv->UpdateExecute(&dpfe)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpfe.tDPH.nID, dpfe);
	InitHash(dpfe.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::DelPoFaEnv(UINT32 nID)
{
	PDB_PO_FA_ENV pdpfe = FindItem(nID);
	if(!pdpfe)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoFaEnv->DeleteExecute(pdpfe->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}

INT32					CManagePoFaEnv::ClearPoFaEnv()
{
	if(SetER(t_DBMgrPoFaEnv->ClearExecute()))
	{
		return g_nErrRtn;
	}

	ClearItem();
	return 0;
}

//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::ApplyPoFaEnv(DB_PO_FA_ENV&	dpfe)
{
	if(FindItem(dpfe.tDPH.nID))
	{
		return EditPoFaEnv(dpfe);
	}
	return AddPoFaEnv(dpfe);
}
//---------------------------------------------------------------------------

String					CManagePoFaEnv::GetName(UINT32 nID)
{
	PDB_PO_FA_ENV pdpfe = FindItem(nID);
    if(!pdpfe)	return "";
    return pdpfe->tDPH.strName;
}
//---------------------------------------------------------------------------



INT32					CManagePoFaEnv::ChkMaxWorkTime(UINT32& nWorkTime)
{
	PDB_PO_FA_ENV pCurPolicy = (PDB_PO_FA_ENV)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_FA_ENV);
	if(!pCurPolicy)
		return 0;

	UINT32 nChkTime = (GetCurrentDateTimeInt() - t_EnvInfoOp->m_nSysOffTime) / TIMER_INTERVAL_TIME_MIN;
	nWorkTime = nChkTime;
	if(nChkTime < pCurPolicy->nLnxSysOffMaxWorkTime)
		return 1;

	return 0;
}

INT32					CManagePoFaEnv::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_FA_ENV_PKG pdpfep = t_ManagePoFaEnvPkg->FindItem(begin->first);
		if(!pdpfep)
			continue;

		PDB_PO_FA_ENV_UNIT pdpfeu = t_ManagePoFaEnvUnit->FindItem(pdpfep->tDPH.nUnitID);
		if(!pdpfeu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpfeu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpfep->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoFaEnvItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV pdpfe = FindItem(nID);
	if(!pdpfe)	return -1;

	SetPkt(pdpfe, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpfe->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaEnvPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV pdpfe = FindItem(nID);
	if(!pdpfe)	return -1;

	SetPkt(pdpfe, SendToken);

	{
		SendToken.TokenAdd_32(pdpfe->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpfe->tDPH.tSubIDMap.begin();	end = pdpfe->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaEnvPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_FA_ENV pdpfe = FindItem(nID);
	if(!pdpfe)	return -1;

	SetPkt(pdpfe, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpfe->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoFaEnvPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::SetPkt(PDB_PO_FA_ENV pdpfe, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpfe->tDPH);

	SendToken.TokenAdd_32(pdpfe->nSysOffMaxWorkTime);
	SendToken.TokenAdd_32(pdpfe->nNotifyInfoID);

	// linux
	SendToken.TokenAdd_32(pdpfe->nLnxSysOffMaxWorkTime);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoFaEnv::GetPkt(MemToken& RecvToken, DB_PO_FA_ENV& dpfe)
{
	if (!RecvToken.TokenDel_DPH(dpfe.tDPH))				goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(dpfe.nSysOffMaxWorkTime))	goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpfe.nNotifyInfoID))			goto	INVALID_PKT;

	// linux
	if (!RecvToken.TokenDel_32(dpfe.nLnxSysOffMaxWorkTime))			goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



