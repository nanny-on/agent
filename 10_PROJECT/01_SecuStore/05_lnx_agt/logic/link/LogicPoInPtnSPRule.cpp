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
#include "LogicPoInPtnSPRule.h"

//---------------------------------------------------------------------------

CLogicPoInPtnSPRule*		t_LogicPoInPtnSPRule = NULL;

//---------------------------------------------------------------------------

CLogicPoInPtnSPRule::CLogicPoInPtnSPRule()
{	
	m_strLogicName		= "link po in ptn sp rule";
		
	m_nEvtObjType		= 0;
	m_nEvtObjCode		= 0;
}
//---------------------------------------------------------------------------

CLogicPoInPtnSPRule::~CLogicPoInPtnSPRule()
{

}
//---------------------------------------------------------------------------

INT32	CLogicPoInPtnSPRule::AnalyzePkt_FromLink_Ext()
{
	switch(m_nPktCode)
	{		
		case G_CODE_COMMON_REQUEST:		AnalyzePkt_FromLink_Ext_Req();		break;
		default:						break;		
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicPoInPtnSPRule::AnalyzePkt_FromLink_Ext_Req()
{
	DB_PO_IN_PTN_SP_RULE data;
	PDB_PO_IN_PTN_SP pdata_sp = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);

	if(CheckPktResult(RecvToken))
	{
		WriteLogE("[%s] check pkt result is : [%x]:[pt:%x][pc:%x]", m_strLogicName.c_str(), m_nPktRst, m_nPktType, m_nPktCode);
		return AZPKT_CB_RTN_SUCCESS_END;
	}
	
	if(!RecvToken.TokenDel_32(m_nRecvID))					return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(data.tDPH.strName) < 0)	return AZPKT_CB_RTN_PKT_INVALID;
	if( RecvToken.TokenDel_String(data.tDPH.strDescr) < 0)	return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(data.nReqLevel))				return AZPKT_CB_RTN_PKT_INVALID;
	if(!RecvToken.TokenDel_32(data.nReqDay))				return AZPKT_CB_RTN_PKT_INVALID;
	
	{
		if(!pdata_sp)
		{
			return AZPKT_CB_RTN_SUCCESS_END;
		}
	}

	{
		PDB_LOG_SECU pdata_log = t_ManageLogSecu->FindItem(m_nRecvID);
		if(!pdata_log)		return AZPKT_CB_RTN_SUCCESS_END;

		{
			DWORD dwFileType = AS_INVALID_FILE;
			ASI_WENG_WL_EX tAWWE;
			memset(&tAWWE, 0, sizeof(ASI_WENG_WL_EX));

			String strFileFullPath;
			strFileFullPath = SPrintf("%s/%s", pdata_log->strObjectPath.c_str(), pdata_log->strObjectName.c_str());

			t_ASIWENGDLLUtil->GetWL(strFileFullPath.c_str(), (PVOID)&tAWWE, (DWORD)sizeof(ASI_WENG_WL_EX), &dwFileType);
			if(dwFileType == AS_INVALID_FILE)
			{
				return AZPKT_CB_RTN_SUCCESS_END;
			}

//			data.strFileKey = BinToHexLen((PBYTE)(tAWWE.acWhiteHash), SHA512_BLOCK_SIZE);
			data.strFileKey = tAWWE.acWhiteHash;
			data.strFilePath = pdata_log->strObjectPath;
			data.strFileName = pdata_log->strObjectName;
			data.tDPH.nUsedMode = STATUS_USED_MODE_ON;
		}	
	}

	{
		if(data.strFileKey.empty())	
		{
			return AZPKT_CB_RTN_SUCCESS_END;
		}

		if(t_ManagePoInPtnSPRule->FindSKeyID(data.strFileKey))
		{
			return AZPKT_CB_RTN_SUCCESS_END;
		}
	}

	if(ChkAutoConfirm(&data))
	{
		WriteLogN("[%s] request auto check result : [%d]", m_strLogicName.c_str(), g_nErrRtn);
		t_LogicMgrPoInPtnSPRule->SendPkt_Req(&data);
	}
	else
	{
		data.tDPH.nID = t_ManagePoInPtnSPRule->GetNextLocalID();

		if(t_ManagePoInPtnSPRule->ApplyPoInPtnSPRule(data))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);

			return AZPKT_CB_RTN_SUCCESS_END;
		}
		{
			m_tMutex.Lock();
			t_ManagePoInPtnSPRule->SetPkt(SendToken);
			SendData_Link(G_TYPE_PO_IN_PTN_SP_RULE, G_CODE_COMMON_EDIT, SendToken);
			SendToken.Clear();
			m_tMutex.UnLock();
		}

		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
	}
	
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}

