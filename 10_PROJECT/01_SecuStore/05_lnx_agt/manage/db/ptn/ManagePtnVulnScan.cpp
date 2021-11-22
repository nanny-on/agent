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
#include "ManagePtnVulnScan.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

CManagePtnVulnScan::CManagePtnVulnScan()
{
	m_tDBMgrPtnVulnScan	= new CDBMgrPtnVulnScan();
}
//---------------------------------------------------------------------------

CManagePtnVulnScan::~CManagePtnVulnScan()
{
	if(m_tDBMgrPtnVulnScan)		{	delete m_tDBMgrPtnVulnScan;		m_tDBMgrPtnVulnScan = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePtnVulnScan::LoadDBMS()
{
	TListDBPtnVulnScan tDBPtnVulnScanList;
    TListDBPtnVulnScanItor begin, end;
	if(SetER(m_tDBMgrPtnVulnScan->LoadExecute(&tDBPtnVulnScanList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPtnVulnScanList.begin();	end = tDBPtnVulnScanList.end();
    for(begin; begin != end; begin++)
    {
		AddItem(begin->nID, *begin);
		AddKeyIDList(begin->nVulnID, begin->nID);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnScan::AddPtnVulnScan(DB_PTN_VULN_SCAN&	data)
{
	if(SetER(m_tDBMgrPtnVulnScan->InsertExecute(&data)))
    {
    	return g_nErrRtn;
    }

	AddItem(data.nID, data);   
	AddKeyIDList(data.nVulnID, data.nID);    
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnScan::EditPtnVulnScan(DB_PTN_VULN_SCAN&	data)
{
    if(m_tDBMgrPtnVulnScan->UpdateExecute(&data))		return ERR_DBMS_UPDATE_FAIL;

	EditItem(data.nID, data);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePtnVulnScan::DelPtnVulnScan(UINT32 nID)
{
	PDB_PTN_VULN_SCAN pdata = FindItem(nID);
	if(!pdata)	return ERR_INFO_NOT_DELETE_BECAUSE_NOT_FIND;

	if(SetER(m_tDBMgrPtnVulnScan->DeleteExecute(pdata->nID)))
    {
    	return g_nErrRtn;
    }
    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32	CManagePtnVulnScan::GetPkt(MemToken& RecvToken, DB_PTN_VULN_SCAN& data)
{	
	if (!RecvToken.TokenDel_32(data.nID))						goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nUsedMode))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nRegDate))					goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nVulnID))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nVulnSubID))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nScanOrder))				goto INVALID_PKT;

	if ( RecvToken.TokenDel_String(data.strName) < 0)			goto INVALID_PKT;

	if (!RecvToken.TokenDel_64(data.nSysType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysSPType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nSysArchType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nScanType))					goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nCustomID))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strScanPath) < 0)		goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nValueType))				goto INVALID_PKT;

	if (!RecvToken.TokenDel_32(data.nDecisionType))				goto INVALID_PKT;
	if (!RecvToken.TokenDel_32(data.nCompType))					goto INVALID_PKT;
	if ( RecvToken.TokenDel_String(data.strCompValue) < 0)		goto INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//-------------------------------------------------------------------------------------

INT32		CManagePtnVulnScan::SetPkt(MemToken& SendToken)
{	
	SendToken.TokenAdd_32(m_tMap.size());
	TMapDBPtnVulnScanItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PTN_VULN_SCAN pdata = &(begin->second);
		if(!pdata)	continue;

		SetPkt(pdata, SendToken);
	}	
	return m_tMap.size();
}
//---------------------------------------------------------------------------

INT32		CManagePtnVulnScan::SetPkt(PDB_PTN_VULN_SCAN pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pdata->nID);
	SendToken.TokenAdd_32(pdata->nUsedMode);
	SendToken.TokenAdd_32(pdata->nRegDate);

	SendToken.TokenAdd_32(pdata->nVulnID);
	SendToken.TokenAdd_32(pdata->nVulnSubID);
	SendToken.TokenAdd_32(pdata->nScanOrder);

	SendToken.TokenAdd_String(pdata->strName);

	SendToken.TokenAdd_64(pdata->nSysType);
	SendToken.TokenAdd_32(pdata->nSysSPType);
	SendToken.TokenAdd_32(pdata->nSysArchType);

	SendToken.TokenAdd_32(pdata->nScanType);
	SendToken.TokenAdd_32(pdata->nCustomID);
	SendToken.TokenAdd_String(pdata->strScanPath);
	SendToken.TokenAdd_32(pdata->nValueType);

	SendToken.TokenAdd_32(pdata->nDecisionType);
	SendToken.TokenAdd_32(pdata->nCompType);
	SendToken.TokenAdd_String(pdata->strCompValue);

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

void 		CManagePtnVulnScan::RemainDBG_ItemCnt(UINT32 nIdx)
{
	WriteLogN("remain manage item cnt [%d][ptn_vuln_scan] : %u, %u", nIdx, m_tMap.size(), m_tKeyMap.size());
    return;
}
//---------------------------------------------------------------------------





