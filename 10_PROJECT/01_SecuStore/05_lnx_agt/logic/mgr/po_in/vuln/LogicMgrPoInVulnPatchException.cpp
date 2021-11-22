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
#include "LogicMgrPoInVulnPatchException.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnPatchException*		t_LogicMgrPoInVulnPatchException = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnPatchException::CLogicMgrPoInVulnPatchException()
{
	t_ManagePoInVulnPatchException		= new CManagePoInVulnPatchException();
	t_ManagePoInVulnPatchExceptionPkg	= new CManagePoInVulnPatchExceptionPkg();
	t_ManagePoInVulnPatchExceptionUnit	= new CManagePoInVulnPatchExceptionUnit();

	t_ManagePoInVulnPatchException->LoadDBMS();
	t_ManagePoInVulnPatchExceptionPkg->LoadDBMS();
	t_ManagePoInVulnPatchExceptionUnit->LoadDBMS();

	t_ManagePoInVulnPatchException->InitHash();

	m_strLogicName		= "mgr agt po in vuln patch exception";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_PATCH_EXCEPTION;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_PATCH_EXCEPTION;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnPatchException::~CLogicMgrPoInVulnPatchException()
{
	SAFE_DELETE(t_ManagePoInVulnPatchException);
	SAFE_DELETE(t_ManagePoInVulnPatchExceptionPkg);
	SAFE_DELETE(t_ManagePoInVulnPatchExceptionUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnPatchException::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnPatchException::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_PATCH_EXCEPTION pdata = NULL;
	DB_PO_IN_VULN_PATCH_EXCEPTION data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnPatchExceptionPkg	tPkgList;
	TListDBPoInVulnPatchExceptionUnit	tUnitList;

	if( t_ManagePoInVulnPatchException->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_PATCH_EXCEPTION_PKG		data_pkg;		
		DB_PO_IN_VULN_PATCH_EXCEPTION_UNIT	data_unit;

		if( t_ManagePoInVulnPatchExceptionPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInVulnPatchExceptionUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_PATCH_EXCEPTION)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnPatchExceptionPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnPatchException->DelPoInVulnPatchException(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnPatchExceptionUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnPatchExceptionUnit->ApplyPoInVulnPatchExceptionUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnPatchExceptionPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnPatchExceptionPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnPatchExceptionPkg->AddPoInVulnPatchExceptionPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnPatchException->ApplyPoInVulnPatchException(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
