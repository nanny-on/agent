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
#include "ManagePoDvDefault.h"

//---------------------------------------------------------------------------

CManagePoDvDefault*	t_ManagePoDvDefault = NULL;

//---------------------------------------------------------------------------

CManagePoDvDefault::CManagePoDvDefault()
{
	t_DBMgrPoDvDefault	= new CDBMgrPoDvDefault();
}
//---------------------------------------------------------------------------

CManagePoDvDefault::~CManagePoDvDefault()
{
	if(t_DBMgrPoDvDefault)	{	delete t_DBMgrPoDvDefault;	t_DBMgrPoDvDefault = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoDvDefault::LoadDBMS()
{
	TListDBPoDvDefault tDBPoDvDefaultList;
    TListDBPoDvDefaultItor begin, end;
	if(SetER(t_DBMgrPoDvDefault->LoadExecute(&tDBPoDvDefaultList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoDvDefaultList.begin();	end = tDBPoDvDefaultList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::AddPoDvDefault(DB_PO_DV_DEFAULT&	didi)
{
	if(SetER(t_DBMgrPoDvDefault->InsertExecute(&didi)))
    {
    	return g_nErrRtn;
    }

    AddItem(didi.tDPH.nID, didi);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::EditPoDvDefault(DB_PO_DV_DEFAULT&	didi)
{
	PDB_PO_DV_DEFAULT pdidi = FindItem(didi.tDPH.nID);
	if(!pdidi)	return ERR_DBMS_UPDATE_FAIL;
	
	if(SetER(t_DBMgrPoDvDefault->UpdateExecute(&didi)))
    {
    	return g_nErrRtn;
    }

    EditItem(didi.tDPH.nID, didi);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::DelPoDvDefault(UINT32 nID)
{
	PDB_PO_DV_DEFAULT pdidi = FindItem(nID);
	if(!pdidi)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoDvDefault->DeleteExecute(pdidi->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
INT32					CManagePoDvDefault::DeleteDvDefault(UINT32 nID)
{
	PDB_PO_DV_DEFAULT pdidi = FindItem(nID);
	if(!pdidi)	return ERR_DBMS_DELETE_FAIL;

	pdidi->tDPH.nUsedMode = 0;

	return EditPoDvDefault(*pdidi);
}
//---------------------------------------------------------------------------

String					CManagePoDvDefault::GetName(UINT32 nID)
{
	PDB_PO_DV_DEFAULT pdidi = FindItem(nID);
    if(!pdidi)	return "";
    return pdidi->tDPH.strName;
}
//---------------------------------------------------------------------------
INT32					CManagePoDvDefault::ApplyPoDvDefault(DB_PO_DV_DEFAULT&	didi)
{
	if(FindItem(didi.tDPH.nID))
	{
		return EditPoDvDefault(didi);
	}
	return AddPoDvDefault(didi);
}
//---------------------------------------------------------------------------
INT32					CManagePoDvDefault::ResetPoDvDefault()
{

	return 0;
}
//---------------------------------------------------------------------------
String					CManagePoDvDefault::FindDvPdName(TListDeviceInfo& tList, String strDvInstanceID)
{
	String strDvPdName = "";
	return strDvPdName;
}

INT32					CManagePoDvDefault::FindDvPolID(String strDvInstanceID, String strDvPaInstanceID, UINT32 nUsed)
{
/*
	TMapDBPoDvDefaultItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{	
		if(nUsed)
		{
			if(begin->second.tDPH.nUsedMode == 0)	continue;
		}
		String strDstDvInstanceID = begin->second.strDvInstanceID;
		String strDstDvPaInstanceID = begin->second.strDvPaInstanceID;
		strDstDvInstanceID.MakeUpper();	strDstDvPaInstanceID.MakeUpper();	strDvInstanceID.MakeUpper();	strDvPaInstanceID.MakeUpper();
		if(strDstDvInstanceID.Compare(strDvInstanceID) == 0 && 
			strDstDvPaInstanceID.Compare(strDvPaInstanceID) == 0)
			return begin->second.tDPH.nID;
	}
*/
	return 0;
}

//---------------------------------------------------------------------------
INT32					CManagePoDvDefault::FindDvPolID(String strDvPaInstanceID, UINT32 nUsed)
{
/*
	TMapDBPoDvDefaultItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{	
		if(nUsed)
		{
			if(begin->second.tDPH.nUsedMode == 0)	continue;
		}
		CString strDstDvInstanceID = begin->second.strDvInstanceID;
		strDstDvInstanceID.MakeUpper();	strDvPaInstanceID.MakeUpper();
		if(strDstDvInstanceID.Compare(strDvPaInstanceID) == 0)
			return begin->second.tDPH.nID;
	}
*/
	return 0;
}
//---------------------------------------------------------------------------
INT32					CManagePoDvDefault::DeleteAllDvDefault()
{
	TMapDBPoDvDefaultItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		begin->second.tDPH.nUsedMode = 0;
		EditPoDvDefault(begin->second);
	}
	return 0;
}
//---------------------------------------------------------------------------
UINT32					CManagePoDvDefault::GetDvDefaultCnt()
{
	UINT32 nCnt = 0;
	TMapDBPoDvDefaultItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		if(begin->second.tDPH.nUsedMode == 0)	continue;
		nCnt++;
	}
	return nCnt;
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::SetPkt(MemToken& SendToken, BOOL nIsUse)
{
	if(nIsUse)
		SendToken.TokenAdd_32(GetDvDefaultCnt());
	else
		SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoDvDefaultItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
		if(nIsUse && begin->second.tDPH.nUsedMode == 0)	continue;
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_DV_DEFAULT pdeni = FindItem(nID);
	if(!pdeni)	return -1;

	return SetPkt(pdeni, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::SetPkt(PDB_PO_DV_DEFAULT pdidi, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdidi->tDPH);

	SendToken.TokenAdd_32(pdidi->nPoPerm);
	SendToken.TokenAdd_String(pdidi->strDvPaName);
	SendToken.TokenAdd_String(pdidi->strDvName);
	SendToken.TokenAdd_String(pdidi->strDvInstanceID);
	SendToken.TokenAdd_String(pdidi->strDvPaInstanceID);
	SendToken.TokenAdd_String(pdidi->strClassName);

	SendToken.TokenSet_Block();

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoDvDefault::GetPkt(MemToken& RecvToken, DB_PO_DV_DEFAULT& didi)
{
	if (!RecvToken.TokenDel_DPH(didi.tDPH))						goto	INVALID_PKT;

	if (!RecvToken.TokenDel_32(didi.nPoPerm))					goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvPaName) < 0)		goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvName) < 0)			goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvInstanceID) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strDvPaInstanceID) < 0)	goto	INVALID_PKT;
	if ( RecvToken.TokenDel_String(didi.strClassName) < 0)		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



