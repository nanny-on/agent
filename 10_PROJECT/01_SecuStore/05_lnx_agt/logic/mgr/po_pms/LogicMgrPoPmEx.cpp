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
#include "LogicMgrPoPmEx.h"

//---------------------------------------------------------------------------

CLogicMgrPoPmEx*		t_LogicMgrPoPmEx = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoPmEx::CLogicMgrPoPmEx()
{
	t_ManagePoPmEx		= new CManagePoPmEx();
	t_ManagePoPmExPkg	= new CManagePoPmExPkg();
	t_ManagePoPmExUnit	= new CManagePoPmExUnit();

	t_ManagePoPmEx->LoadDBMS();
	t_ManagePoPmExPkg->LoadDBMS();
	t_ManagePoPmExUnit->LoadDBMS();

	t_ManagePoPmEx->InitHash();

	m_strLogicName		= "mgr agt po pm ex";
	
	m_nPolicyType		= SS_POLICY_TYPE_PM_EX;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_PM_EX;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_PM_EX;
}
//---------------------------------------------------------------------------

CLogicMgrPoPmEx::~CLogicMgrPoPmEx()
{
	SAFE_DELETE(t_ManagePoPmEx);
	SAFE_DELETE(t_ManagePoPmExPkg);
	SAFE_DELETE(t_ManagePoPmExUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoPmEx::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmEx::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_PM_EX pdata = NULL;
	DB_PO_PM_EX data;

	m_tDPH = &(data.tDPH);

	TListDBPoPmExPkg	tPkgList;
	TListDBPoPmExUnit	tUnitList;

	if( t_ManagePoPmEx->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_PM_EX_PKG		data_pkg;		
		DB_PO_PM_EX_UNIT	data_unit;

		if( t_ManagePoPmExPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoPmExUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmEx::ApplyPolicy()
{
	return 0;
}
//---------------------------------------------------------------------------


INT32		CLogicMgrPoPmEx::OnTimer_Logic()
{
	if(t_ManageEnvLicense->IsInvalidRight(m_nPackageType, m_nPolicyType, m_nControlType))
	{
		return 0;
	}
	
	ApplyPolicy();
	return 1;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmEx::ApplyPolicy_Unit(UINT32 nUnitID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmEx::ApplyPolicy_Unit_HostPatch(TListID& tHPIDList)
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoPmEx::ApplyExecuteEnd(UINT32 nWorkID)
{
	return 0;
}
//---------------------------------------------------------------------------