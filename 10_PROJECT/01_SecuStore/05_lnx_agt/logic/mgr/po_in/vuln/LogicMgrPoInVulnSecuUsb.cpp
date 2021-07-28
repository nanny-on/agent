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
#include "LogicMgrPoInVulnSecuUsb.h"

//---------------------------------------------------------------------------

CLogicMgrPoInVulnSecuUsb*		t_LogicMgrPoInVulnSecuUsb = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInVulnSecuUsb::CLogicMgrPoInVulnSecuUsb()
{
	t_ManagePoInVulnSecuUsb		= new CManagePoInVulnSecuUsb();
	t_ManagePoInVulnSecuUsbPkg		= new CManagePoInVulnSecuUsbPkg();
	t_ManagePoInVulnSecuUsbUnit	= new CManagePoInVulnSecuUsbUnit();

	t_ManagePoInVulnSecuUsb->LoadDBMS();
	t_ManagePoInVulnSecuUsbPkg->LoadDBMS();
	t_ManagePoInVulnSecuUsbUnit->LoadDBMS();

	t_ManagePoInVulnSecuUsb->InitHash();

	m_strLogicName		= "mgr agt po in vuln secu usb";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_VULN_SECU_USB;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_VULN_SECU_USB;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_VULN_SECU_USB;
}
//---------------------------------------------------------------------------

CLogicMgrPoInVulnSecuUsb::~CLogicMgrPoInVulnSecuUsb()
{
	SAFE_DELETE(t_ManagePoInVulnSecuUsb);
	SAFE_DELETE(t_ManagePoInVulnSecuUsbPkg);
	SAFE_DELETE(t_ManagePoInVulnSecuUsbUnit);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInVulnSecuUsb::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInVulnSecuUsb::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_VULN_SECU_USB pdata = NULL;
	DB_PO_IN_VULN_SECU_USB data;

	m_tDPH = &(data.tDPH);

	TListDBPoInVulnSecuUsbPkg	tPkgList;
	TListDBPoInVulnSecuUsbUnit	tUnitList;

	if( t_ManagePoInVulnSecuUsb->GetPkt(RecvToken, data))					return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))								return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_VULN_SECU_USB_PKG		data_pkg;		
		DB_PO_IN_VULN_SECU_USB_UNIT	data_unit;

		if( t_ManagePoInVulnSecuUsbPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInVulnSecuUsbUnit->GetPkt(RecvToken, data_unit))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_VULN_SECU_USB)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInVulnSecuUsbPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInVulnSecuUsb->DelPoInVulnSecuUsb(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInVulnSecuUsbUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnSecuUsbUnit->ApplyPoInVulnSecuUsbUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInVulnSecuUsbPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInVulnSecuUsbPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInVulnSecuUsbPkg->AddPoInVulnSecuUsbPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInVulnSecuUsb->ApplyPoInVulnSecuUsb(data)))
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
