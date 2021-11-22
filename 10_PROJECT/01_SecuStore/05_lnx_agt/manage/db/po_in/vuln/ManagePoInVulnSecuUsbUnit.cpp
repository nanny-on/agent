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
#include "ManagePoInVulnSecuUsbUnit.h"

//---------------------------------------------------------------------------

CManagePoInVulnSecuUsbUnit*	t_ManagePoInVulnSecuUsbUnit = NULL;

//---------------------------------------------------------------------------

CManagePoInVulnSecuUsbUnit::CManagePoInVulnSecuUsbUnit()
{
	t_DBMgrPoInVulnSecuUsbUnit	= new CDBMgrPoInVulnSecuUsbUnit();
}
//---------------------------------------------------------------------------

CManagePoInVulnSecuUsbUnit::~CManagePoInVulnSecuUsbUnit()
{
	if(t_DBMgrPoInVulnSecuUsbUnit)	{	delete t_DBMgrPoInVulnSecuUsbUnit;	t_DBMgrPoInVulnSecuUsbUnit = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoInVulnSecuUsbUnit::LoadDBMS()
{
	TListDBPoInVulnSecuUsbUnit tDBPoInVulnSecuUsbUnitList;
	TListDBPoInVulnSecuUsbUnitItor begin, end;
	if(SetER(t_DBMgrPoInVulnSecuUsbUnit->LoadExecute(&tDBPoInVulnSecuUsbUnitList)))
	{
		return g_nErrRtn;
	}

	begin = tDBPoInVulnSecuUsbUnitList.begin();	end = tDBPoInVulnSecuUsbUnitList.end();
	for(begin; begin != end; begin++)
	{
		AddItem(begin->tDPH.nID, *begin);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::GetHash(UINT32 nID, String& strOrgValue)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pdata 			= NULL;
	{
		if( (pdata = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_in_vuln_secu_usb_unit by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,"
							"%s,%s,%s,%s,", 
							GetHdrHashInfo(pdata).c_str(),
							pdata->strUsbValue.c_str(), pdata->strRegValue.c_str(), pdata->strProcess.c_str(), pdata->strFilePath.c_str());
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::AddPoInVulnSecuUsbUnit(DB_PO_IN_VULN_SECU_USB_UNIT&	data)
{
	if(SetER(t_DBMgrPoInVulnSecuUsbUnit->InsertExecute(&data)))
	{
		return g_nErrRtn;
	}

	AddItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::EditPoInVulnSecuUsbUnit(DB_PO_IN_VULN_SECU_USB_UNIT&	data)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pdata = FindItem(data.tDPH.nID);
	if(!pdata)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoInVulnSecuUsbUnit->UpdateExecute(&data)))
	{
		return g_nErrRtn;
	}

	EditItem(data.tDPH.nID, data);

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::DelPoInVulnSecuUsbUnit(UINT32 nID)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pdata = FindItem(nID);
	if(!pdata)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoInVulnSecuUsbUnit->DeleteExecute(pdata->tDPH.nID)))
	{
		return g_nErrRtn;
	}

	DeleteItem(nID);
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::ApplyPoInVulnSecuUsbUnit(DB_PO_IN_VULN_SECU_USB_UNIT&	data)
{
	if(FindItem(data.tDPH.nID))
	{
		return EditPoInVulnSecuUsbUnit(data);
	}
	return AddPoInVulnSecuUsbUnit(data);
}
//---------------------------------------------------------------------------

String					CManagePoInVulnSecuUsbUnit::GetName(UINT32 nID)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pdata = FindItem(nID);
	if(!pdata)	return "";
	return pdata->tDPH.strName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::SetPkt(MemToken& SendToken)
{
	SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoInVulnSecuUsbUnitItor begin, end;
	begin = m_tMap.begin();	end = m_tMap.end();
	for(begin; begin != end; begin++)
	{
		SetPkt(&(begin->second), SendToken);
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_IN_VULN_SECU_USB_UNIT pdata = FindItem(nID);
	if(!pdata)	return -1;

	return SetPkt(pdata, SendToken);
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::SetPkt(PDB_PO_IN_VULN_SECU_USB_UNIT pdata, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdata->tDPH);

	SendToken.TokenAdd_String(pdata->strUsbValue);
	SendToken.TokenAdd_String(pdata->strRegValue);
	SendToken.TokenAdd_String(pdata->strProcess);
	SendToken.TokenAdd_String(pdata->strFilePath);

	SendToken.TokenSet_Block();
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoInVulnSecuUsbUnit::GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_SECU_USB_UNIT& data)
{
	if (!RecvToken.TokenDel_DPH(data.tDPH))							goto	INVALID_PKT;

	if (RecvToken.TokenDel_String(data.strUsbValue)<0)				goto	INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strRegValue)<0)				goto	INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strProcess)<0)				goto	INVALID_PKT;
	if (RecvToken.TokenDel_String(data.strFilePath)<0)				goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



