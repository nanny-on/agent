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
#include "DeployPolicyUtil.h"

//---------------------------------------------------------------------------

CDeployPolicyUtil*		t_DeployPolicyUtil;

//---------------------------------------------------------------------------

CDeployPolicyUtil::CDeployPolicyUtil()
{
}
//---------------------------------------------------------------------------

CDeployPolicyUtil::~CDeployPolicyUtil()
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CDeployPolicyUtil::SetPolicy(DB_POLICY& dp, UINT32 nPkgID, UINT64 nPolicyType)
{
	switch(nPolicyType)
    {
		case SS_POLICY_TYPE_UPDATE:				dp.nPUpdateID = nPkgID;				break;	//01
		case SS_POLICY_TYPE_SVR_INFO_LGN:		dp.nPSvrInfoLgnID = nPkgID;			break;	//02
		case SS_POLICY_TYPE_HOST_RM_INFO:		dp.nPHostRmInfoID = nPkgID;			break;	//03
		case SS_POLICY_TYPE_HOST_RUN:			dp.nPHostRunID = nPkgID;			break;	//04
		case SS_POLICY_TYPE_HOST_CLEAR:			dp.nPHostClearID = nPkgID;			break;	//05
		case SS_POLICY_TYPE_POWER:				dp.nPPowerID = nPkgID;				break;	//06
		case SS_POLICY_TYPE_CTL_PANEL:			dp.nPCtlPanelID = nPkgID;			break;	//07
		case SS_POLICY_TYPE_CTL_PROC:			dp.nPCtlProcID = nPkgID;			break;	//08
		case SS_POLICY_TYPE_SELF_PROTECT:		dp.nPSelfProtectID = nPkgID;		break;	//09
		case SS_POLICY_TYPE_DEPLOY_FILE:		dp.nPDeployFileID = nPkgID;			break;	//10

		case SS_POLICY_TYPE_FA_CLEAR:			dp.nPFaClearID = nPkgID;			break;	//11
		case SS_POLICY_TYPE_FA_PROC:			dp.nPFaProcID = nPkgID;				break;	//12
		case SS_POLICY_TYPE_FA_ENV:				dp.nPFaEnvID = nPkgID;				break;	//13
		case SS_POLICY_TYPE_FA_OP:				dp.nPFaOpID = nPkgID;				break;	//14
		case SS_POLICY_TYPE_FA_BK:				dp.nPFaBkID = nPkgID;				break;	//15

		case SS_POLICY_TYPE_WIN_RUN:			dp.nPWinRunID = nPkgID;				break;	//16
		case SS_POLICY_TYPE_SVR_INFO_UDT:		dp.nPSvrInfoUdtID = nPkgID;			break;	//17
		case SS_POLICY_TYPE_SVR_INFO_MGR:		dp.nPSvrInfoMgrID = nPkgID;			break;	//18

		case SS_POLICY_TYPE_FE_PTN_OP:			dp.nPFePtnOpID = nPkgID;			break;	//19
		case SS_POLICY_TYPE_FE_PTN_BL:			dp.nPFePtnBLID = nPkgID;			break;	//20
		case SS_POLICY_TYPE_FE_PTN_WL:			dp.nPFePtnWLID = nPkgID;			break;	//21
		case SS_POLICY_TYPE_FE_EXCLUDE:			dp.nPFeExcludeID = nPkgID;			break;	//22
		case SS_POLICY_TYPE_FE_NOTIFY:			dp.nPFeNotifyID = nPkgID;			break;	//23
		case SS_POLICY_TYPE_FE_SINGLE_PTN:		dp.nPFeSinglePtnID = nPkgID;		break;	//24
		case SS_POLICY_TYPE_FE_PTN_LO:			dp.nPFePtnLoID = nPkgID;			break;	//25

		case SS_POLICY_TYPE_FA_NOTIFY:			dp.nPFaNotifyID = nPkgID;			break;	//26
		case SS_POLICY_TYPE_SCH:				dp.nPSchID = nPkgID;				break;	//29
		
		case SS_POLICY_TYPE_IN_PTN_OP:			dp.nPInPtnOpID	= nPkgID;			break;	//33
 		case SS_POLICY_TYPE_IN_PTN_BL:			dp.nPInPtnBLID	= nPkgID;			break;	//34
 		case SS_POLICY_TYPE_IN_PTN_WL:			dp.nPInPtnWLID	= nPkgID;			break;	//35
 		case SS_POLICY_TYPE_IN_PTN_EX:			dp.nPInPtnExID	= nPkgID;			break;	//36
 		case SS_POLICY_TYPE_IN_PTN_SP:			dp.nPInPtnSPID	= nPkgID;			break;	//37
		case SS_POLICY_TYPE_IN_PTN_NO:			dp.nPInPtnNoID	= nPkgID;			break;	//38

		case SS_POLICY_TYPE_IN_VULN_OP:			dp.nPInVulnOpID	= nPkgID;			break;	//44
		case SS_POLICY_TYPE_IN_VULN_SCAN:		dp.nPInVulnScanID = nPkgID;			break;	//46

		case SS_POLICY_TYPE_HOST_NOTIFY:		dp.nPHostNotifyID = nPkgID;			break;

		case SS_POLICY_TYPE_PM_OP:				dp.nPPmOpID	= nPkgID;				break;
		case SS_POLICY_TYPE_PM_DM:				dp.nPPmDmID	= nPkgID;				break;
		case SS_POLICY_TYPE_PM_SCAN:			dp.nPPmScanID = nPkgID;				break;
		case SS_POLICY_TYPE_PM_EX:				dp.nPPmExID	= nPkgID;				break;
		case SS_POLICY_TYPE_PM_NO:				dp.nPPmNoID	= nPkgID;				break;

		case SS_POLICY_TYPE_IN_VULN_AX:			dp.nPInVulnAxID	= nPkgID;			break;	//42
		case SS_POLICY_TYPE_IN_VULN_EDIT_APP:	dp.nPInVulnEditAppID= nPkgID;		break;	//43
		case SS_POLICY_TYPE_IN_VULN_QNA:		dp.nPInVulnQnaID= nPkgID;			break;	//45
		case SS_POLICY_TYPE_IN_VULN_SECU_USB:	dp.nPInVulnSecuUsbID= nPkgID;		break;	//47
		case SS_POLICY_TYPE_IN_VULN_SW:			dp.nPInVulnSwID	= nPkgID;			break;	//48
		case SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION:	dp.nPInVulnPatchExceptionID= nPkgID;		break;	//49

		case SS_POLICY_TYPE_DV_OP:				dp.nPInDevOOpID	= nPkgID;			break;
		case SS_POLICY_TYPE_DV_BL:				dp.nPInDevOBLID	= nPkgID;			break;
		case SS_POLICY_TYPE_DV_WL:				dp.nPInDevOWLID	= nPkgID;			break;
		case SS_POLICY_TYPE_DV_EX:				dp.nPInDevOExID	= nPkgID;			break;
		case SS_POLICY_TYPE_DV_NOTIFY:			dp.nPInDevONotifyID	= nPkgID;		break;

		case SS_POLICY_TYPE_IN_RS_OP:			dp.nPInRsOpID	= nPkgID;			break;	//39
		case SS_POLICY_TYPE_IN_RS_BK:			dp.nPInRsBkID	= nPkgID;			break;	//40
		case SS_POLICY_TYPE_IN_RS_NO:			dp.nPInRsNoID	= nPkgID;			break;	//41

		case SS_POLICY_TYPE_NC_PTN_OP:			dp.nPNcPtnOpID	= nPkgID;			break;	//33
		case SS_POLICY_TYPE_NC_PTN_LO:			dp.nPNcPtnLoID	= nPkgID;			break;	//33
		case SS_POLICY_TYPE_NC_PTN_BL:			dp.nPNcPtnBLID	= nPkgID;			break;	//34
		case SS_POLICY_TYPE_NC_PTN_WL:			dp.nPNcPtnWLID	= nPkgID;			break;	//35
		case SS_POLICY_TYPE_NC_PTN_EX:			dp.nPNcPtnExID	= nPkgID;			break;	//36
		case SS_POLICY_TYPE_NC_PTN_SP:			dp.nPNcPtnSPID	= nPkgID;			break;	//37
		case SS_POLICY_TYPE_NC_PTN_NO:			dp.nPNcPtnNoID	= nPkgID;			break;	//38

		case SS_POLICY_TYPE_HOST_WEB_SOCK:		dp.nPHostWebSockID	= nPkgID;		break;	//38
	}
    return 0;
}
//---------------------------------------------------------------------------

