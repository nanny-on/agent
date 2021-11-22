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
#include "ManageLinkLicense.h"

//---------------------------------------------------------------------------

CManageLinkLicense*	t_ManageLinkLicense = NULL;

//---------------------------------------------------------------------------

CManageLinkLicense::CManageLinkLicense()
{
	t_DBMgrLinkLicense	= new CDBMgrLinkLicense();

	m_nSID = 1;
}
//---------------------------------------------------------------------------

CManageLinkLicense::~CManageLinkLicense()
{
	if(t_DBMgrLinkLicense)	{	delete t_DBMgrLinkLicense;	t_DBMgrLinkLicense = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageLinkLicense::LoadDBMS()
{
	TListDBLinkLicense tDBLinkLicenseList;
    TListDBLinkLicenseItor begin, end;
	if(SetER(t_DBMgrLinkLicense->LoadExecute(&tDBLinkLicenseList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBLinkLicenseList.begin();	end = tDBLinkLicenseList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::InitHash()
{
	String strOrgValue;
	return 0;
}
//---------------------------------------------------------------------------

String					CManageLinkLicense::GetHash()
{
	PDB_LINK_LICENSE pdl = FirstItem();
	if(!pdl)	return "";

	return pdl->tDPH.strHash;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::AddLinkLicense(DB_LINK_LICENSE&	dl)
{
	if(SetER(t_DBMgrLinkLicense->InsertExecute(&dl)))
    {
    	return g_nErrRtn;
    }

    AddItem(dl.tDPH.nID, dl);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::EditLinkLicense(DB_LINK_LICENSE&	dl)
{
	PDB_LINK_LICENSE pdl = FindItem(dl.tDPH.nID);
	if(!pdl)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrLinkLicense->UpdateExecute(&dl)))
    {
    	return g_nErrRtn;
    }

    EditItem(dl.tDPH.nID, dl);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::DelLinkLicense(UINT32 nID)
{
	PDB_LINK_LICENSE pdl = FindItem(nID);
	if(!pdl)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrLinkLicense->DeleteExecute(pdl->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

String					CManageLinkLicense::GetName(UINT32 nID)
{
	PDB_LINK_LICENSE pdl = FindItem(nID);
    if(!pdl)	return "";
    return pdl->strLicenseKey;
}
//---------------------------------------------------------------------------

PDB_LINK_LICENSE		CManageLinkLicense::FindLicense(String strLicense)
{
	TMapDBLinkLicenseItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.strLicenseKey.compare(strLicense))		continue;

		return &(begin->second);
	}
	return NULL;
}
//---------------------------------------------------------------------------

UINT32					CManageLinkLicense::GetNextSID()
{
	m_tSIDMap[++m_nSID] = GetCurrentDateTimeInt();
	return m_nSID;
}
//---------------------------------------------------------------------------

UINT32					CManageLinkLicense::DelSID(UINT32 nSID)
{
	m_tSIDMap.erase(nSID);
	return m_tSIDMap.size();
}
//---------------------------------------------------------------------------

UINT32					CManageLinkLicense::GetSIDList(TListID& tIDList)
{
	ConvertMapToList(m_tSIDMap, tIDList);
	return tIDList.size();
}
//---------------------------------------------------------------------------

UINT32					CManageLinkLicense::GetSIDCount()
{
	return m_tSIDMap.size();
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

INT32					CManageLinkLicense::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBLinkLicenseItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_LINK_LICENSE pdl = FindItem(nID);
	if(!pdl)	return -1;

	return SetPkt(pdl, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::SetPkt(PDB_LINK_LICENSE pdl, MemToken& SendToken)
{

	SendToken.TokenAdd_DPH(pdl->tDPH);

	SendToken.TokenAdd_String(pdl->strLicenseKey);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageLinkLicense::GetPkt(MemToken& RecvToken, DB_LINK_LICENSE& dl)
{
	if (!RecvToken.TokenDel_DPH(dl.tDPH))						goto	INVALID_PKT;

	if ( RecvToken.TokenDel_String(dl.strLicenseKey) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



