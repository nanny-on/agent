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
#include "ManagePoInDevOInfo.h"

//---------------------------------------------------------------------------

CManagePoInDevOInfo*	t_ManagePoInDevOInfo = NULL;

//---------------------------------------------------------------------------

CManagePoInDevOInfo::CManagePoInDevOInfo()
{
	t_DBMgrPoInDevOInfo	= new CDBMgrPoInDevOInfo();
}
//---------------------------------------------------------------------------

CManagePoInDevOInfo::~CManagePoInDevOInfo()
{
	if(t_DBMgrPoInDevOInfo)	{	delete t_DBMgrPoInDevOInfo;	t_DBMgrPoInDevOInfo = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInDevOInfo::LoadDBMS()
{
	TListDBPoInDevOInfo tDBPoInDevOInfoList;
    TListDBPoInDevOInfoItor begin, end;
	if(SetER(t_DBMgrPoInDevOInfo->LoadExecute(&tDBPoInDevOInfoList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoInDevOInfoList.begin();	end = tDBPoInDevOInfoList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::InitHash()
{
	String strOrgValue;

	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_IN_DEVO_INFO pdidi 			= NULL;
		{
			if( (pdidi = FindItem(*begin)) == NULL)
			{
				WriteLogE("not find po_in_devo_info by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
				return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
			}
		}

		{
			String strOrgValueItem;
			strOrgValueItem = SPrintf("%s,"
								"%s,%u,%s,%s,", 
								GetHdrHashInfo(pdidi).c_str(),
								pdidi->strDvName.c_str(), pdidi->nDvType, pdidi->strDvInstanceID.c_str(), pdidi->strDvPaInstanceID.c_str());

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

String					CManagePoInDevOInfo::GetHash()
{
	return m_strFullHash;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_DEVO_INFO pdidi 			= NULL;
	{
		if( (pdidi = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_devo_info by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%u,%s,%s,", 
							GetHdrHashInfo(pdidi).c_str(),
							pdidi->strDvName.c_str(), pdidi->nDvType, pdidi->strDvInstanceID.c_str(), pdidi->strDvPaInstanceID.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::AddPoInDevOInfo(DB_PO_IN_DEVO_INFO&	didi)
{
	if(SetER(t_DBMgrPoInDevOInfo->InsertExecute(&didi)))
    {
    	return g_nErrRtn;
    }

    AddItem(didi.tDPH.nID, didi);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::EditPoInDevOInfo(DB_PO_IN_DEVO_INFO&	didi)
{
	PDB_PO_IN_DEVO_INFO pdidi = FindItem(didi.tDPH.nID);
	if(!pdidi)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInDevOInfo->UpdateExecute(&didi)))
    {
    	return g_nErrRtn;
    }

    EditItem(didi.tDPH.nID, didi);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::DelPoInDevOInfo(UINT32 nID)
{
	PDB_PO_IN_DEVO_INFO pdidi = FindItem(nID);
	if(!pdidi)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInDevOInfo->DeleteExecute(pdidi->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

String					CManagePoInDevOInfo::GetName(UINT32 nID)
{
	PDB_PO_IN_DEVO_INFO pdidi = FindItem(nID);
    if(!pdidi)	return "";
    return pdidi->tDPH.strName;
}
//---------------------------------------------------------------------------
INT32					CManagePoInDevOInfo::ApplyPoInDevOInfo(DB_PO_IN_DEVO_INFO&	didi)
{
	if(FindItem(didi.tDPH.nID))
	{
		return EditPoInDevOInfo(didi);
	}
	return AddPoInDevOInfo(didi);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInDevOInfoItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_DEVO_INFO pdeni = FindItem(nID);
	if(!pdeni)	return -1;

	return SetPkt(pdeni, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::SetPkt(PDB_PO_IN_DEVO_INFO pdidi, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdidi->tDPH);

	SendToken.TokenAdd_64(pdidi->nPolicyType);
	SendToken.TokenAdd_String(pdidi->strDvName);
	SendToken.TokenAdd_32(pdidi->nDvType);
	SendToken.TokenAdd_String(pdidi->strDvInstanceID);
	SendToken.TokenAdd_String(pdidi->strDvPaInstanceID);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInDevOInfo::GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_INFO& didi)
{
	if (!RecvToken.TokenDel_DPH(didi.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_64(didi.nPolicyType))				goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvName) < 0)			goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(didi.nDvType))					goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvInstanceID) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvPaInstanceID) < 0)	goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