UINT32		CDeployPolicyUtil::GetPolicy(DB_POLICY& dp, UINT64 nPolicyType)
{
	UINT32 nPkgID = 0;
	switch(nPolicyType)
	{
		case SS_POLICY_TYPE_UPDATE:				nPkgID	= dp.nPUpdateID;			break;	//01
		case SS_POLICY_TYPE_SVR_INFO_LGN:		nPkgID	= dp.nPSvrInfoLgnID;		break;	//02
		case SS_POLICY_TYPE_HOST_RM_INFO:		nPkgID  = dp.nPHostRmInfoID;		break;	//03
		case SS_POLICY_TYPE_HOST_RUN:			nPkgID  = dp.nPHostRunID;			break;	//04
		case SS_POLICY_TYPE_HOST_CLEAR:			nPkgID  = dp.nPHostClearID;			break;	//05
		case SS_POLICY_TYPE_POWER:				nPkgID  = dp.nPPowerID;				break;	//06
		case SS_POLICY_TYPE_CTL_PANEL:			nPkgID  = dp.nPCtlPanelID;			break;	//07
		case SS_POLICY_TYPE_CTL_PROC:			nPkgID  = dp.nPCtlProcID;			break;	//08
		case SS_POLICY_TYPE_SELF_PROTECT:		nPkgID  = dp.nPSelfProtectID;		break;	//09
		case SS_POLICY_TYPE_DEPLOY_FILE:		nPkgID  = dp.nPDeployFileID;		break;	//10

		case SS_POLICY_TYPE_FA_CLEAR:			nPkgID  = dp.nPFaClearID;			break;	//11
		case SS_POLICY_TYPE_FA_PROC:			nPkgID  = dp.nPFaProcID;			break;	//12
		case SS_POLICY_TYPE_FA_ENV:				nPkgID  = dp.nPFaEnvID;				break;	//13
		case SS_POLICY_TYPE_FA_OP:				nPkgID  = dp.nPFaOpID;				break;	//14
		case SS_POLICY_TYPE_FA_BK:				nPkgID  = dp.nPFaBkID;				break;	//15

		case SS_POLICY_TYPE_WIN_RUN:			nPkgID  = dp.nPWinRunID;			break;	//16
		case SS_POLICY_TYPE_SVR_INFO_UDT:		nPkgID  = dp.nPSvrInfoUdtID;		break;	//17
		case SS_POLICY_TYPE_SVR_INFO_MGR:		nPkgID	= dp.nPSvrInfoMgrID;		break;	//18

		case SS_POLICY_TYPE_FE_PTN_OP:			nPkgID	= dp.nPFePtnOpID;			break;	//19
		case SS_POLICY_TYPE_FE_PTN_BL:			nPkgID	= dp.nPFePtnBLID;			break;	//20
		case SS_POLICY_TYPE_FE_PTN_WL:			nPkgID	= dp.nPFePtnWLID;			break;	//21
		case SS_POLICY_TYPE_FE_EXCLUDE:			nPkgID	= dp.nPFeExcludeID;			break;	//22
		case SS_POLICY_TYPE_FE_NOTIFY:			nPkgID	= dp.nPFeNotifyID;			break;	//23
		case SS_POLICY_TYPE_FE_SINGLE_PTN:		nPkgID	= dp.nPFeSinglePtnID;		break;	//24
		case SS_POLICY_TYPE_FE_PTN_LO:			nPkgID	= dp.nPFePtnLoID;			break;	//25

		case SS_POLICY_TYPE_FA_NOTIFY:			nPkgID	= dp.nPFaNotifyID;			break;	//26

		case SS_POLICY_TYPE_SCH:				nPkgID	= dp.nPSchID;				break;	//32

		case SS_POLICY_TYPE_IN_PTN_OP:			nPkgID	= dp.nPInPtnOpID;			break;	//33
 		case SS_POLICY_TYPE_IN_PTN_BL:			nPkgID	= dp.nPInPtnBLID;			break;	//34
 		case SS_POLICY_TYPE_IN_PTN_WL:			nPkgID	= dp.nPInPtnWLID;			break;	//35
		case SS_POLICY_TYPE_IN_PTN_EX:			nPkgID	= dp.nPInPtnExID;			break;	//36
		case SS_POLICY_TYPE_IN_PTN_SP:			nPkgID	= dp.nPInPtnSPID;			break;	//37
		case SS_POLICY_TYPE_IN_PTN_NO:			nPkgID	= dp.nPInPtnNoID;			break;	//38

		case SS_POLICY_TYPE_IN_VULN_OP:			nPkgID	= dp.nPInVulnOpID;			break;	//44
		case SS_POLICY_TYPE_IN_VULN_SCAN:		nPkgID	= dp.nPInVulnScanID;		break;	//46

		case SS_POLICY_TYPE_HOST_NOTIFY:		nPkgID	= dp.nPHostNotifyID;		break;

		case SS_POLICY_TYPE_PM_OP:				nPkgID	= dp.nPPmOpID;				break;	
		case SS_POLICY_TYPE_PM_DM:				nPkgID	= dp.nPPmDmID;				break;	
		case SS_POLICY_TYPE_PM_SCAN:			nPkgID	= dp.nPPmScanID;			break;	
		case SS_POLICY_TYPE_PM_EX:				nPkgID	= dp.nPPmExID;				break;	
		case SS_POLICY_TYPE_PM_NO:				nPkgID	= dp.nPPmNoID;				break;	

		case SS_POLICY_TYPE_IN_VULN_AX:			nPkgID	= dp.nPInVulnAxID;			break;	//42
		case SS_POLICY_TYPE_IN_VULN_EDIT_APP:	nPkgID	= dp.nPInVulnEditAppID;		break;	//43
		case SS_POLICY_TYPE_IN_VULN_QNA:		nPkgID	= dp.nPInVulnQnaID;			break;	//45
		case SS_POLICY_TYPE_IN_VULN_SECU_USB:	nPkgID	= dp.nPInVulnSecuUsbID;		break;	//47
		case SS_POLICY_TYPE_IN_VULN_SW:			nPkgID	= dp.nPInVulnSwID;			break;	//48
		case SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION:	nPkgID	= dp.nPInVulnPatchExceptionID;	break;	//49

		case SS_POLICY_TYPE_DV_OP:				nPkgID	= dp.nPInDevOOpID;			break;
		case SS_POLICY_TYPE_DV_BL:				nPkgID	= dp.nPInDevOBLID;			break;
		case SS_POLICY_TYPE_DV_WL:				nPkgID	= dp.nPInDevOWLID;			break;
		case SS_POLICY_TYPE_DV_EX:				nPkgID	= dp.nPInDevOExID;			break;
		case SS_POLICY_TYPE_DV_NOTIFY:			nPkgID	= dp.nPInDevONotifyID;		break;

		case SS_POLICY_TYPE_IN_RS_OP:			nPkgID	= dp.nPInRsOpID;			break;	//39
		case SS_POLICY_TYPE_IN_RS_BK:			nPkgID	= dp.nPInRsBkID;			break;	//40
		case SS_POLICY_TYPE_IN_RS_NO:			nPkgID	= dp.nPInRsNoID;			break;	//41

		case SS_POLICY_TYPE_NC_PTN_OP:			nPkgID	= dp.nPNcPtnOpID;			break;	//33
		case SS_POLICY_TYPE_NC_PTN_LO:			nPkgID	= dp.nPNcPtnLoID;			break;	//34
		case SS_POLICY_TYPE_NC_PTN_BL:			nPkgID	= dp.nPNcPtnBLID;			break;	//34
		case SS_POLICY_TYPE_NC_PTN_WL:			nPkgID	= dp.nPNcPtnWLID;			break;	//35
		case SS_POLICY_TYPE_NC_PTN_EX:			nPkgID	= dp.nPNcPtnExID;			break;	//36
		case SS_POLICY_TYPE_NC_PTN_SP:			nPkgID	= dp.nPNcPtnSPID;			break;	//37
		case SS_POLICY_TYPE_NC_PTN_NO:			nPkgID	= dp.nPNcPtnNoID;			break;	//38

		case SS_POLICY_TYPE_HOST_WEB_SOCK:		nPkgID	= dp.nPHostWebSockID;		break;	//38


	}

	return nPkgID;
}
//---------------------------------------------------------------------------

