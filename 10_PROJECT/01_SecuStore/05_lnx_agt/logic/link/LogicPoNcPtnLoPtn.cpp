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
#include "LogicPoNcPtnLoPtn.h"

//---------------------------------------------------------------------------

CLogicPoNcPtnLoPtn*		t_LogicPoNcPtnLoPtn = NULL;

//---------------------------------------------------------------------------

CLogicPoNcPtnLoPtn::CLogicPoNcPtnLoPtn()
{	
	m_strLogicName = "logic po nc ptn lo ptn";
}
//---------------------------------------------------------------------------

CLogicPoNcPtnLoPtn::~CLogicPoNcPtnLoPtn()
{
}
//---------------------------------------------------------------------------

INT32	CLogicPoNcPtnLoPtn::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{		
		case G_CODE_COMMON_SYNC:		AnalyzePkt_FromLink_Ext_Sync();		break;
		default:						break;		
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPoNcPtnLoPtn::AnalyzePkt_FromLink_Add_Ext()
{
	TListStr tPathList;

	if(RecvToken.TokenDel_StrList(tPathList) < 0)		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);		

	{
		if(t_LogicMgrPtnLWC->OpenDBFQ())	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		CHAR szLongPathName[CHAR_MAX_SIZE] = {0, };
		CHAR szShortPathName[CHAR_MAX_SIZE] = {0, };

		TListStrItor begin, end;
		begin = tPathList.begin();	end = tPathList.end();
		for(begin; begin != end; begin++)
		{
			String strFilePath = *begin;
						
			DWORD dwFileType = 0;
			if(dwFileType == AS_INVALID_FILE)
			{
				continue;
			}

			DB_PTN_PROC_FILE tDPPF;
			CTokenCString TokenFile(szLongPathName, strlen_ext(szLongPathName), '/', 1);

			tDPPF.strFileName	= TokenFile.NextToken();
			tDPPF.strFilePath	= TokenFile.RemainValue();
			tDPPF.nRegDate		= GetCurrentDateTimeInt();
			tDPPF.nPtnRisk		= PTN_FB_PTN_RISK_LOCAL;

			GatherFileInfo(&tDPPF);
			
			m_tSendIDList.push_back(tDPPF.nID);
		}
	}	
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(ERR_SUCCESS);
		SendData_Link(m_nPktType, m_nPktCode, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
		if(!pdata)
			return -1;

		String strLWCFileName;
		strLWCFileName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->strSaveName.c_str());

		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		pdata->strFileHash = szHash;

		t_ManagePoNcPtnLo->EditPoNcPtnLo(*pdata);
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	{
		if(t_LogicMgrInfoProcReputation)
			t_LogicMgrInfoProcReputation->OnTimer();
	}
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS_END);
}
//---------------------------------------------------------------------------

INT32		CLogicPoNcPtnLoPtn::AnalyzePkt_FromLink_Del_Ext()
{
	UINT32 nID = 0;

	if(RecvToken.TokenDel_IDList(m_tIDList) < 0)		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);		

	{
		TListIDItor begin, end;
		begin = m_tIDList.begin();	end = m_tIDList.end();
		for(begin; begin != end; begin++)
		{
			m_tSendIDList.push_back(*begin);
		}
	}	
	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(ERR_SUCCESS);
		SendToken.TokenAdd_IDList(m_tSendIDList);
		SendData_Link(m_nPktType, m_nPktCode, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}

	{
		PDB_PO_NC_PTN_LO pdata = (PDB_PO_NC_PTN_LO)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_NC_PTN_LO);
		if(!pdata)
			return -1;

		String strLWCFileName;
		strLWCFileName = SPrintf("%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, pdata->strSaveName.c_str());

		CHAR szHash[CHAR_MAX_SIZE] = {0, };
		pdata->strFileHash = szHash;

		t_ManagePoNcPtnLo->EditPoNcPtnLo(*pdata);
	}

	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}

	{
		if(t_LogicMgrInfoProcReputation)
			t_LogicMgrInfoProcReputation->OnTimer();
	}
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS_END);
}
//---------------------------------------------------------------------------

INT32		CLogicPoNcPtnLoPtn::AnalyzePkt_FromLink_Ext_Sync()
{
	SendPkt_Sync();

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

void		CLogicPoNcPtnLoPtn::SendPkt_Sync()
{
/*
	INT32 nOnceNum = 0, nSendedNum = 0;;
	INT32 nTotalNum = t_MMPPLWC->t_ManagePtnProcFile->Count();
	{
		SendToken.TokenAdd_32(nTotalNum);
		SendData_Link(m_nSessionID, G_TYPE_PO_NC_PTN_LO_PTN, G_CODE_COMMON_MULTI_BEGIN, SendToken);
		SendToken.Clear();
	}

	do
	{
		nOnceNum = t_MMPPLWC->t_ManagePtnProcFile->SetPkt(SendToken, m_nSessionID, 1000);
		if(nOnceNum)
		{
			SendData_Link(m_nSessionID, G_TYPE_PO_NC_PTN_LO_PTN, G_CODE_COMMON_MULTI_LOOP, SendToken);
			SendToken.Clear();

			nSendedNum += nOnceNum;
			WriteLogN("[%s] send init data loop [%d/%d]", m_strLogicName.c_str(), nSendedNum, nTotalNum);
		}
	}while(nSendedNum < nTotalNum);


	{
		SendToken.TokenAdd_32(nSendedNum);
		SendData_Link(m_nSessionID, G_TYPE_PO_NC_PTN_LO_PTN, G_CODE_COMMON_MULTI_END, SendToken);
		SendToken.Clear();
	}

	{
		if(pdata_ver)
		{
			t_MMPPLWC->t_ManagePtnVersion->SetPkt(pdata_ver, SendToken);
			SendData_Link(m_nSessionID, G_TYPE_PO_NC_PTN_LO_PTN, G_CODE_COMMON_INFO, SendToken);
			SendToken.Clear();
		}		
	}
	return;
*/
}
//---------------------------------------------------------------------------

INT32		CLogicPoNcPtnLoPtn::GatherFileInfo(PDB_PTN_PROC_FILE pdata)
{
	CHAR szFileInfo[CHAR_MAX_SIZE] = {0, };

	if(pdata)
	{
		CString strFullFileName;
		strFullFileName.Format("%s/%s", (char *)(LPCTSTR)pdata->strFilePath, (char *)(LPCTSTR)pdata->strFileName);

		pdata->nRegDate		= GetCurrentDateTimeInt();
		pdata->nUsedMode	= STATUS_USED_MODE_ON;
	}
	return 0;
}
//---------------------------------------------------------------------------