INT32		CLogicPoInPtnSPRule::SendMgr_Ext_Req(PPIPE_PO_IN_REQ pPoInReq)
{
	DB_PO_IN_PTN_SP_RULE data;
	PDB_PO_IN_PTN_SP pdata_sp = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);

	if(pPoInReq == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strLogicName.c_str());
		return -1;
	}

	if(is_file(pPoInReq->acReqFullPath) != REG_FILE)
	{
		WriteLogE("[%s] invalid request path (%s) (%d)", m_strLogicName.c_str(), pPoInReq->acReqFullPath, errno);
		return -2;
	}

	if(pPoInReq->acReqPath[0] == 0)
	{
		if(get_dirname(pPoInReq->acReqFullPath, pPoInReq->acReqPath, MAX_PATH-1) == NULL)
		{
			WriteLogE("[%s] fail to get dir name (%s)", m_strLogicName.c_str(), pPoInReq->acReqFullPath);
			return -3;
		}
		pPoInReq->acReqPath[MAX_PATH-1] = 0;
	}
	if(pPoInReq->acReqFile[0] == 0)
	{
		if(get_basename(pPoInReq->acReqFullPath, pPoInReq->acReqFile, MAX_FILE_NAME-1) == NULL)
		{
			WriteLogE("[%s] fail to get base name (%s)", m_strLogicName.c_str(), pPoInReq->acReqFullPath);
			return -4;
		}
		pPoInReq->acReqFile[MAX_FILE_NAME-1] = 0;
	}

	DWORD dwFileType = AS_INVALID_FILE;
	ASI_WENG_WL_EX tAWWE;
	memset(&tAWWE, 0, sizeof(ASI_WENG_WL_EX));

	t_ASIWENGDLLUtil->GetWL(pPoInReq->acReqFullPath, (PVOID)&tAWWE, (DWORD)sizeof(ASI_WENG_WL_EX), &dwFileType);
	if(dwFileType == AS_INVALID_FILE)
	{
		WriteLogE("[%s] invalid execute file type (%s) (%d)", m_strLogicName.c_str(), pPoInReq->acReqFullPath, dwFileType);
		return -5;
	}

	data.strFileKey = tAWWE.acWhiteHash;
	data.strFilePath = pPoInReq->acReqPath;
	data.strFileName = pPoInReq->acReqFile;
	data.nReqDay = pPoInReq->nReqDay;
	data.nReqLevel = pPoInReq->nReqLevel;
	data.tDPH.nUsedMode = STATUS_USED_MODE_ON;

	t_LogicMgrPoInPtnSPRule->SendPkt_Req(&data);
	return 0;
}

//---------------------------------------------------------------------------