UINT32		CDeployPolicyUtil::GetCurPoID(UINT64 nPolicyType)
{
	UINT32 nPkgID = 0;
	UINT32 nPolicyID = 0;

	{
// 		PDB_USER pDU = t_ManageUser->FindStrKeyItem(t_EnvInfoOp->GetUserUniCode());
// 		if(pDU)
// 		{
// 			PDB_USER_POLICY pDUP = t_ManageUserPolicy->FindKeyItem(pDU->nID);
// 			if(pDUP)
// 			{
// 				nPkgID = GetPolicy(pDUP->tDP, nPolicyType);
// 			}
// 			if(nPkgID)	return nPkgID;
// 		}
	}

	{
		PDB_HOST_POLICY pdhp = t_ManageHostPolicy->FindItem(DEFAULT_ID);
		if(pdhp)
		{
			nPkgID = GetPolicy(pdhp->tDP, nPolicyType);
		}

		if(nPkgID)	return nPkgID;
	}
	return 0;
}
//---------------------------------------------------------------------------

PVOID		CDeployPolicyUtil::GetCurPoPtr(UINT64 nPolicyType)
{   
	UINT32 nPoID = GetCurPoID(nPolicyType);
	if(!nPoID)	return NULL;

	return GetPoPtr(nPoID, nPolicyType);
}
//---------------------------------------------------------------------------

