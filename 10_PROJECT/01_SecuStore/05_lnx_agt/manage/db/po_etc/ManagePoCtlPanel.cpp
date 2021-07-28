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
#include "ManagePoCtlPanel.h"

//---------------------------------------------------------------------------

CManagePoCtlPanel*	t_ManagePoCtlPanel = NULL;

//---------------------------------------------------------------------------

CManagePoCtlPanel::CManagePoCtlPanel()
{
	t_DBMgrPoCtlPanel	= new CDBMgrPoCtlPanel();
}
//---------------------------------------------------------------------------

CManagePoCtlPanel::~CManagePoCtlPanel()
{
	if(t_DBMgrPoCtlPanel)	{	delete t_DBMgrPoCtlPanel;	t_DBMgrPoCtlPanel = NULL;	}
}
//---------------------------------------------------------------------------

INT32		CManagePoCtlPanel::LoadDBMS()
{
	TListDBPoCtlPanel tDBPoCtlPanelList;
    TListDBPoCtlPanelItor begin, end;
	if(SetER(t_DBMgrPoCtlPanel->LoadExecute(&tDBPoCtlPanelList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBPoCtlPanelList.begin();	end = tDBPoCtlPanelList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->tDPH.nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::InitHash()
{
	t_ManagePoCtlPanelPkg->InitPkg();
	
	TListID tIDList;
	TListIDItor begin, end;

	GetItemIDList(tIDList);
	begin = tIDList.begin();		end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		InitHash(*begin);
	}	
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::InitHash(UINT32 nID)
{
	String strOrgValue;

	PDB_PO_CTL_PANEL pdpcp 			= NULL;
	{
		if( (pdpcp = FindItem(nID)) == NULL)
		{
			WriteLogE("not find po_ctl_panel by hash : [%d]", ERR_INFO_NOT_OP_BECAUSE_NOT_FIND);
			return ERR_INFO_NOT_OP_BECAUSE_NOT_FIND;
		}
	}

	{
		strOrgValue = SPrintf("%s,%u,", 
							GetHdrHashInfo(pdpcp).c_str(),
							pdpcp->nControlType);

		{
			TMapIDItor begin, end;
			begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
			for(begin; begin != end; begin++)
			{
				String strPkgValue;
				t_ManagePoCtlPanelPkg->GetHash(begin->first, strPkgValue);
				strOrgValue += strPkgValue;
			}
		}
	}

	{
		char pszHashValue[CHAR_MAX_SIZE] = {0, };
		SHAString(ASIHASH_SHA_LEN_TYPE_128, strOrgValue.c_str(), strOrgValue.length(), pszHashValue, CHAR_MAX_SIZE);
		pdpcp->tDPH.strHash = pszHashValue;
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::AddPoCtlPanel(DB_PO_CTL_PANEL&	dpcp)
{
	if(SetER(t_DBMgrPoCtlPanel->InsertExecute(&dpcp)))
    {
    	return g_nErrRtn;
    }

    AddItem(dpcp.tDPH.nID, dpcp);
	InitHash(dpcp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::EditPoCtlPanel(DB_PO_CTL_PANEL&	dpcp)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(dpcp.tDPH.nID);
	if(!pdpcp)	return ERR_DBMS_UPDATE_FAIL;

	if(SetER(t_DBMgrPoCtlPanel->UpdateExecute(&dpcp)))
    {
    	return g_nErrRtn;
    }

    EditItem(dpcp.tDPH.nID, dpcp);
	InitHash(dpcp.tDPH.nID);

    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::DelPoCtlPanel(UINT32 nID)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(nID);
	if(!pdpcp)	return ERR_DBMS_DELETE_FAIL;

	if(SetER(t_DBMgrPoCtlPanel->DeleteExecute(pdpcp->tDPH.nID)))
    {
    	return g_nErrRtn;
    }

    DeleteItem(nID);
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::ApplyPoCtlPanel(DB_PO_CTL_PANEL& dpcp)
{
	if(FindItem(dpcp.tDPH.nID))
	{
		return EditPoCtlPanel(dpcp);
	}
	return AddPoCtlPanel(dpcp);
}
//---------------------------------------------------------------------------

String					CManagePoCtlPanel::GetName(UINT32 nID)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(nID);
    if(!pdpcp)	return "";
    return pdpcp->tDPH.strName;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList)
{
	TMapIDItor begin, end;
	begin = tSubIDMap.begin();	end = tSubIDMap.end();
	for(begin; begin != end; begin++)
	{
		PDB_PO_CTL_PANEL_PKG pdpcpp = t_ManagePoCtlPanelPkg->FindItem(begin->first);
		if(!pdpcpp)	continue;

		PDB_PO_CTL_PANEL_UNIT pdpppu = t_ManagePoCtlPanelUnit->FindItem(pdpcpp->tDPH.nUnitID);
		if(!pdpppu)	continue;

		if(t_EnvInfo->m_nHostSysType & pdpppu->tDPH.nTargetOS)
			tPkgIDList.push_back(pdpcpp->tDPH.nID);
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::SetPkt(MemToken& SendToken)
{
    SendToken.TokenAdd_32(m_tMap.size());

	TMapDBPoCtlPanelItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	SetPkt(&(begin->second), SendToken);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::SetPkt(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);
	
	{
		SendToken.TokenAdd_32(pdpcp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlPanelPkg->SetPkt(begin->first, SendToken);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::SetPktHost(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);

	{
		SendToken.TokenAdd_32(pdpcp->tDPH.tSubIDMap.size());

		TMapIDItor begin, end;
		begin = pdpcp->tDPH.tSubIDMap.begin();	end = pdpcp->tDPH.tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlPanelPkg->SetPktHost(begin->first, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::SetPktHost_EPS(UINT32 nID, MemToken& SendToken)
{
	PDB_PO_CTL_PANEL pdpcp = FindItem(nID);
	if(!pdpcp)	return -1;

	SetPkt(pdpcp, SendToken);

	{
		TListID tPkgIDList;
		GetHostSysTypePkgListID(pdpcp->tDPH.tSubIDMap, tPkgIDList);	// OSType check

		SendToken.TokenAdd_32(tPkgIDList.size());		

		TListIDItor begin, end;
		begin = tPkgIDList.begin();	end = tPkgIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoCtlPanelPkg->SetPktHost(*begin, SendToken);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::SetPkt(PDB_PO_CTL_PANEL pdpcp, MemToken& SendToken)
{
	SendToken.TokenAdd_DPH(pdpcp->tDPH);

	SendToken.TokenAdd_32(pdpcp->nControlType);

	SendToken.TokenSet_Block();
    return 0;
}
//---------------------------------------------------------------------------

INT32					CManagePoCtlPanel::GetPkt(MemToken& RecvToken, DB_PO_CTL_PANEL& dpcp)
{
	if (!RecvToken.TokenDel_DPH(dpcp.tDPH))				goto	INVALID_PKT;
	if (!RecvToken.TokenDel_32(dpcp.nControlType))		goto	INVALID_PKT;

	RecvToken.TokenSkip_Block();
	return 0;
INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



