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
#include "LogicMgrInfoProcReputation.h"

//---------------------------------------------------------------------------

CLogicMgrInfoProcReputation*		t_LogicMgrInfoProcReputation = NULL;

//---------------------------------------------------------------------------

CLogicMgrInfoProcReputation::CLogicMgrInfoProcReputation()
{
	t_LogicInfoProcReputation		= new CLogicInfoProcReputation();
	t_ManageInfoProcReputation		= new CManageInfoProcReputation();
	t_ManageProcessInfo				= new CManageProcessInfo();
	
	t_ManageInfoProcReputation->LoadDBMS();
	
	t_ManageInfoProcReputation->InitHash();

	m_strLogicName		= "mgr agt info proc reputation";
	
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;	
	
	m_nControlType		= 0;
	m_nEvtObjType		= EVENT_OBJECT_TYPE_INFO;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_INFO_PROC_REPUTATION;
}
//---------------------------------------------------------------------------

CLogicMgrInfoProcReputation::~CLogicMgrInfoProcReputation()
{
	SAFE_DELETE(t_ManageInfoProcReputation);
	SAFE_DELETE(t_ManageProcessInfo);

	SAFE_DELETE(t_LogicInfoProcReputation);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrInfoProcReputation::AnalyzePkt_FromMgr_Ext()
{
	UINT32 nRtn = AZPKT_CB_RTN_SUCCESS;
	switch(m_nPktCode)
	{
		case G_CODE_COMMON_SYNC:		nRtn = AnalyzePkt_FromMgr_Ext_Sync();			break;
		default:
		{
			nRtn = AZPKT_CB_RTN_PKT_NOT_DEFINE_CODE;
			break;
		}
	}

	return SetHdrAndRtn(nRtn);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrInfoProcReputation::AnalyzePkt_FromMgr_Ext_Sync()
{	
	TListDBInfoProcReputation	tDBInfoProcReputationList;

	if(!RecvToken.TokenDel_32(m_nRecvNum))							return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_INFO_PROC_REPUTATION		data;	

		if( t_ManageInfoProcReputation->GetPkt(RecvToken, data))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tDBInfoProcReputationList.push_back(data);
	}

	{
		TListDBInfoProcReputationItor begin, end;
		begin = tDBInfoProcReputationList.begin();	end = tDBInfoProcReputationList.end();
		for(begin; begin != end; begin++)
		{
			PDB_SITE_FILE pdata_sf = t_ManageSiteFile->FindPosSKeyID_Item(begin->strFileKey, SS_SITE_FILE_KEY_TYPE_FE);
			if(!pdata_sf)	continue;

			PDB_INFO_PROC_REPUTATION pdata = t_ManageInfoProcReputation->FindKeyItem(pdata_sf->nID);
			if(!pdata)	continue;

			pdata->nReputation = begin->nReputation;
			pdata->nNumOfUser = begin->nNumOfUser;
			pdata->nEditDate = GetCurrentYearMonDayHour();
			pdata->nSendPktRef = 0;
						
			if(t_ManageInfoProcReputation->EditInfoProcReputation(*pdata))
			{
				SetDLEH_EC(g_nErrRtn);
				WriteLogE("[%s] edit unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
				continue;
			}
		}

		t_ManageInfoProcReputation->InitHash();
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrInfoProcReputation::OnTimer_Logic()
{
	UINT32 nEnvLicense = 0;
	{
		nEnvLicense = (t_ManageEnvLicense->IsInvalidRight(SS_PACKAGE_TYPE_NOMS, SS_CLASS_TYPE_NO_PTN, 0, 0) ? 0 : SS_PACKAGE_TYPE_NOMS);
		nEnvLicense = (t_ManageEnvLicense->IsInvalidRight(SS_PACKAGE_TYPE_NCMS, 0, 0, 0) ? 0 : SS_PACKAGE_TYPE_NCMS);
	}

	if(!nEnvLicense)	return -1;


	t_ManageProcessInfo->Clear();

	{
		TListResInfoProcInfo tProcList;
		if(t_ProcInfoDLLUtil)
			t_ProcInfoDLLUtil->GetProcessInfo(tProcList);
		if(tProcList.size() > 0)
		{
			TListResInfoProcInfoItor begin, end;
			begin = tProcList.begin();	end = tProcList.end();
			for(begin; begin != end; begin++)
			{
				if(begin->pbi.szProcessFullName && begin->pbi.szProcessFullName[0] == 0)
				{
					continue;
				}
				
				String strFeKey = t_LogicMgrSiteFile->GetWEngKey(begin->pbi.szProcessFullName);
								
				if (strFeKey.empty())
				{
//					TRACE("[%s] get file weng key fail : [err:%d][%d][%s]\n", __FUNCTION__, GetLastError(), begin->pbi.nPID, begin->pbi.szProcessFullName);
					continue;
				}

				DB_INFO_PROC_REPUTATION data;
				PDB_SITE_FILE pdata_sf = t_ManageSiteFile->FindPosSKeyID_Item(strFeKey, SS_SITE_FILE_KEY_TYPE_FE);
				if(pdata_sf)
				{
					PDB_INFO_PROC_REPUTATION pdata_pr = t_ManageInfoProcReputation->FindKeyItem(pdata_sf->nID);
					if(!pdata_pr)
					{
						data.nSiteFileID = pdata_sf->nID;
						t_ManageInfoProcReputation->AddInfoProcReputation(data);
					}
					else
					{
						data = *pdata_pr;
					}
				}
				else
				{
					t_LogicMgrSiteFile->CheckSiteFile(begin->pbi.szProcessFullName);
				}

				{
					MEM_PROCESS_INFO tMPI;
					tMPI.nPID		= begin->pbi.nPID;
					tMPI.nPPID		= begin->pbi.nPPID;
					tMPI.strName	= begin->pbi.szProcessName;
					tMPI.strPath	= begin->pbi.szProcessFullName;
					tMPI.nCpuValue	= begin->psi.nCpuValue;
					tMPI.nMemValue	= begin->psi.nMemValue;
					tMPI.strProduct	= begin->pai.szProductName;
					tMPI.strCompany	= begin->pai.szCompanyName;

					tMPI.nReputation	= data.nReputation;
					tMPI.nNumOfUser		= data.nNumOfUser;

					if(nEnvLicense & SS_PACKAGE_TYPE_NOMS)
					{						
						if(t_MMPPGWO && t_MMPPGWO->t_ManagePtnProcFile->FindSKeyID(strFeKey.c_str()))
							tMPI.nNcPtnMatchType += SS_POLICY_TYPE_IN_PTN_WL;

						if(t_MMPPGBO && t_MMPPGBO->t_ManagePtnProcFile->FindSKeyID(strFeKey.c_str()))
							tMPI.nNcPtnMatchType += SS_POLICY_TYPE_IN_PTN_BL;		
					}
/*					
					if(nEnvLicense & SS_PACKAGE_TYPE_NCMS)
					{
						if(t_MMPPLWC->t_ManagePtnProcFile->FindSKeyID(strFeKey))
							tMPI.nNcPtnMatchType += SS_POLICY_TYPE_NC_PTN_LO;

						if(t_MMPPGWC->t_ManagePtnProcFile->FindSKeyID(strFeKey))
							tMPI.nNcPtnMatchType += SS_POLICY_TYPE_NC_PTN_WL;

						if(t_MMPPGBC->t_ManagePtnProcFile->FindSKeyID(strFeKey))
							tMPI.nNcPtnMatchType += SS_POLICY_TYPE_NC_PTN_BL;						
						
					}
*/
					t_ManageProcessInfo->AddProcessInfo(tMPI);
				}				
			}
		}
	}

	if(t_ManageProcessInfo->Count())
	{
		m_tMutex.Lock();
		t_ManageProcessInfo->SetPkt(SendToken);
		SendData_Link(G_TYPE_INFO_PROC_REPUTATION, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		SendPkt_Sync();
	}
	return 0;
}
//---------------------------------------------------------------------------

void		CLogicMgrInfoProcReputation::SendPkt_Sync()
{
	TListID tIDList;
	TListDBInfoProcReputation tSendList;
	t_ManageInfoProcReputation->GetItemIDList(tIDList);

	{
		UINT32 nCurTime = GetCurrentYearMonDayHour();

		TListIDItor begin, end;
		begin = tIDList.begin();	end = tIDList.end();
		for(begin; begin != end; begin++)
		{
			PDB_INFO_PROC_REPUTATION pdata = t_ManageInfoProcReputation->FindItem(*begin);
			if(!pdata)	continue;

			if(nCurTime - pdata->nEditDate < 24)	continue;

			pdata->nSendPktRef += 1;
			if(pdata->nSendPktRef > 10)
			{
				pdata->nEditDate = GetCurrentYearMonDayHour(GetCurrentDateTimeInt() - (3600 * 20), 0);
				pdata->nSendPktRef = 0;
				continue;
			}

			PDB_SITE_FILE pdata_sf = t_ManageSiteFile->FindItem(pdata->nSiteFileID);
			if(!pdata_sf)	continue;

			pdata->strFileKey = pdata_sf->strFeKey;
			tSendList.push_back(*pdata);
		}
	}

	if(tSendList.size() == 0)		return;

	{
		m_tMutex.Lock();
		SendToken.TokenAdd_32(tSendList.size());

		TListDBInfoProcReputationItor begin, end;
		begin = tSendList.begin();	end = tSendList.end();
		for(begin; begin != end; begin++)
		{
			t_ManageInfoProcReputation->SetPkt(&(*begin), SendToken);
		}
		SendData_Mgr(G_TYPE_INFO_PROC_REPUTATION, G_CODE_COMMON_SYNC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
	}	


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
