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
#include "LogicMgrHostSw.h"

//---------------------------------------------------------------------------

CLogicMgrHostSw*		t_LogicMgrHostSw = NULL;

//---------------------------------------------------------------------------

CLogicMgrHostSw::CLogicMgrHostSw()
{
	t_ManageHostSw		= new CManageHostSw();
	if(t_ManageHostSw)
		t_ManageHostSw->LoadDBMS();

	m_strLogicName = "logic mgr host sw";

	m_nControlType	= SS_CONTROL_TYPE_ORG_HOST;
	m_nEvtObjType	= EVENT_OBJECT_TYPE_HOST;
	m_nEvtObjCode	= 0;
}
//---------------------------------------------------------------------------

CLogicMgrHostSw::~CLogicMgrHostSw()
{
	SAFE_DELETE(t_ManageHostSw);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrHostSw::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
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

INT32		CLogicMgrHostSw::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_HOST_SW pdhs = NULL;
	TListDBHostSw tDBHostSwList;
	DB_HOST_SW dhs;

	if(!RecvToken.TokenDel_32(m_nRecvNum))
		return AZPKT_CB_RTN_PKT_INVALID;
	while(m_nRecvNum--)
	{
		if( t_ManageHostSw->GetPkt(RecvToken, dhs))
			return AZPKT_CB_RTN_PKT_INVALID;

		tDBHostSwList.push_back(dhs);
	}

	
	{
		TListDBHostSwItor begin, end;
		begin = tDBHostSwList.begin();	end = tDBHostSwList.end();
		for(begin; begin != end; begin++)
		{
			if( (pdhs = t_ManageHostSw->FindChkKeyNameItem(begin->strName, begin->strVersion)) == NULL)
			{
				t_ManageHostSw->AddHostSw(*begin);
			}
			else
			{
				if(begin->nUsedFlag)
					t_ManageHostSw->EditHostSw(*begin);
				else
					t_ManageHostSw->DelHostSw(pdhs->nID);
			}
		}
	}

	{
		SendToken.TokenAdd_32(ERR_SUCCESS);	
		t_ManageHostSw->SetPkt(SendToken);

		SendData_Link(G_TYPE_HOST_SW, G_CODE_COMMON_EDIT, SendToken);
		SendToken.Clear();
	}

	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrHostSw::SendPkt_HostSw()
{
	INT32 nSendPkt = 1;
	INT32 nListCnt = 0;
	INT32 nRetVal = 0;
	TListSwInfo tSwInfoList;
	TListDBHostSw tDBHostSwList; 
	if(t_ASISIDLLUtil)
	{
		CSystemInfo tSysInfo;
		INT32 nProcArchitect = (INT32)tSysInfo.GetASIProcArchitecture();
		nRetVal = t_ASISIDLLUtil->GetSwInfo(nProcArchitect, GetSoftwareInfo, (PVOID)&tSwInfoList);
		if(nRetVal != 0)
		{
			WriteLogE("[SendPkt_HostSw] fail to get sw list (%d)", nRetVal);
		}
	}
	{				
		TListSwInfoItor begin, end;
		begin = tSwInfoList.begin();	end = tSwInfoList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->szName[0] == 0)
				continue;

			PDB_HOST_SW pdhs = t_ManageHostSw->FindChkKeyNameItem(begin->szName, begin->szVersion);
			if(pdhs)
			{
				continue;
			}
			else
			{
				DB_HOST_SW tDHS;
				tDHS.nProcArch		= begin->nProcArchitect;
				tDHS.strChkKey		= begin->szChkKey;
				tDHS.strName		= begin->szName;
				tDHS.strPublisher	= begin->szPublisher;
				tDHS.strVersion		= begin->szVersion;
				tDHS.strInsDate		= begin->szInsDate;

				tDBHostSwList.push_back(tDHS);
			}
		}
	}

	{
		TListID tIDList;
		t_ManageHostSw->GetItemIDList(tIDList);

		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_HOST_SW pdhs = t_ManageHostSw->FindItem(*begin);
			if(!pdhs)
				continue;

			if(t_ManageHostSw->IsExistLocalSwKeyVer(&tSwInfoList, pdhs->strName, pdhs->strVersion))
				continue;

			pdhs->nUsedFlag = 0;
			tDBHostSwList.push_back(*pdhs);
		}
	}

	nListCnt = tDBHostSwList.size();
	if(nListCnt < 1)
	{
		WriteLogN("[%s] changed software item does not exist...", m_strLogicName.c_str());
		return;
	}

	SendToken.Clear();
	t_ManageHostSw->SetPkt(tDBHostSwList, SendToken);
	SendData_Mgr(G_TYPE_HOST_SW, G_CODE_COMMON_EDIT, SendToken);
	SendToken.Clear();

	return;
}
//---------------------------------------------------------------------------
VOID		CLogicMgrHostSw::GetSoftwareInfo(PASISI_SW_INFO pasi, INT32 nSize, PVOID lParam)
{
	ASISI_SW_INFO tASI;
	if(pasi == NULL)
		return;
	memcpy(&tASI, pasi, sizeof(ASISI_SW_INFO));

	if(!lParam)
		return;

	TListSwInfo* tSwInfoList = (TListSwInfo*)lParam;
	tSwInfoList->push_back(tASI);
	return;
}
//---------------------------------------------------------------------------

void		CLogicMgrHostSw::SendPkt_HostSw_Cur()
{
	TListSwInfo tSwInfoList;
	TListDBHostSw tDBHostSwList;
	if(t_ASISIDLLUtil)
	{
		CSystemInfo tSysInfo;
		INT32 nRetVal=0;
		INT32 nProcArchitect = (INT32)tSysInfo.GetASIProcArchitecture();
		nRetVal = t_ASISIDLLUtil->GetSwInfo(nProcArchitect, GetSoftwareInfo, (PVOID)&tSwInfoList);
		if(nRetVal != 0)
		{
			WriteLogE("[SendPkt_HostSw_Cur] fail to get sw list (%d)", nRetVal);
		}
	}

	{				
		TListSwInfoItor begin, end;
		begin = tSwInfoList.begin();	end = tSwInfoList.end();
		for(begin; begin != end; begin++)
		{
			if(begin->szName[0] == 0)
				continue;

			PDB_HOST_SW pdhs = t_ManageHostSw->FindChkKeyNameItem(begin->szName, begin->szVersion);
			if(pdhs)
			{
				continue;
			}
			else
			{
				DB_HOST_SW tDHS;
				tDHS.nProcArch		= begin->nProcArchitect;
				tDHS.strChkKey		= begin->szChkKey;
				tDHS.strName		= begin->szName;
				tDHS.strPublisher	= begin->szPublisher;
				tDHS.strVersion		= begin->szVersion;
				tDHS.strInsDate		= begin->szInsDate;

				tDBHostSwList.push_back(tDHS);
			}
		}
	}

	if(tDBHostSwList.size() == 0)
	{
		WriteLogN("[%s] host software item does not exist...", m_strLogicName.c_str());
		return;
	}

	SendToken.Clear();
	t_ManageHostSw->SetPkt(tDBHostSwList, SendToken);
	SendData_Mgr(G_TYPE_HOST_SW, G_CODE_COMMON_SET, SendToken);
	SendToken.Clear();
}
