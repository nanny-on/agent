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
#include "LogicInitLink.h"

//---------------------------------------------------------------------------

CLogicInitLink*		t_LogicInitLink = NULL;

//---------------------------------------------------------------------------

CLogicInitLink::CLogicInitLink()
{
	m_strLogicName = "logic init link";
}
//---------------------------------------------------------------------------

CLogicInitLink::~CLogicInitLink()
{
}
//---------------------------------------------------------------------------

void		CLogicInitLink::AnalyzePkt_Init(PPKT_DATA m_tPktData)
{
	InitBaseMember(m_tPktData);
	switch(m_nPktCode)
    {
        case G_CODE_INIT_REQUEST:			AnalyzePkt_Init_Request();		break;
        case G_CODE_INIT_LAST:				AnalyzePkt_Init_Last();			break;
    	default:
        {
        	WriteLogE("[logic init] not define pkt code from link : [%X][%X]", m_nPktType, m_nPktCode);
            return;
        }
    }
	InitBaseMember();
    return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::AnalyzePkt_Init_Request()
{
    UINT64 	nRequestInfo;
        
    if(!RecvToken.TokenDel_64(nRequestInfo))			{	m_nDbgPos++;		goto INVALID_PKT;	}	
	
	{
		if((nRequestInfo & G_INIT_REQUEST_TYPE_COMM) != 0)					{		SendPkt_Init_Common();			}
		if((nRequestInfo & G_INIT_REQUEST_TYPE_ADMIN) != 0)					{		SendPkt_Init_Admin();			}
    	if((nRequestInfo & G_INIT_REQUEST_TYPE_HOST) != 0)					{		SendPkt_Init_Host();			}
		if((nRequestInfo & G_INIT_REQUEST_TYPE_GROUP) != 0)					{		SendPkt_Init_Group();			}  
		if((nRequestInfo & G_INIT_REQUEST_TYPE_USER) != 0)					{		SendPkt_Init_User();			}
		if((nRequestInfo & G_INIT_REQUEST_TYPE_ENV) != 0)					{		SendPkt_Init_Env();				}       
		if((nRequestInfo & G_INIT_REQUEST_TYPE_PTN) != 0)					{		SendPkt_Init_Ptn();				}       
		if((nRequestInfo & G_INIT_REQUEST_TYPE_LOCAL_ENV) != 0)				{		SendPkt_Init_LocalEnv();		}       
		if((nRequestInfo & G_INIT_REQUEST_TYPE_POLICY) != 0)				{		SendPkt_Init_Policy();			}       
		if((nRequestInfo & G_INIT_REQUEST_TYPE_INFO) != 0)					{		SendPkt_Init_Info();			}      
	}	

    SendPkt_Init_Data();
	
	m_tPktData->hdr.code = G_CODE_INIT_REQUEST;
	m_tMutex.Lock();
    SendToken.TokenAdd_32(ERR_SUCCESS);
	SendData_Link(m_tPktData, SendToken);
	m_tMutex.UnLock();
    return;

INVALID_PKT:
    WriteLogE("[logic init] recv invalid pkt from link : [%x][%x]:[%d]", m_nPktType, m_nPktCode, m_nDbgPos);
	m_tMutex.Lock();
    SendToken.TokenAdd_32(ERR_SOCKET_CLT_TO_SVR_INVALID_PKT);
	SendData_Link(m_tPktData, SendToken);
	m_tMutex.UnLock();
    return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::AnalyzePkt_Init_Last()
{
	{
		SendPkt_InitLast_DelLastNotify();
	}

	WriteLogN("init link last.");
	return;

// INVALID_PKT:
// 	WriteLogE("[logic init] recv invalid pkt from link : [%x][%x]:[%d]", m_nPktType, m_nPktCode, m_nDbgPos);
// 	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Data()
{
	INT32 nPktNum = 0;

	{
		m_tMutex.Lock();
		nPktNum = t_ManageLogEvent->SetPktSyncMode(SendToken, SS_LOG_EVENT_HOST_SYNC_MODE_AGT);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_EVENT, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_event][%d]", nPktNum);
	}
	
	{
		m_tMutex.Lock();
		t_ManageLogDoc->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_DOC, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_doc]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogSecu->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_SECU, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_secu]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogDeployFile->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_DEPLOY_FILE, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_deploy_file]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogDocDScan->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_DOC_DSCAN, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_doc_dscan]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogPatch->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_PATCH, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_patch]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogDevice->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_DEVICE, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_device]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogRs->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_RS, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_rs]");
	}

	{
		m_tMutex.Lock();
		t_ManageLogRsBk->SetPkt_Link(SendToken);
		SendData_Link(m_nSessionID, G_TYPE_INIT, G_CODE_INIT_LOG_RS_BK, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [log_rs_bk]");
	}

	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Common()
{
	{
		m_tPktData->hdr.code = G_CODE_INIT_ERR_CODE;
		m_tMutex.Lock();
		t_ManageErrorCode->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [error_code][%d]", t_ManageErrorCode->Count());
	}

	{
		m_tMutex.Lock();
		m_tPktData->hdr.code = G_CODE_INIT_SITE_FILE;
		t_ManageSiteFile->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [site_file][%d]", t_ManageSiteFile->Count());
	}


	{
		m_tPktData->hdr.code = G_CODE_INIT_SITE_VULN;
		m_tMutex.Lock();
		t_ManageSiteVuln->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [site_vuln][%d]", t_ManageSiteVuln->Count());
	}
	{
		m_tPktData->hdr.code = G_CODE_INIT_SITE_VULN_SCAN;
		m_tMutex.Lock();
		t_ManageSiteVulnScan->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [site_vuln_scan][%d]", t_ManageSiteVulnScan->Count());
	}
// 	{
// 		m_tPktData->hdr.code = G_CODE_INIT_SITE_VULN_REPAIR;
// 		t_ManageSiteVulnRepair->SetPkt(SendToken);
// 		SendData_Link(m_tPktData, SendToken);
// 		SendToken.Clear();
// 		WriteLogN("[logic init] send init link data [site_vuln_repair][%d]", t_ManageSiteVulnRepair->Count());
// 	}
// 	{
// 		m_tPktData->hdr.code = G_CODE_INIT_SITE_VULN_LOCK;
// 		t_ManageSiteVulnLock->SetPkt(SendToken);
// 		SendData_Link(m_tPktData, SendToken);
// 		SendToken.Clear();
// 		WriteLogN("[logic init] send init link data [site_vuln_lock][%d]", t_ManageSiteVulnLock->Count());
// 	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Host()
{
	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST;
		m_tMutex.Lock();
        t_ManageHost->SetPktHost(SendToken);
        SendData_Link(m_tPktData, SendToken);
        SendToken.Clear();
		m_tMutex.UnLock();
        WriteLogN("[logic init] send init link data [host][%d]", t_ManageHost->Count());
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST_POLICY;
		m_tMutex.Lock();
		t_ManageHostPolicy->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_policy][%d]", t_ManageHostPolicy->Count());
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST_SYS;
		m_tMutex.Lock();
		t_ManageHostSys->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_sys][%d]", t_ManageHostSys->Count());
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST_SW;
		m_tMutex.Lock();
		t_ManageHostSw->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_sw][%d]", t_ManageHostSw->Count());
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST_PATCH;
		m_tMutex.Lock();
		t_ManageHostPatch->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_patch][%d]", t_ManageHostPatch->Count());
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_HOST_VULN_RST;
		m_tMutex.Lock();
		t_ManageHostVulnRst->SetPkt_Link(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_vuln_rst][%d]", t_ManageHostVulnRst->Count());

		m_tPktData->hdr.code = G_CODE_INIT_HOST_VULN_RST_UNIT;
		m_tMutex.Lock();
		t_ManageHostVulnRstUnit->SetPkt_Link(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [host_vuln_rst_unit][%d]", t_ManageHostVulnRstUnit->Count());
	}
	
    return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Group()
{	
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_User()
{
	{
		m_tPktData->hdr.code = G_CODE_INIT_USER;
		m_tMutex.Lock();
		t_ManageUser->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [user][%d]", t_ManageUser->Count());
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Admin()
{
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Env()
{
	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_LICENSE;
		m_tMutex.Lock();
		t_ManageEnvLicense->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_license][%d]", t_ManageEnvLicense->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_POLICY;
		m_tMutex.Lock();
		t_ManageEnvPolicy->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_policy][%d]", t_ManageEnvPolicy->Count());	
	}
		
	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_NOTIFY_INFO;
		m_tMutex.Lock();
		t_ManageEnvNotifyInfo->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_notify_info][%d]", t_ManageEnvNotifyInfo->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_LOG;
		m_tMutex.Lock();
		t_ManageEnvLog->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_log_event][%d]", t_ManageEnvLog->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_LOG_UNIT;
		m_tMutex.Lock();
		t_ManageEnvLogUnit->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_log_event_unit][%d]", t_ManageEnvLogUnit->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_SELF_PROTECT_AGT;
		m_tMutex.Lock();
		t_ManageEnvSelfProtectAgt->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_self_protect_agt][%d]", t_ManageEnvSelfProtectAgt->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_ENV_TRUST_SUBJECT;
		m_tMutex.Lock();
		t_ManageEnvTrustSubject->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [env_trust_subject][%d]", t_ManageEnvTrustSubject->Count());	
	}

	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_LocalEnv()
{
	{
		m_tPktData->hdr.code = G_CODE_INIT_LOCAL_ENV_AUTH;
		m_tMutex.Lock();
		t_ManageLocalEnvAuth->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		m_tMutex.UnLock();
		WriteLogN("[logic init] send init link data [local_env_auth][%d]", t_ManageLocalEnvAuth->Count());	
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Ptn()
{
/*
	{
		TMapID tIDMap;
		t_ManageLinkEnv->GetKeyIDMap(G_TYPE_PTN_PATCH, tIDMap);
		
		m_tPktData->hdr.code = G_CODE_INIT_PTN_PATCH;
//		t_MMPPPatch->t_ManagePtnPatch->SetPkt(tIDMap, SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		WriteLogN("[logic init] send init link data [ptn_patch][%d]", tIDMap.size());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_PTN_VULN;
//		t_MMPPVuln->t_ManagePtnVuln->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		WriteLogN("[logic init] send init link data [ptn_vuln][%d]", t_MMPPVuln->t_ManagePtnVuln->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_PTN_VULN_SCAN;
//		t_MMPPVuln->t_ManagePtnVulnScan->SetPkt(SendToken);
		SendData_Link(m_tPktData, SendToken);
		SendToken.Clear();
		WriteLogN("[logic init] send init link data [ptn_vuln_scan][%d]", t_MMPPVuln->t_ManagePtnVulnScan->Count());	
	}

	{
		m_tPktData->hdr.code = G_CODE_INIT_PTN_PROC_NC_LO_VERSION;
		PDB_PTN_VERSION pdata_ver = t_MMPPLWC->t_ManagePtnVersion->FirstItem();
		if(pdata_ver)
		{
			t_MMPPLWC->t_ManagePtnVersion->SetPkt(pdata_ver, SendToken);
			SendData_Link(m_tPktData, SendToken);
			SendToken.Clear();
			WriteLogN("[logic init] send init link data [ptn_proc_nc_lo_ver][%d]", t_MMPPLWC->t_ManagePtnVersion->Count());	
		}
	}
	return;
*/
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_Init_Info()
{
	{
		{
			m_tPktData->hdr.code = G_CODE_INIT_INFO_PROC_REPUTATION;
			m_tMutex.Lock();
			t_ManageProcessInfo->SetPkt(SendToken);
			SendData_Link(m_tPktData, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
			WriteLogN("[logic init] send init link data [info_proc_reputation][%d]", t_ManageProcessInfo->Count());
		}
	}
	return;
}
//---------------------------------------------------------------------------

void		CLogicInitLink::SendPkt_InitLast_DelLastNotify()
{
	TListMemFindOrderInfo tMemFindOrderInfoList;
	if(t_ManageDelLastNotify)
		t_ManageDelLastNotify->GetRemainNotifyList(tMemFindOrderInfoList);

	TListMemFindOrderInfoItor begin, end;
	begin = tMemFindOrderInfoList.begin();		end = tMemFindOrderInfoList.end();
	for(; begin != end; begin++)
	{
		t_LogicPoFaClear->SendPkt_Del_Last(*begin);
		if(t_ManageDelLastNotify)
			t_ManageDelLastNotify->DelDelLastNotify(begin->nID);
	}

	return;
}
//---------------------------------------------------------------------------
