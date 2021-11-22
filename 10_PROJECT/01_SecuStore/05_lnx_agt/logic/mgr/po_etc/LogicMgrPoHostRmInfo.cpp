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
#include "as_hash.h"
#include "com_struct.h"
#include "LogicMgrPoHostRmInfo.h"

//---------------------------------------------------------------------------

CLogicMgrPoHostRmInfo*		t_LogicMgrPoHostRmInfo = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoHostRmInfo::CLogicMgrPoHostRmInfo()
{
	t_ManagePoHostRmInfo	= new CManagePoHostRmInfo();
	t_ManagePoHostRmInfo->LoadDBMS();
	t_ManagePoHostRmInfo->InitHash();

	m_strLogicName		= "mgr po host rm info";

	m_nPolicyType		= SS_POLICY_TYPE_HOST_RM_INFO;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_HOST_RM_INFO;
}
//---------------------------------------------------------------------------

CLogicMgrPoHostRmInfo::~CLogicMgrPoHostRmInfo()
{
	SAFE_DELETE(t_ManagePoHostRmInfo);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRmInfo::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRmInfo::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_HOST_RM_INFO pdphri = NULL;
	DB_PO_HOST_RM_INFO dphri;
	m_tDPH = &(dphri.tDPH);

	if( t_ManagePoHostRmInfo->GetPkt(RecvToken, dphri))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

	pdphri = (PDB_PO_HOST_RM_INFO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdphri)
	{
		t_ManagePoHostRmInfo->DelPoHostRmInfo(pdphri->tDPH.nID);
	}
	
	{
		if(SetER(t_ManagePoHostRmInfo->ApplyPoHostRmInfo(dphri)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRmInfo::ApplyPolicy()
{
	int nHashType = -1;
	int nRetVal = 0;
	char szHashed[CHAR_MAX_SIZE] = {0, };
	PDB_PO_HOST_RM_INFO pdphri = (PDB_PO_HOST_RM_INFO)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdphri)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -1;
	}

	switch (pdphri->nRmPwType)
	{
		case 0 :	// nRmPwType = 0 : SHA256 ÀúÀå
			nHashType = ASIHASH_SHA_LEN_TYPE_256;
			break;

		default:
			WriteLogE("[%s] unknown nRmPwType (%d).", m_strLogicName.c_str(), pdphri->nRmPwType);
			return -2;
	}


	nRetVal = SHAString(nHashType, pdphri->strRmPw.c_str(), pdphri->strRmPw.length(), szHashed, CHAR_MAX_SIZE);
	if (nRetVal != 0)
	{
		WriteLogE("[%s] can not make hash (%d).", m_strLogicName.c_str(), nRetVal);
		return -3;
	}

	t_EnvInfo->SetReg_RmKey(szHashed);

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoHostRmInfo::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
