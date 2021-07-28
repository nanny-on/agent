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
#include "LogicMgrEnvLicense.h"

//---------------------------------------------------------------------------

CLogicMgrEnvLicense*		t_LogicMgrEnvLicense = NULL;

//---------------------------------------------------------------------------

CLogicMgrEnvLicense::CLogicMgrEnvLicense()
{
	t_ManageEnvLicense	= new CManageEnvLicense();
	t_ManageEnvLicense->LoadDBMS();
	t_ManageEnvLicense->InitHash();

	m_strLogicName		= "mgr env license";

	m_nControlType		= SS_CONTROL_TYPE_ENV_LICENSE;	
	m_nEvtObjType		= EVENT_OBJECT_TYPE_ENV;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_ENV_LICENSE;
}
//---------------------------------------------------------------------------

CLogicMgrEnvLicense::~CLogicMgrEnvLicense()
{
	SAFE_DELETE(t_ManageEnvLicense);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLicense::AnalyzePkt_FromMgr_Ext()
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

INT32		CLogicMgrEnvLicense::AnalyzePkt_FromMgr_Edit_Ext()
{
	DB_ENV_LICENSE del;

	if( t_ManageEnvLicense->GetPkt(RecvToken, del))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	
	{
		m_tChgPkgMap.clear();

		UINT64 nPkgRight = 0;
		nPkgRight = t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0);
		nPkgRight += t_ManageEnvLicense->IsValidRight(&del, SS_PACKAGE_TYPE_NPMS, 0, 0);
		m_tChgPkgMap[SS_PACKAGE_TYPE_NPMS] = nPkgRight;

		nPkgRight = t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0);
		nPkgRight += t_ManageEnvLicense->IsValidRight(&del, SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0);
		m_tChgPkgMap[SS_PACKAGE_TYPE_NOMS] = nPkgRight;
	}
	
	if(t_ManageEnvLicense->ApplyEnvLicense(del))
	{
		SetDLEH_EC(g_nErrRtn);
		WriteLogE("[%s] apply env information : [%d]", m_strLogicName.c_str(), g_nErrRtn);			
		return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
	}

	t_ManageEnvLicense->InitHash();

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLicense::ApplyPolicy()
{	
	if(IsChangePackage(SS_PACKAGE_TYPE_NPMS))
	{
		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
		{
			do 
			{
				if(t_LogicMgrPoPmOp->CheckRunEnv())			break;			
				if(t_LogicMgrPtnPatch->InitPtnPatch())		break;

				t_LogicMgrHostPatch->ScanPatchList();		
			} while (FALSE);		

			{
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_SCAN);	
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_ROLLBACK);	
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_MON_RM_PATCH);	
			}
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_APPLY_PM_SCAN);	
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_APPLY_PM_ROLLBACK);	
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_APPLY_PM_MON_RM_PATCH);	
		}
	}

	if(IsChangePackage(SS_PACKAGE_TYPE_NOMS))
	{
		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
		{
			do 
			{
				if(t_LogicMgrPoInVulnOp->CheckRunEnv())		break;			
				if(t_LogicMgrPtnVuln->InitPtnVuln())		break;

			} while (FALSE);	
			
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_READY_IN_DEPLOY);
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_VULN_SCAN);	
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_READY_IN_DEPLOY);	
			t_ThreadTimer->t_TimerUtil.DisableTimer(TIMER_ID_POLICY_APPLY_IN_VULN_SCAN);	
		}
	}
	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLicense::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicMgrEnvLicense::IsChangePackage(UINT64 nPkgRight)
{
	TMapID64Itor find = m_tChgPkgMap.find(nPkgRight);
	if(find == m_tChgPkgMap.end())	return 0;
	
	if(find->second != 1)		return 0;
	return 1;
}
//---------------------------------------------------------------------------