PVOID		CDeployPolicyUtil::GetPoPtr(UINT32 nID, UINT64 nPolicyType)
{
	PVOID	pPolicy = NULL;
	switch(nPolicyType)
	{
		case SS_POLICY_TYPE_HOST_RM_INFO:	pPolicy = t_ManagePoHostRmInfo->FindItem(nID);		break;
		case SS_POLICY_TYPE_SVR_INFO_LGN:	pPolicy = t_ManagePoSvrInfoLgn->FindItem(nID);		break;
		case SS_POLICY_TYPE_HOST_RUN:		pPolicy = t_ManagePoHostRun->FindItem(nID);			break;
		case SS_POLICY_TYPE_POWER:			pPolicy = t_ManagePoPower->FindItem(nID);			break;
		case SS_POLICY_TYPE_CTL_PANEL:		pPolicy = t_ManagePoCtlPanel->FindItem(nID);		break;
		case SS_POLICY_TYPE_CTL_PROC:		pPolicy = t_ManagePoCtlProc->FindItem(nID);			break;
		case SS_POLICY_TYPE_SELF_PROTECT:	pPolicy = t_ManagePoSelfProtect->FindItem(nID);		break;
		case SS_POLICY_TYPE_DEPLOY_FILE:	pPolicy = t_ManagePoDeployFile->FindItem(nID);		break;

		case SS_POLICY_TYPE_FA_CLEAR:		pPolicy = t_ManagePoFaClear->FindItem(nID);			break;	//11
		case SS_POLICY_TYPE_FA_PROC:		pPolicy = t_ManagePoFaProc->FindItem(nID);			break;	//12
		case SS_POLICY_TYPE_FA_ENV:			pPolicy = t_ManagePoFaEnv->FindItem(nID);			break;	//13
		case SS_POLICY_TYPE_FA_OP:			pPolicy = t_ManagePoFaOp->FindItem(nID);			break;	//14
		case SS_POLICY_TYPE_FA_BK:			pPolicy = t_ManagePoFaBk->FindItem(nID);			break;	//15	
		
		case SS_POLICY_TYPE_WIN_RUN:		pPolicy = t_ManagePoWinRun->FindItem(nID);			break;
		case SS_POLICY_TYPE_SVR_INFO_UDT:	pPolicy = t_ManagePoSvrInfoUdt->FindItem(nID);		break;	//17

		case SS_POLICY_TYPE_FE_PTN_OP:		pPolicy	= t_ManagePoFePtnOp->FindItem(nID);			break;
		case SS_POLICY_TYPE_FE_PTN_BL:		pPolicy	= t_ManagePoFePtnBL->FindItem(nID);			break;
		case SS_POLICY_TYPE_FE_PTN_WL:		pPolicy	= t_ManagePoFePtnWL->FindItem(nID);			break;
		case SS_POLICY_TYPE_FE_EXCLUDE:		pPolicy	= t_ManagePoFeExclude->FindItem(nID);		break;
		case SS_POLICY_TYPE_FE_NOTIFY:		pPolicy	= t_ManagePoFeNotify->FindItem(nID);		break;	
		case SS_POLICY_TYPE_FE_SINGLE_PTN:	pPolicy	= t_ManagePoFeSinglePtn->FindItem(nID);		break;	//24
		case SS_POLICY_TYPE_FE_PTN_LO:		pPolicy	= t_ManagePoFePtnLo->FindItem(nID);			break;
		
		case SS_POLICY_TYPE_FA_NOTIFY:				pPolicy	= t_ManagePoFaNotify->FindItem(nID);		break;	//26
		case SS_POLICY_TYPE_SCH:					pPolicy = t_ManagePoSch->FindItem(nID);				break;	//32

		case SS_POLICY_TYPE_IN_PTN_OP:				pPolicy	= t_ManagePoInPtnOp->FindItem(nID);			break;	//32
		case SS_POLICY_TYPE_IN_PTN_BL:				pPolicy	= t_ManagePoInPtnBL->FindItem(nID);			break;	//33
		case SS_POLICY_TYPE_IN_PTN_WL:				pPolicy	= t_ManagePoInPtnWL->FindItem(nID);			break;	//34
		case SS_POLICY_TYPE_IN_PTN_EX:				pPolicy	= t_ManagePoInPtnEx->FindItem(nID);			break;	//35
		case SS_POLICY_TYPE_IN_PTN_SP:				pPolicy	= t_ManagePoInPtnSP->FindItem(nID);			break;	//36
		case SS_POLICY_TYPE_IN_PTN_NO:				pPolicy	= t_ManagePoInPtnNo->FindItem(nID);			break;	//37

		case SS_POLICY_TYPE_IN_VULN_OP:				pPolicy	= t_ManagePoInVulnOp->FindItem(nID);		break;	//40
		case SS_POLICY_TYPE_IN_VULN_SCAN:			pPolicy	= t_ManagePoInVulnScan->FindItem(nID);		break;	//42

		case SS_POLICY_TYPE_HOST_NOTIFY:			pPolicy = t_ManagePoHostNotify->FindItem(nID);		break;

		case SS_POLICY_TYPE_PM_OP:					pPolicy	= t_ManagePoPmOp->FindItem(nID);			break;
		case SS_POLICY_TYPE_PM_DM:					pPolicy	= t_ManagePoPmDm->FindItem(nID);			break;
		case SS_POLICY_TYPE_PM_SCAN:				pPolicy	= t_ManagePoPmScan->FindItem(nID);			break;
		case SS_POLICY_TYPE_PM_EX:					pPolicy	= t_ManagePoPmEx->FindItem(nID);			break;
		case SS_POLICY_TYPE_PM_NO:					pPolicy	= t_ManagePoPmNo->FindItem(nID);			break;

		case SS_POLICY_TYPE_IN_VULN_AX:				pPolicy	= t_ManagePoInVulnAx->FindItem(nID);		break;	//38
		case SS_POLICY_TYPE_IN_VULN_EDIT_APP:		pPolicy	= t_ManagePoInVulnEditApp->FindItem(nID);	break;	//39
		case SS_POLICY_TYPE_IN_VULN_QNA:			pPolicy	= t_ManagePoInVulnQna->FindItem(nID);		break;	//41
		case SS_POLICY_TYPE_IN_VULN_SECU_USB:		pPolicy	= t_ManagePoInVulnSecuUsb->FindItem(nID);	break;	//43
		case SS_POLICY_TYPE_IN_VULN_SW:				pPolicy	= t_ManagePoInVulnSw->FindItem(nID);		break;	//44
		case SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION:		pPolicy	= t_ManagePoInVulnPatchException->FindItem(nID);	break;	//45

		case SS_POLICY_TYPE_DV_OP:					pPolicy = t_ManagePoInDevOOp->FindItem(nID);		break;
		case SS_POLICY_TYPE_DV_BL:					pPolicy = t_ManagePoInDevOBL->FindItem(nID);		break;
		case SS_POLICY_TYPE_DV_WL:					pPolicy = t_ManagePoInDevOWL->FindItem(nID);		break;
		case SS_POLICY_TYPE_DV_EX:					pPolicy = t_ManagePoInDevOEx->FindItem(nID);		break;
		case SS_POLICY_TYPE_DV_NOTIFY:				pPolicy = t_ManagePoInDevONotify->FindItem(nID);	break;

		case SS_POLICY_TYPE_IN_RS_OP:				pPolicy	= t_ManagePoInRsOp->FindItem(nID);			break;	//38
 		case SS_POLICY_TYPE_IN_RS_BK:				pPolicy	= t_ManagePoInRsBk->FindItem(nID);			break;	//39
		case SS_POLICY_TYPE_IN_RS_NO:				pPolicy	= t_ManagePoInRsNo->FindItem(nID);			break;	//40

		case SS_POLICY_TYPE_NC_PTN_OP:				pPolicy	= t_ManagePoNcPtnOp->FindItem(nID);			break;	//32
		case SS_POLICY_TYPE_NC_PTN_LO:				pPolicy	= t_ManagePoNcPtnLo->FindItem(nID);			break;	//33
		case SS_POLICY_TYPE_NC_PTN_BL:				pPolicy	= t_ManagePoNcPtnBL->FindItem(nID);			break;	//33
		case SS_POLICY_TYPE_NC_PTN_WL:				pPolicy	= t_ManagePoNcPtnWL->FindItem(nID);			break;	//34
		case SS_POLICY_TYPE_NC_PTN_EX:				pPolicy	= t_ManagePoNcPtnEx->FindItem(nID);			break;	//35
		case SS_POLICY_TYPE_NC_PTN_SP:				pPolicy	= t_ManagePoNcPtnSP->FindItem(nID);			break;	//36
		case SS_POLICY_TYPE_NC_PTN_NO:				pPolicy	= t_ManagePoNcPtnNo->FindItem(nID);			break;	//37

		case SS_POLICY_TYPE_HOST_WEB_SOCK:			pPolicy	= t_ManagePoHostWebSock->FindItem(nID);		break;	//37

		default:
		{
			WriteLogE("[po ptr] not find policy type %016X", nPolicyType);
			break;
		}
	}
	return pPolicy;
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
UINT64			CDeployPolicyUtil::GetPoTypeFromPoIdx(UINT32 nPoIdx)
{
	UINT64 nPoType = 0;
	switch(nPoIdx)
	{
		case SS_POLICY_INDEX_UPDATE:					nPoType = SS_POLICY_TYPE_UPDATE;					break;
		case SS_POLICY_INDEX_SVR_INFO_LGN:				nPoType = SS_POLICY_TYPE_SVR_INFO_LGN;				break;
		case SS_POLICY_INDEX_HOST_RM_INFO:				nPoType = SS_POLICY_TYPE_HOST_RM_INFO;				break;
		case SS_POLICY_INDEX_HOST_RUN:					nPoType = SS_POLICY_TYPE_HOST_RUN;					break;
		case SS_POLICY_INDEX_HOST_CLEAR:				nPoType = SS_POLICY_TYPE_HOST_CLEAR;				break;
		case SS_POLICY_INDEX_POWER:						nPoType = SS_POLICY_TYPE_POWER;						break;
		case SS_POLICY_INDEX_CTL_PANEL:					nPoType = SS_POLICY_TYPE_CTL_PANEL;					break;
		case SS_POLICY_INDEX_CTL_PROC:					nPoType = SS_POLICY_TYPE_CTL_PROC;					break;
		case SS_POLICY_INDEX_SELF_PROTECT:				nPoType = SS_POLICY_TYPE_SELF_PROTECT;				break;
		case SS_POLICY_INDEX_DEPLOY_FILE:				nPoType = SS_POLICY_TYPE_DEPLOY_FILE;				break;

		case SS_POLICY_INDEX_FA_OP:						nPoType  = SS_POLICY_TYPE_FA_OP;					break;	//10
		case SS_POLICY_INDEX_FA_CLEAR:					nPoType  = SS_POLICY_TYPE_FA_CLEAR;					break;	//11
		case SS_POLICY_INDEX_FA_PROC:					nPoType  = SS_POLICY_TYPE_FA_PROC;					break;	//12
		case SS_POLICY_INDEX_FA_ENV:					nPoType  = SS_POLICY_TYPE_FA_ENV;					break;	//13
		case SS_POLICY_INDEX_FA_BK:						nPoType  = SS_POLICY_TYPE_FA_BK;					break;	//14

		case SS_POLICY_INDEX_WIN_RUN:					nPoType = SS_POLICY_TYPE_WIN_RUN;					break;	//15
		case SS_POLICY_INDEX_SVR_INFO_UDT:				nPoType = SS_POLICY_TYPE_SVR_INFO_UDT;				break;	//16
		case SS_POLICY_INDEX_SVR_INFO_MGR:				nPoType = SS_POLICY_TYPE_SVR_INFO_MGR;				break;	//17

		case SS_POLICY_INDEX_FE_PTN_OP:					nPoType = SS_POLICY_TYPE_FE_PTN_OP;					break;	//18
		case SS_POLICY_INDEX_FE_PTN_BL:					nPoType = SS_POLICY_TYPE_FE_PTN_BL;					break;	//19
		case SS_POLICY_INDEX_FE_PTN_WL:					nPoType = SS_POLICY_TYPE_FE_PTN_WL;					break;	//20
		case SS_POLICY_INDEX_FE_EXCLUDE:				nPoType = SS_POLICY_TYPE_FE_EXCLUDE;				break;	//21
		case SS_POLICY_INDEX_FE_NOTIFY:					nPoType = SS_POLICY_TYPE_FE_NOTIFY;					break;	//22
		case SS_POLICY_INDEX_FE_SINGLE_PTN:				nPoType = SS_POLICY_TYPE_FE_SINGLE_PTN;				break;	//23
		case SS_POLICY_INDEX_FE_PTN_LO:					nPoType = SS_POLICY_TYPE_FE_PTN_LO;					break;	//24

		case SS_POLICY_INDEX_FA_NOTIFY:					nPoType = SS_POLICY_TYPE_FA_NOTIFY;					break;	//25
		case SS_POLICY_INDEX_SCH:						nPoType = SS_POLICY_TYPE_SCH;						break;	//26

		case SS_POLICY_INDEX_IN_PTN_OP:					nPoType = SS_POLICY_TYPE_IN_PTN_OP;					break;	//28
		case SS_POLICY_INDEX_IN_PTN_BL:					nPoType = SS_POLICY_TYPE_IN_PTN_BL;					break;	//29
		case SS_POLICY_INDEX_IN_PTN_WL:					nPoType = SS_POLICY_TYPE_IN_PTN_WL;					break;	//30
		case SS_POLICY_INDEX_IN_PTN_EX:					nPoType = SS_POLICY_TYPE_IN_PTN_EX;					break;	//31
		case SS_POLICY_INDEX_IN_PTN_SP:					nPoType = SS_POLICY_TYPE_IN_PTN_SP;					break;	//32
		case SS_POLICY_INDEX_IN_PTN_NO:					nPoType = SS_POLICY_TYPE_IN_PTN_NO;					break;	//33

		case SS_POLICY_INDEX_IN_VULN_OP:				nPoType = SS_POLICY_TYPE_IN_VULN_OP;				break;	//39
		case SS_POLICY_INDEX_IN_VULN_SCAN:				nPoType = SS_POLICY_TYPE_IN_VULN_SCAN;				break;	//41

		case SS_POLICY_INDEX_HOST_NOTIFY:				nPoType = SS_POLICY_TYPE_HOST_NOTIFY;				break;

		case SS_POLICY_INDEX_PM_OP:						nPoType = SS_POLICY_TYPE_PM_OP;						break;	//41
		case SS_POLICY_INDEX_PM_DM:						nPoType = SS_POLICY_TYPE_PM_DM;						break;	//42
		case SS_POLICY_INDEX_PM_SCAN:					nPoType = SS_POLICY_TYPE_PM_SCAN;					break;	//43
		case SS_POLICY_INDEX_PM_EX:						nPoType = SS_POLICY_TYPE_PM_EX;						break;	//44
		case SS_POLICY_INDEX_PM_NO:						nPoType = SS_POLICY_TYPE_PM_NO;						break;	//44

		case SS_POLICY_INDEX_IN_VULN_AX:				nPoType = SS_POLICY_TYPE_IN_VULN_AX;				break;	//37
		case SS_POLICY_INDEX_IN_VULN_EDIT_APP:			nPoType = SS_POLICY_TYPE_IN_VULN_EDIT_APP;			break;	//38
		case SS_POLICY_INDEX_IN_VULN_QNA:				nPoType = SS_POLICY_TYPE_IN_VULN_QNA;				break;	//40
		case SS_POLICY_INDEX_IN_VULN_SECU_USB:			nPoType = SS_POLICY_TYPE_IN_VULN_SECU_USB;			break;	//42
		case SS_POLICY_INDEX_IN_VULN_SW:				nPoType = SS_POLICY_TYPE_IN_VULN_SW;				break;	//43
		case SS_POLICY_INDEX_IN_VULN_PATCH_EXCEPTION:	nPoType = SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION;	break;	//44

		case SS_POLICY_INDEX_DV_OP:						nPoType = SS_POLICY_TYPE_DV_OP;						break;
		case SS_POLICY_INDEX_DV_BL:						nPoType = SS_POLICY_TYPE_DV_BL;						break;
		case SS_POLICY_INDEX_DV_WL:						nPoType = SS_POLICY_TYPE_DV_WL;						break;
		case SS_POLICY_INDEX_DV_EX:						nPoType = SS_POLICY_TYPE_DV_EX;						break;
		case SS_POLICY_INDEX_DV_NOTIFY:					nPoType = SS_POLICY_TYPE_DV_NOTIFY;					break;

		case SS_POLICY_INDEX_IN_RS_OP:					nPoType = SS_POLICY_TYPE_IN_RS_OP;					break;	//28
		case SS_POLICY_INDEX_IN_RS_NO:					nPoType = SS_POLICY_TYPE_IN_RS_NO;					break;	//29
		case SS_POLICY_INDEX_IN_RS_BK:					nPoType = SS_POLICY_TYPE_IN_RS_BK;					break;	//30

		case SS_POLICY_INDEX_NC_PTN_OP:					nPoType = SS_POLICY_TYPE_NC_PTN_OP;					break;	//28
		case SS_POLICY_INDEX_NC_PTN_LO:					nPoType = SS_POLICY_TYPE_NC_PTN_LO;					break;	//28
		case SS_POLICY_INDEX_NC_PTN_BL:					nPoType = SS_POLICY_TYPE_NC_PTN_BL;					break;	//29
		case SS_POLICY_INDEX_NC_PTN_WL:					nPoType = SS_POLICY_TYPE_NC_PTN_WL;					break;	//30
		case SS_POLICY_INDEX_NC_PTN_EX:					nPoType = SS_POLICY_TYPE_NC_PTN_EX;					break;	//31
		case SS_POLICY_INDEX_NC_PTN_SP:					nPoType = SS_POLICY_TYPE_NC_PTN_SP;					break;	//32
		case SS_POLICY_INDEX_NC_PTN_NO:					nPoType = SS_POLICY_TYPE_NC_PTN_NO;					break;	//33

		case SS_POLICY_INDEX_HOST_WEB_SOCK:				nPoType = SS_POLICY_TYPE_HOST_WEB_SOCK;				break;	//33


		default:
		{
			WriteLogE("[get po type from po idx] not find policy type %u", nPoIdx);
			break;
		}
	}
	return nPoType;
}
//---------------------------------------------------------------------------

UINT32			CDeployPolicyUtil::GetPoIdxFromPoType(UINT64 nPoType)
{
	UINT32 nPoIdx = SS_POLICY_INDEX_TOTAL_NUMBER;
	switch(nPoType)
	{
		case SS_POLICY_TYPE_UPDATE:					nPoIdx = SS_POLICY_INDEX_UPDATE;					break;
		case SS_POLICY_TYPE_SVR_INFO_LGN:			nPoIdx = SS_POLICY_INDEX_SVR_INFO_LGN;				break;
		case SS_POLICY_TYPE_HOST_RM_INFO:			nPoIdx = SS_POLICY_INDEX_HOST_RM_INFO;				break;
		case SS_POLICY_TYPE_HOST_RUN:				nPoIdx = SS_POLICY_INDEX_HOST_RUN;					break;
		case SS_POLICY_TYPE_HOST_CLEAR:				nPoIdx = SS_POLICY_INDEX_HOST_CLEAR;				break;
		case SS_POLICY_TYPE_POWER:					nPoIdx = SS_POLICY_INDEX_POWER;						break;
		case SS_POLICY_TYPE_CTL_PANEL:				nPoIdx = SS_POLICY_INDEX_CTL_PANEL;					break;
		case SS_POLICY_TYPE_CTL_PROC:				nPoIdx = SS_POLICY_INDEX_CTL_PROC;					break;
		case SS_POLICY_TYPE_SELF_PROTECT:			nPoIdx = SS_POLICY_INDEX_SELF_PROTECT;				break;
		case SS_POLICY_TYPE_DEPLOY_FILE:			nPoIdx = SS_POLICY_INDEX_DEPLOY_FILE;				break;

		case SS_POLICY_TYPE_FA_CLEAR:				nPoIdx  = SS_POLICY_INDEX_FA_CLEAR;					break;	//11
		case SS_POLICY_TYPE_FA_PROC:				nPoIdx  = SS_POLICY_INDEX_FA_PROC;					break;	//12
		case SS_POLICY_TYPE_FA_ENV:					nPoIdx  = SS_POLICY_INDEX_FA_ENV;					break;	//13
		case SS_POLICY_TYPE_FA_OP:					nPoIdx  = SS_POLICY_INDEX_FA_OP;					break;	//14
		case SS_POLICY_TYPE_FA_BK:					nPoIdx  = SS_POLICY_INDEX_FA_BK;					break;	//15

		case SS_POLICY_TYPE_WIN_RUN:				nPoIdx = SS_POLICY_INDEX_WIN_RUN;					break;
		case SS_POLICY_TYPE_SVR_INFO_UDT:			nPoIdx = SS_POLICY_INDEX_SVR_INFO_UDT;				break;	//17
		case SS_POLICY_TYPE_SVR_INFO_MGR:			nPoIdx = SS_POLICY_INDEX_SVR_INFO_MGR;				break;	//18

		case SS_POLICY_TYPE_FE_PTN_OP:				nPoIdx = SS_POLICY_INDEX_FE_PTN_OP;					break;	//19
		case SS_POLICY_TYPE_FE_PTN_BL:				nPoIdx = SS_POLICY_INDEX_FE_PTN_BL;					break;	//20
		case SS_POLICY_TYPE_FE_PTN_WL:				nPoIdx = SS_POLICY_INDEX_FE_PTN_WL;					break;	//21
		case SS_POLICY_TYPE_FE_EXCLUDE:				nPoIdx = SS_POLICY_INDEX_FE_EXCLUDE;				break;	//22
		case SS_POLICY_TYPE_FE_NOTIFY:				nPoIdx = SS_POLICY_INDEX_FE_NOTIFY;					break;	//23
		case SS_POLICY_TYPE_FE_SINGLE_PTN:			nPoIdx = SS_POLICY_INDEX_FE_SINGLE_PTN;				break;	//24
		case SS_POLICY_TYPE_FE_PTN_LO:				nPoIdx = SS_POLICY_INDEX_FE_PTN_LO;					break;	//25

		case SS_POLICY_TYPE_FA_NOTIFY:				nPoIdx = SS_POLICY_INDEX_FA_NOTIFY;					break;	//26
		case SS_POLICY_TYPE_SCH:					nPoIdx = SS_POLICY_INDEX_SCH;						break;

		case SS_POLICY_TYPE_IN_PTN_OP:				nPoIdx = SS_POLICY_INDEX_IN_PTN_OP;					break;	//33
		case SS_POLICY_TYPE_IN_PTN_BL:				nPoIdx = SS_POLICY_INDEX_IN_PTN_BL;					break;	//34
		case SS_POLICY_TYPE_IN_PTN_WL:				nPoIdx = SS_POLICY_INDEX_IN_PTN_WL;					break;	//35
		case SS_POLICY_TYPE_IN_PTN_EX:				nPoIdx = SS_POLICY_INDEX_IN_PTN_EX;					break;	//36
		case SS_POLICY_TYPE_IN_PTN_SP:				nPoIdx = SS_POLICY_INDEX_IN_PTN_SP;					break;	//37
		case SS_POLICY_TYPE_IN_PTN_NO:				nPoIdx = SS_POLICY_INDEX_IN_PTN_NO;					break;	//37

		case SS_POLICY_TYPE_IN_VULN_OP:				nPoIdx = SS_POLICY_INDEX_IN_VULN_OP;				break;	//37
		case SS_POLICY_TYPE_IN_VULN_SCAN:			nPoIdx = SS_POLICY_INDEX_IN_VULN_SCAN;				break;	//37

		case SS_POLICY_TYPE_HOST_NOTIFY:			nPoIdx = SS_POLICY_INDEX_HOST_NOTIFY;				break;

		case SS_POLICY_TYPE_PM_OP:					nPoIdx = SS_POLICY_INDEX_PM_OP;						break;
		case SS_POLICY_TYPE_PM_DM:					nPoIdx = SS_POLICY_INDEX_PM_DM;						break;
		case SS_POLICY_TYPE_PM_SCAN:				nPoIdx = SS_POLICY_INDEX_PM_SCAN;					break;
		case SS_POLICY_TYPE_PM_EX:					nPoIdx = SS_POLICY_INDEX_PM_EX;						break;
		case SS_POLICY_TYPE_PM_NO:					nPoIdx = SS_POLICY_INDEX_PM_NO;						break;

		case SS_POLICY_TYPE_IN_VULN_AX:				nPoIdx = SS_POLICY_INDEX_IN_VULN_AX;				break;	//37
		case SS_POLICY_TYPE_IN_VULN_EDIT_APP:		nPoIdx = SS_POLICY_INDEX_IN_VULN_EDIT_APP;			break;	//37
		case SS_POLICY_TYPE_IN_VULN_QNA:			nPoIdx = SS_POLICY_INDEX_IN_VULN_QNA;				break;	//37
		case SS_POLICY_TYPE_IN_VULN_SECU_USB:		nPoIdx = SS_POLICY_INDEX_IN_VULN_SECU_USB;			break;	//37
		case SS_POLICY_TYPE_IN_VULN_SW:				nPoIdx = SS_POLICY_INDEX_IN_VULN_SW;				break;	//37
		case SS_POLICY_TYPE_IN_VULN_PATCH_EXCEPTION:nPoIdx = SS_POLICY_INDEX_IN_VULN_PATCH_EXCEPTION;	break;	//37

		case SS_POLICY_TYPE_DV_OP:					nPoIdx = SS_POLICY_INDEX_DV_OP;						break;
		case SS_POLICY_TYPE_DV_BL:					nPoIdx = SS_POLICY_INDEX_DV_BL;						break;
		case SS_POLICY_TYPE_DV_WL:					nPoIdx = SS_POLICY_INDEX_DV_WL;						break;
		case SS_POLICY_TYPE_DV_EX:					nPoIdx = SS_POLICY_INDEX_DV_EX;						break;
		case SS_POLICY_TYPE_DV_NOTIFY:				nPoIdx = SS_POLICY_INDEX_DV_NOTIFY;					break;

		case SS_POLICY_TYPE_IN_RS_OP:				nPoIdx = SS_POLICY_INDEX_IN_RS_OP;					break;	//33
		case SS_POLICY_TYPE_IN_RS_NO:				nPoIdx = SS_POLICY_INDEX_IN_RS_NO;					break;	//34
		case SS_POLICY_TYPE_IN_RS_BK:				nPoIdx = SS_POLICY_INDEX_IN_RS_BK;					break;	//35

		case SS_POLICY_TYPE_NC_PTN_OP:				nPoIdx = SS_POLICY_INDEX_NC_PTN_OP;					break;	//33
		case SS_POLICY_TYPE_NC_PTN_LO:				nPoIdx = SS_POLICY_INDEX_NC_PTN_LO;					break;	//34
		case SS_POLICY_TYPE_NC_PTN_BL:				nPoIdx = SS_POLICY_INDEX_NC_PTN_BL;					break;	//34
		case SS_POLICY_TYPE_NC_PTN_WL:				nPoIdx = SS_POLICY_INDEX_NC_PTN_WL;					break;	//35
		case SS_POLICY_TYPE_NC_PTN_EX:				nPoIdx = SS_POLICY_INDEX_NC_PTN_EX;					break;	//36
		case SS_POLICY_TYPE_NC_PTN_SP:				nPoIdx = SS_POLICY_INDEX_NC_PTN_SP;					break;	//37
		case SS_POLICY_TYPE_NC_PTN_NO:				nPoIdx = SS_POLICY_INDEX_NC_PTN_NO;					break;	//37

		case SS_POLICY_TYPE_HOST_WEB_SOCK:			nPoIdx = SS_POLICY_INDEX_HOST_WEB_SOCK;				break;	//37


		default:
		{
			WriteLogE("[get po idx from po type] not find policy type %016X", nPoType);
			break;
		}
	}
	return nPoIdx;
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

INT32		CDeployPolicyUtil::ApplyPolicy(UINT64 nPolicyType, UINT32 nPoID)
{
	{
		PDB_HOST_POLICY pdhp = NULL;
		DB_HOST_POLICY dhp;
		
		if((pdhp = t_ManageHostPolicy->FindItem(DEFAULT_ID)) == NULL)
		{
			dhp.nID	= DEFAULT_ID;
			dhp.nHostID = 0;
			dhp.nRegDate = GetCurrentDateTimeInt();
			SetPolicy(dhp.tDP, nPoID, nPolicyType);

			if(SetER(t_ManageHostPolicy->AddHostPolicy(dhp)))
			{
				WriteLogE("add host policy fail : [%d]", g_nErrRtn);
				return -1;
			}
		}
		else
		{
			if(GetPolicy(pdhp->tDP, nPolicyType) != nPoID)
			{
				SetPolicy(pdhp->tDP, nPoID, nPolicyType);
				if(SetER(t_ManageHostPolicy->EditHostPolicy(*pdhp)))
				{
					WriteLogE("edit host policy fail : [%d]", g_nErrRtn);
					return -2;
				}
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------








