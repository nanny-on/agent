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
#include "ManageHostVulnRst.h"

//---------------------------------------------------------------------------

CManageHostVulnRst*	t_ManageHostVulnRst = NULL;

//---------------------------------------------------------------------------

CManageHostVulnRst::CManageHostVulnRst()
{
	t_DBMgrHostVulnRst	= new CDBMgrHostVulnRst();
}
//---------------------------------------------------------------------------

CManageHostVulnRst::~CManageHostVulnRst()
{
	if(t_DBMgrHostVulnRst)		{	delete t_DBMgrHostVulnRst;		t_DBMgrHostVulnRst = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::LoadDBMS()
{
	TListDBHostVulnRst tDBHostVulnRstList;
    TListDBHostVulnRstItor begin, end;
	if(SetER(t_DBMgrHostVulnRst->LoadExecute(&tDBHostVulnRstList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBHostVulnRstList.begin();	end = tDBHostVulnRstList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddKeyID(begin->nScanMon, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRst::AddHostVulnRst(DB_HOST_VULN_RST&	data)
{
	if(SetER(t_DBMgrHostVulnRst->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);    
	AddKeyID(data.nScanMon, data.nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRst::EditHostVulnRst(DB_HOST_VULN_RST&	data)
{
    if(t_DBMgrHostVulnRst->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManageHostVulnRst::DelHostVulnRst(UINT32 nID)
{
	PDB_HOST_VULN_RST pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(t_DBMgrHostVulnRst->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }

	DelKeyID(pdata->nScanMon);
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

void			CManageHostVulnRst::SetRstMapToStr(PDB_HOST_VULN_RST pdata)
{
	UINT32 nTotal = 0, nScore = 0;

	String strRst;
	TMapDBHostVulnRstUnitSimpleItor begin, end;
	begin = pdata->tRstUnitMap.begin();	end = pdata->tRstUnitMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_VULN_RST_UNIT_SIMPLE pdata = &(begin->second);
		P32ToHex((PINT32)pdata, 3, strRst);

		nTotal += pdata->nDScore;
		if(pdata->nRst == SS_PTN_VULN_SCAN_RESULT_SAFE)
			nScore += pdata->nDScore;
	}

	pdata->strScanRst = strRst;
	pdata->nScanScore = (nTotal ? ((nScore * 100)) / nTotal : 0);
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManageHostVulnRst::SetPktSync(MemToken& SendToken)
{
	TListPVOID tSendList;

	{
		TMapDBHostVulnRstItor begin, end;
		begin = m_tMap.begin();	end = m_tMap.end();
		for(begin; begin != end; begin++)
		{
			if(	ISSYNCSTEP(begin->second.nSyncSvrStep))	continue;

			tSendList.push_back(&(begin->second));
		}
		if(tSendList.empty())	return -1;
	}

	SendToken.TokenAdd_32(tSendList.size());
	{
		TListPVOIDItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			SetPkt((PDB_HOST_VULN_RST)(*begin), SendToken);
		}
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32	CManageHostVulnRst::GetPkt(MemToken& RecvToken, DB_HOST_VULN_RST& data)
{
	if (!RecvToken.TokenDel_32(data.nID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanMon))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nScanDate))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nGroupID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strIPAddr) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strUserName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSysName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strVersion) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanScore))			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strScanRst) < 0)	goto INVALID_PKT;
	
	RecvToken.TokenSkip_Block(); 
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt(MemToken& SendToken)
{
	TListID tIDList;
	GetItemIDList(tIDList);
	
	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_VULN_RST pdata = FindItem(*begin);
		if(!pdata)  	continue;

		SetPkt(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt(TListDBHostVulnRst& tDBHostVulnRstList, MemToken& SendToken)
{
	SendToken.TokenAdd_32(tDBHostVulnRstList.size());
	TListDBHostVulnRstItor begin, end;
	begin = tDBHostVulnRstList.begin();	end = tDBHostVulnRstList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_VULN_RST pdata = &(*begin);
		if(!pdata)  	continue;

		SetPkt(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt(PDB_HOST_VULN_RST pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_32(pdata->nScanMon);
	SendToken.TokenAdd_32(pdata->nScanDate);

	SendToken.TokenAdd_32(pdata->nGroupID);
	SendToken.TokenAdd_32(pdata->nHostID);

	SendToken.TokenAdd_String(pdata->strIPAddr);
	SendToken.TokenAdd_String(pdata->strUserName);
	SendToken.TokenAdd_String(pdata->strSysName);
	SendToken.TokenAdd_String(pdata->strVersion);

	SendToken.TokenAdd_32(pdata->nScanScore);
	SendToken.TokenAdd_String(pdata->strScanRst);

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

INT32	CManageHostVulnRst::GetPkt_Link(MemToken& RecvToken, DB_HOST_VULN_RST& data)
{
	if (!RecvToken.TokenDel_32(data.nID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedFlag))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSyncSvrStep))			goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanMon))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nScanDate))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nGroupID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nHostID))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strIPAddr) < 0)		goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strUserName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strSysName) < 0)	goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strVersion) < 0)	goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanScore))			goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strScanRst) < 0)	goto INVALID_PKT;

	RecvToken.TokenSkip_Block(); 
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt_Link(MemToken& SendToken)
{
	TListID tIDList;
	GetItemIDList(tIDList);

	SendToken.TokenAdd_32(tIDList.size());
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		PDB_HOST_VULN_RST pdata = FindItem(*begin);
		if(!pdata)  	continue;

		SetPkt_Link(pdata, SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt_Link(UINT32 nID, MemToken& SendToken)
{
	PDB_HOST_VULN_RST pdata = FindItem(nID);
	if(!pdata)	return -1;

	SetPkt_Link(pdata, SendToken);
	{
		TMapID tIDMap;
		t_ManageHostVulnRstUnit->GetKeyTypeID(nID, tIDMap);
		SendToken.TokenAdd_32(tIDMap.size());

		TMapIDItor begin, end;
		begin = tIDMap.begin();	end = tIDMap.end();
		for(begin; begin != end; begin++)
		{
			PDB_HOST_VULN_RST_UNIT pdata_unit = t_ManageHostVulnRstUnit->FindItem(begin->second);
			if(!pdata_unit)		continue;

			t_ManageHostVulnRstUnit->SetPkt_Link(pdata_unit, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CManageHostVulnRst::SetPkt_Link(PDB_HOST_VULN_RST pdata, MemToken& SendToken)	
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedFlag);
	SendToken.TokenAdd_32(pdata->nRegDate);
	SendToken.TokenAdd_32(pdata->nSyncSvrStep);

	SendToken.TokenAdd_32(pdata->nScanMon);
	SendToken.TokenAdd_32(pdata->nScanDate);

	SendToken.TokenAdd_32(pdata->nGroupID);
	SendToken.TokenAdd_32(pdata->nHostID);

	SendToken.TokenAdd_String(pdata->strIPAddr);
	SendToken.TokenAdd_String(pdata->strUserName);
	SendToken.TokenAdd_String(pdata->strSysName);
	SendToken.TokenAdd_String(pdata->strVersion);

	SendToken.TokenAdd_32(pdata->nScanScore);
	SendToken.TokenAdd_String(pdata->strScanRst);

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
void 		CManageHostVulnRst::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][host_vuln_rst] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





