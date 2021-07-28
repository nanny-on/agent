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
#include "LogicMgrPoInPtnExTrust.h"

//---------------------------------------------------------------------------

CLogicMgrPoInPtnExTrust*		t_LogicMgrPoInPtnExTrust = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInPtnExTrust::CLogicMgrPoInPtnExTrust()
{
	t_ManagePoInPtnExTrust		= new CManagePoInPtnExTrust();
	t_LogicPoInPtnExTrust		= new CLogicPoInPtnExTrust();
	
	t_ManagePoInPtnExTrust->LoadDBMS();
	
	m_strLogicName		= "mgr agt po in ptn ex trust";
	
	m_nPolicyUnitType	= SS_POLICY_UNIT_TYPE_IN_PTN_EX_TRUST;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY_UNIT;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_UNIT_IN_PTN_EX_TRUST;
}
//---------------------------------------------------------------------------

CLogicMgrPoInPtnExTrust::~CLogicMgrPoInPtnExTrust()
{
	SAFE_DELETE(t_LogicPoInPtnExTrust);
	SAFE_DELETE(t_ManagePoInPtnExTrust);
}
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInPtnExTrust::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnExTrust::AnalyzePkt_FromMgr_Add_Ext()
{
	DB_PO_IN_PTN_EX_TRUST data;

	if(t_ManagePoInPtnExTrust->GetPkt(RecvToken, data))		return AZPKT_CB_RTN_PKT_INVALID;

	data.tDPH.nRegDate = GetCurrentDateTimeInt();
	SetPoInPtnExTrsut(data);
	
	return AZPKT_CB_RTN_SUCCESS_END;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnExTrust::CheckPtnLifeCycle()
{
	TListID tDelIDList;	
	{
		TListID tIDList;
		t_ManagePoInPtnExTrust->GetItemIDList(tIDList);

		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_PTN_EX_TRUST pdata = t_ManagePoInPtnExTrust->FindItem(*begin);
			if(!pdata)	continue;

			UINT32 nFindPtn = 0;
			if(t_MMPPGBO && t_MMPPGBO->t_ManagePtnProcFile->FindSKeyID(pdata->strFileKey.c_str()))
				nFindPtn += PTN_FB_PTN_RISK_BLACK;
			if(t_MMPPGWO && t_MMPPGWO->t_ManagePtnProcFile->FindSKeyID(pdata->strFileKey.c_str()))
				nFindPtn += PTN_FB_PTN_RISK_WHITE;

			if(nFindPtn)
			{
				WriteLogN("[%s] find file key in admin make ptn... remote key from auto ptn data : [%s]:[%d]->[%d]", m_strLogicName.c_str(), pdata->strOFName.c_str(), pdata->nBlockMode, nFindPtn);
				tDelIDList.push_back(*begin);
			}
		}
	}

	if(tDelIDList.size())
	{
		TListIDItor begin, end;
		begin = tDelIDList.begin();	end = tDelIDList.end();
		for(begin; begin != end; begin++)
		{
			t_ManagePoInPtnExTrust->DelPoInPtnExTrust(*begin);
		}
		WriteLogN("[%s] remote key from auto ptn data success : [%d]", m_strLogicName.c_str(), tDelIDList.size());
	}

	{	
		SendToken.Clear();
		SendToken.TokenAdd_IDList(tDelIDList);
		{
			SendData_Link(G_TYPE_PO_IN_PTN_EX_TRUST, G_CODE_COMMON_DEL, SendToken);
		}
		SendToken.Clear();
	}
	return 0;

}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnExTrust::LoadWEngPtn(CASIWEngDLLUtil* pWEDLLUtil, String strFileName, String strFileHash)
{
	CFileUtil tFileUtil;
	String strFullPath;

	strFullPath = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, strFileName.c_str());
	strFullPath = tFileUtil.GetEnvPathToLocalPathSingle(strFullPath);
	if(tFileUtil.FileExists(strFullPath.c_str()) == FALSE)
	{
		WriteLogE("[%s] not find op ptn file [%s]", m_strLogicName.c_str(), strFullPath.c_str());
		return 0;
	}

	CHAR szFileHash[CHAR_MAX_SIZE] = {0, };
	SHAFile(ASIHASH_SHA_LEN_TYPE_256, strFullPath.c_str(), szFileHash, CHAR_MAX_SIZE);
	if(strFileHash.compare(szFileHash))
	{
		WriteLogE("[%s] invalid ptn op pattern file hash : [%s][%s]", m_strLogicName.c_str(), strFullPath.c_str(), szFileHash);
		return -100;
	}

	INT32 nPtnCnt = 0;
	if(SetER(pWEDLLUtil->LoadWL(strFullPath.c_str(), NULL, 0, &nPtnCnt)))
	{
		WriteLogE("[%s] load ptn op fail : [%s]", m_strLogicName.c_str(), pWEDLLUtil->GetLastError());
		return -200;
	}	
	return 0;
}
//--------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void		CLogicMgrPoInPtnExTrust::SetPoInPtnExTrsut(DB_PO_IN_PTN_EX_TRUST& data)
{
	{
		if(data.strOFName.find("%") == string::npos && data.strSFName.find("%") == string::npos)
		{
			WriteLogN("[%s] remain ptn ex trust log to file : of[%s]:sf[%s]:bm:[%d]", 
				m_strLogicName.c_str(), data.strOFName.c_str(), data.strSFName.c_str(), data.nBlockMode);
		}
	}
	
	{
		if(t_ManagePoInPtnExTrust->FindSKeyID(data.strFileKey) == 0)
			t_ManagePoInPtnExTrust->AddPoInPtnExTrust(data);
	}

	{	
		SendToken.Clear();
		SendToken.TokenAdd_32(1);
		t_ManagePoInPtnExTrust->SetPkt(&data, SendToken);
		{
			SendData_Link(G_TYPE_PO_IN_PTN_EX_TRUST, G_CODE_COMMON_ADD, SendToken);
		}
		SendToken.Clear();
	}
	return;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