INT32		CLogicPoInPtnSPRule::AnalyzePkt_FromLink_Del_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicPoInPtnSPRule::ChkAutoConfirm(PDB_PO_IN_PTN_SP_RULE pdata)
{
	INT32 nMgrSvrAuthStatus = CLIENT_CON_STATUS_DISCONNECTED;
	PDB_PO_IN_PTN_SP pdata_pol = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
	if(!pdata_pol)		return -1;

	t_EnvInfoOp->GetMgrSvrAuthStatus(nMgrSvrAuthStatus);

	if(pdata_pol->tDPH.nUsedMode == STATUS_USED_MODE_OFF)	return -1;
	if((pdata_pol->tDPH.nExtOption & SS_PO_IN_PTN_SP_EXT_OPTION_RULE_CHECK_AGENT) == 0)		return -2;
	if(nMgrSvrAuthStatus == CLIENT_CON_STATUS_CONNECTED)						return -3;

	do 
	{
		PDB_SITE_FILE pdata_sf = t_ManageSiteFile->FindItem(pdata->nFileID);
		if(!pdata_sf)	continue;

		DB_FILE_INFO tSrcDFI;
		{
			tSrcDFI.strName = pdata->strFileName;
			tSrcDFI.strPath = pdata->strFilePath;
		}

		{
			CMatchFilterUtil tMatchFilterUtil;
			TListID tUnitIDList;
			t_ManagePoInPtnSPPkg->GetUnitIDListByChkOrder(pdata_pol->tDPH.nID, tUnitIDList);

			TListIDItor begin, end;
			begin = tUnitIDList.begin();	end = tUnitIDList.end();
			for(begin; begin != end; begin++)
			{
				PDB_PO_IN_PTN_SP_UNIT pdata_unit = t_ManagePoInPtnSPUnit->FindItem(*begin);
				if(!pdata_unit)		continue;

				if(pdata_unit->tDPH.nUsedMode != STATUS_USED_MODE_ON)	continue;

				if(tMatchFilterUtil.IsMatchFilter(pdata_sf, tSrcDFI, pdata_unit->tDFI, pdata_unit->nFltChkType))	continue;

				if(pdata_unit->nConfirmLevel == SS_PO_IN_PTN_SP_REQ_LEVEL_NONE)
				{
					continue;
				}

				switch(pdata_unit->nBlockMode)
				{
					case SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_ALLOW:
					{
						if(pdata_unit->nConfirmLevel > pdata->nReqLevel)
						{
							WriteLogN("[%s] request auto check result is wait because invalid level : allow [%d] > [%d]", m_strLogicName.c_str(), pdata_unit->nConfirmLevel, pdata->nReqLevel);
							return 0;
						}		
						break;
					}
					case SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_DENY:
					{
						if(pdata_unit->nConfirmLevel < pdata->nReqLevel)
						{
							WriteLogN("[%s] request auto check result is wait because invalid level : deny [%d] < [%d]", m_strLogicName.c_str(), pdata_unit->nConfirmLevel, pdata->nReqLevel);
							return 0;
						}		
						break;
					}
				}

				{
					pdata->nConfirmType	= SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_AGENT_POLICY_UNIT;
					pdata->nConfirmID = pdata_unit->tDPH.nID;				
					pdata->nConfirmRst = pdata_unit->nBlockMode;

					if(pdata_unit->nConfirmDay == 0)		pdata->nUseDay = pdata->nReqDay;
					else if(pdata->nReqDay == 0)			pdata->nUseDay = pdata_unit->nConfirmDay;
					else
					{
						pdata->nUseDay = (pdata_unit->nConfirmDay > pdata->nReqDay ? pdata->nReqDay : pdata_unit->nConfirmDay);
					}
					pdata->nEvtDate = GetCurrentDateTimeInt();
					return 0;
				}
			}
		}
	} while (FALSE);
	

	if(pdata_pol->nConfirmLevel != SS_PO_IN_PTN_SP_REQ_LEVEL_NONE && pdata_pol->nConfirmLevel <= pdata->nReqLevel)
	{
		pdata->nConfirmType = SS_PO_IN_PTN_SP_RULE_CONFIRM_TYPE_AGENT_POLICY;
		pdata->nConfirmID = pdata_pol->tDPH.nID;
		pdata->nConfirmRst = SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_ALLOW;
		if(pdata_pol->nConfirmDay == 0)		pdata->nUseDay = pdata->nReqDay;
		else if(pdata->nReqDay == 0)		pdata->nUseDay = pdata_pol->nConfirmDay;
		else
		{
			pdata->nUseDay = (pdata_pol->nConfirmDay > pdata->nReqDay ? pdata->nReqDay : pdata_pol->nConfirmDay);
		}
		pdata->nEvtDate = GetCurrentDateTimeInt();		
	}
	return 0;
}
//---------------------------------------------------------------------------


INT32		CLogicPoInPtnSPRule::IsExistMatchInfo(PDB_PO_IN_PTN_SP_RULE pdata)
{
	PDB_PO_IN_PTN_SP pdata_pol = (PDB_PO_IN_PTN_SP)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_IN_PTN_SP);
	if(!pdata_pol)	return -1;


	String strMatchSrc = "";

	if(pdata_pol->tDPH.nExtOption & SS_PO_IN_PTN_SP_EXT_OPTION_PATH_BASED_INSPECTION)
	{
		strMatchSrc = SPrintf("%s_%s_%s", pdata->strFileKey.c_str(), pdata->strFilePath.c_str(), pdata->strFileName.c_str());
	}
	else
	{
		strMatchSrc = pdata->strFileKey;
	}

	{
		TListID tIDList;
		t_ManagePoInPtnSPRule->GetItemIDList(tIDList);

		TListIDRItor begin, end;
		begin = tIDList.rbegin();	end = tIDList.rend();
		for(begin; begin != end; begin++)
		{
			PDB_PO_IN_PTN_SP_RULE pdata_cur = t_ManagePoInPtnSPRule->FindItem(*begin);
			if(!pdata_cur)	continue;
			if(pdata->tDPH.nUsedMode == STATUS_USED_MODE_OFF)								continue;
			if(pdata->nConfirmRst == SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END)				continue;
			if(pdata_cur->nEndDate)															continue;
			{
				if(pdata->nConfirmRst != SS_PO_IN_PTN_SP_RULE_CONFIRM_RST_TYPE_END_FOREVER)	continue;
			}

			String strMatchTar = "";
			if(pdata_pol->tDPH.nExtOption & SS_PO_IN_PTN_SP_EXT_OPTION_PATH_BASED_INSPECTION)
			{
				strMatchTar = SPrintf("%s_%s_%s", pdata_cur->strFileKey.c_str(), pdata_cur->strFilePath.c_str(), pdata_cur->strFileName.c_str());
			}
			else
			{
				strMatchTar = pdata_cur->strFileKey;
			}


			if(strMatchSrc == strMatchTar)		return 1;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
