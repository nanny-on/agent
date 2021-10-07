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
#include "ManagePtnPatch.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnPatch::CManagePtnPatch()
{
	m_tDBMgrPtnPatch	= new CDBMgrPtnPatch();

	SetSystemInfo();
}
//---------------------------------------------------------------------------

CManagePtnPatch::~CManagePtnPatch()
{
	if(m_tDBMgrPtnPatch)		{	delete m_tDBMgrPtnPatch;		m_tDBMgrPtnPatch = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatch::LoadDBMS()
{
	TListDBPtnPatch tDBPtnPatchList;
    TListDBPtnPatchItor begin, end;
	if(SetER(m_tDBMgrPtnPatch->LoadExecute(&tDBPtnPatchList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnPatchList.begin();	end = tDBPtnPatchList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
		AddKeyIDList(begin->nVulnKBID, begin->nID);

		if(begin->strVulnGuid.empty() == FALSE)
			AddPosSKeyID(begin->strVulnGuid, begin->nID, POS_KEY_STR_POSITION_PTN_PATCH_GUID_MAP);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatch::AddPtnPatch(DB_PTN_PATCH&	data)
{
	if(SetER(m_tDBMgrPtnPatch->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
	AddKeyIDList(data.nVulnKBID, data.nID);
	if(data.strVulnGuid.empty() == FALSE)
		AddPosSKeyID(data.strVulnGuid, data.nID, POS_KEY_STR_POSITION_PTN_PATCH_GUID_MAP);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatch::EditPtnPatch(DB_PTN_PATCH&	data)
{
    if(m_tDBMgrPtnPatch->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnPatch::DelPtnPatch(UINT32 nID)
{
	PDB_PTN_PATCH pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnPatch->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
	if(pdata->strVulnGuid.empty() == FALSE)
		DelPosSKeyID(pdata->strVulnGuid, POS_KEY_STR_POSITION_PTN_PATCH_GUID_MAP);

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

PDB_PTN_PATCH			CManagePtnPatch::FindPtnPatchKey(String strKey)
{
	return FindItem(FindSKeyID(strKey));
}
//---------------------------------------------------------------------------

PDB_PTN_PATCH	CManagePtnPatch::FindPtnPatchByKBID(UINT32 nKBID)
{
	TListID tIDList;
	GetKeyIDList(nKBID, tIDList);
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH pdata = FindItem(*begin);
		if(!pdata)	continue;

		if(pdata->nProcArch != m_nProcArch)					continue;
		if(pdata->strTarProduct.find(m_strOsName) == string::npos)	continue;

		return pdata;
	}
	return NULL;
}
//---------------------------------------------------------------------------

String			CManagePtnPatch::GetOsShortName(UINT64 nOsID)
{
	String strRtn = "";
	switch(nOsID)
	{
		case ASI_SYSTEM_ID_7:		strRtn = "Windows 7";					break;
		case ASI_SYSTEM_ID_28R2:	strRtn = "Windows Server 2008 R2";		break;
		case ASI_SYSTEM_ID_8:		strRtn = "Windows 8";					break;
		case ASI_SYSTEM_ID_212:		strRtn = "Windows Server 2012";			break;
		case ASI_SYSTEM_ID_81:		strRtn = "Windows 8.1";					break;
		case ASI_SYSTEM_ID_212R2:	strRtn = "Windows Server 2012 R2";		break;
		case ASI_SYSTEM_ID_10:		strRtn = "Windows 10";					break;
		case ASI_SYSTEM_ID_16:		strRtn = "Windows Server 2016";			break;
		case ASI_SYSTEM_ID_219:		strRtn = "Windows Server 2019";			break;
	}
	return strRtn;
}
//---------------------------------------------------------------------------

PDB_PTN_PATCH_LAN		CManagePtnPatch::GetLan(UINT32 nID)
{
	PDB_PTN_PATCH pdata = FindItem(nID);
	if(!pdata)
	{
		return NULL;
	}

	PDB_PTN_PATCH_LAN pdata_lan = NULL;

	UINT32 dwLocaleID = t_EnvInfoOp->m_nSysLocaleID;
	switch(dwLocaleID)
	{
		case SC_LCID_KOREA:		pdata_lan = &(pdata->tLanKor);		break;	
		default:				pdata_lan = &(pdata->tLanEng);		break;
	}
	return pdata_lan;
}
//-------------------------------------------------------------------------------------

String					CManagePtnPatch::GetName(UINT32 nID)
{
	PDB_PTN_PATCH_LAN pdata = GetLan(nID);
	if(!pdata)	return "";

	return pdata->strName;	
}
//-------------------------------------------------------------------------------------

INT32			CManagePtnPatch::SetSystemInfo()
{
//	CSystemInfo tSysInfo;
	UINT64 nSystemID = 0;
	UINT64 nProcArch = 0;
	if(t_SystemInfo != NULL)
	{
		nSystemID = t_SystemInfo->GetSystemID();
		nProcArch = t_SystemInfo->GetSysProcArchitecture();
	}
	
	m_strOsName = GetOsShortName(nSystemID);
	m_nProcArch = nProcArch;

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnPatch::GetPkt(MemToken& RecvToken, DB_PTN_PATCH& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRevID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nType))						goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nAnnounce))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.tLanKor.strName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tLanKor.strDescr) < 0)	goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.tLanEng.strName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.tLanEng.strDescr) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nRisk))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nReBoot))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRollBack))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nApplyOrder))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strVulnName) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strVulnPublisher) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strVulnGuid) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nVulnKBID))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strRefer) < 0)			goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nReplace))					goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strTarComp) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarFamily) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strTarProduct) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nProcArch))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nSrcType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnCrDate))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nPtnModDate))				goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnPatch::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnPatchItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_PATCH pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatch::SetPkt(TMapID& tIDMap, MemToken& SendToken)
{
	TListPVOID tPktList;
	{
		TMapIDItor begin, end;
		begin = tIDMap.begin();		end = tIDMap.end();
		for(begin; begin != end; begin++)
		{
			PDB_PTN_PATCH pdata = FindItem(begin->first);
			if(!pdata)	continue;

			tPktList.push_back(PVOID(pdata));
		}
	}
	if(tPktList.size() == 0)		return 0;

	{
		SendToken.TokenAdd_32(tPktList.size());

		TListPVOIDItor begin, end;
		begin = tPktList.begin();	end = tPktList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PTN_PATCH pdata = (PDB_PTN_PATCH)(*begin);
			if(!pdata)	continue;

			SetPkt(pdata, SendToken);
		}
	}
	return tPktList.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnPatch::SetPkt(PDB_PTN_PATCH pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);
	SendToken.TokenAdd_32(pdata->nRevID);
	SendToken.TokenAdd_32(pdata->nType);

	SendToken.TokenAdd_32(pdata->nAnnounce);
	SendToken.TokenAdd_String(pdata->tLanKor.strName);
	SendToken.TokenAdd_String(pdata->tLanKor.strDescr);
	SendToken.TokenAdd_String(pdata->tLanEng.strName);
	SendToken.TokenAdd_String(pdata->tLanEng.strDescr);

	SendToken.TokenAdd_32(pdata->nRisk);
	SendToken.TokenAdd_32(pdata->nReBoot);
	SendToken.TokenAdd_32(pdata->nRollBack);
	SendToken.TokenAdd_32(pdata->nApplyOrder);

	SendToken.TokenAdd_String(pdata->strVulnName);
	SendToken.TokenAdd_String(pdata->strVulnPublisher);
	SendToken.TokenAdd_String(pdata->strVulnGuid);
	SendToken.TokenAdd_32(pdata->nVulnKBID);

	SendToken.TokenAdd_String(pdata->strRefer);
	SendToken.TokenAdd_32(pdata->nReplace);

	SendToken.TokenAdd_String(pdata->strTarComp);
	SendToken.TokenAdd_String(pdata->strTarFamily);
	SendToken.TokenAdd_String(pdata->strTarProduct);
	SendToken.TokenAdd_32(pdata->nProcArch);

	SendToken.TokenAdd_32(pdata->nSrcType);
	SendToken.TokenAdd_32(pdata->nPtnCrDate);
	SendToken.TokenAdd_32(pdata->nPtnModDate);

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

void 		CManagePtnPatch::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_patch] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





