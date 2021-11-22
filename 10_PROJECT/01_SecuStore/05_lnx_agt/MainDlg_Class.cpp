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

// MainRun.cpp: implementation of the CMainRun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "MainDlg.h"

INT32		CMainDlg::PreCreateSubClass()
{
	INT32 nRetVal = 0;
//	CSystemInfo tSysInfo;

	do
	{
		t_SystemInfo = new CSystemInfo;
		if(t_SystemInfo == NULL)
		{
			nRetVal = -1;
			break;
		}

		t_EnvInfo = new CEnvironment();
		if(t_EnvInfo == NULL)
		{
			nRetVal = -2;
			break;
		}
		
		t_EnvInfoOp = new CEnvironmentOp();
		if(t_EnvInfo == NULL)
		{
			nRetVal = -3;
			break;
		}

		nRetVal = t_EnvInfo->LoadEnv(); 
		if(nRetVal != 0) 
		{
			nRetVal -= 1000;
			break;
		}	

		t_EnvInfoOp->SetMainHandle((HWND)this);
		
		nRetVal = t_EnvInfoOp->InitEnvOp();
		if(nRetVal != 0) 
		{
			nRetVal -= 2000;
			break;
		}

//		WriteLogN("SystemInfo : [%s] : %s", tSysInfo.GetCompName().c_str(), tSysInfo.GetSysName().c_str());

		nRetVal = 0;
	}while(FALSE);

	return nRetVal;
}

//--------------------------------------------------------------------

INT32		CMainDlg::CreateLoadLibrary()
{
	//	create dll util
	INT32 nRetVal = 0;
	do
	{
		t_ResInfoDLLUtil			= new CResInfoDLLUtil();		
		if(t_ResInfoDLLUtil == NULL)
		{
			nRetVal = -1;
			break;
		}

		t_ASICOMPDLLUtil			= new CASICOMPDLLUtil();
		if(t_ASICOMPDLLUtil == NULL)
		{
			nRetVal = -12;
			break;
		}

		t_RemoveLogDLLUtil			= new CRemoveLogDLLUtil();
		if(t_RemoveLogDLLUtil == NULL)
		{
			nRetVal = -2;
			break;
		}
		
		t_ASIFFDLLUtil				= new CASIFFDLLUtil();
		if(t_ASIFFDLLUtil == NULL)
		{
			nRetVal = -11;
			break;
		}
		/*
		t_ASIFDLDLLUtil				= new CASIFDLDLLUtil();
		if(t_ASIFDLDLLUtil == NULL)
		{
			nRetVal = -12;
			break;
		}
		*/

		t_NetworkDLLUtil			= new CNetworkDLLUtil();
		if(t_NetworkDLLUtil == NULL)
		{
			nRetVal = -3;
			break;
		}
		t_SeedDLLUtil				= new CSeedDLLUtil();
		if(t_SeedDLLUtil == NULL)
		{
			nRetVal = -4;
			break;
		}

//		t_ASIEPSAPPDLLUtil			= new CASIEPSAPPDLLUtil();

		t_ProcInfoDLLUtil			= new CPROCINFODLLUtil();
		if(t_ProcInfoDLLUtil == NULL)
		{
			nRetVal = -5;
			break;
		}
		t_ASISIDLLUtil				= new CASISIDLLUtil();
		if(t_ASISIDLLUtil == NULL)
		{
			nRetVal = -6;
			break;
		}
/*
		t_DevInfoDLLUtil			= new CDEVINFODLLUtil();
		if(t_DevInfoDLLUtil == NULL)
		{
			nRetVal = -6;
			break;
		}
*/
		t_LgnCltUtil				= new CCltSocketUtil();
		if(t_LgnCltUtil == NULL)
		{
			nRetVal = -7;
			break;
		}	
		t_MgrCltUtil				= new CCltSocketUtil();
		if(t_MgrCltUtil == NULL)
		{
			nRetVal = -8;
			break;
		}		
		t_UdtCltUtil				= new CCltSocketUtil();
		if(t_UdtCltUtil == NULL)
		{
			nRetVal = -9;
			break;
		}			
		t_LinkSvrUtil				= new CSvrSocketUtil();
		if(t_LinkSvrUtil == NULL)
		{
			nRetVal = -10;
			break;
		}

		t_ASIWENGDLLUtil			= new CASIWEngDLLUtil();
		if(t_ASIWENGDLLUtil == NULL)
		{
			nRetVal = -11;
			break;
		}

		nRetVal = 0;
	}while(FALSE);
	
	return nRetVal;
}
//--------------------------------------------------------------------

INT32		CMainDlg::CreateSubClass()
{
	int nRetVal	= 0;

	do{
	
		t_DBMS_Base = (CDBMgrBase*)new CDBMgrBase();
		if(t_DBMS_Base == NULL)
		{
			nRetVal = -1;
			break;
		}
		t_DBMS_Base->SetProperty(t_EnvInfo->t_DBAcInfoF);
		nRetVal = t_DBMS_Base->ConnectDB();
		if(nRetVal != 0)
		{
			nRetVal -= 2;
			break;
		}
		SyncDatabase();

		t_ValidTimeUtil = new CValidTimeUtil();
		if(t_ValidTimeUtil == NULL)
		{
			nRetVal = -11;
			break;
		}
		t_DeployPolicyUtil = new CDeployPolicyUtil();
		if(t_DeployPolicyUtil == NULL)
		{
			nRetVal = -12;
			break;
		}
		
		t_ExecuteFileUtil = new CExecuteFileUtil();
		if(t_ExecuteFileUtil == NULL)
		{
			nRetVal = -13;
			break;
		}
		
		t_DocBackupUtil = new CDocBackupUtil();
		if(t_DocBackupUtil == NULL)
		{
			nRetVal = -14;
			break;
		}
		
		t_FileDeleteUtil = new CFileDeleteUtil();
		if(t_FileDeleteUtil == NULL)
		{
			nRetVal = -15;
			break;
		}

		t_ComFuncUtil = new CComFuncUtil();
		if(t_ComFuncUtil == NULL)
		{
			nRetVal = -16;
			break;
		}
/*
		t_ExPopupUtil				= new CExPopupUtil();
		if(t_ExPopupUtil == NULL)
		{
			nRetVal = -17;
			break;
		}
*/

		t_ManageFileDown = new CManageFileDown();
		if(t_ManageFileDown == NULL)
		{
			nRetVal = -21;
			break;
		}

		t_ManageLogicTimer = new CManageLogicTimer();		
		if(t_ManageLogicTimer == NULL)
		{
			nRetVal = -22;
			break;
		}

//		t_ManageDeviceMedia			= new CManageDeviceMedia();

		t_ManageDevOInfo			= new CManageDevOInfo();
		if(t_ManageDevOInfo == NULL)
		{
			nRetVal = -23;
			break;
		}
//		t_ManagePMSWork				= new CManagePMSWork();
		t_ManageLinkEnv				= new CManageLinkEnv();
		if(t_ManageLinkEnv == NULL)
		{
			nRetVal = -23;
			break;
		}
//		t_ManagePatchFile			= new CManagePatchFile();
//		t_ManageVulnFileProtect		= new CManageVulnFileProtect();
//		t_ManageVulnRegProtect		= new CManageVulnRegProtect();
//		t_ManageDelLastNotify		= new CManageDelLastNotify();

//		t_ManageEpsRegInfo			= new CManageEpsRegInfo();

		t_ManageBaseUtil = new CManageBaseUtil();
		if(t_ManageBaseUtil == NULL)
		{
			nRetVal = -24;
			break;
		}
		t_ManageHostPolicy = new CManageHostPolicy();
		if(t_ManageHostPolicy == NULL)
		{
			nRetVal = -25;
			break;
		}
		t_ManageUserPolicy = new CManageUserPolicy();
		if(t_ManageUserPolicy == NULL)
		{
			nRetVal = -26;
			break;
		}
		
		t_ManageErrorCode = new CManageErrorCode();
		if(t_ManageErrorCode == NULL)
		{
			nRetVal = -27;
			break;
		}

		t_ManageEnvPolicy = new CManageEnvPolicy();		
		if(t_ManageEnvPolicy == NULL)
		{
			nRetVal = -28;
			break;
		}
		
		t_ManageLinkLicense	= new CManageLinkLicense();
		if(t_ManageLinkLicense == NULL)
		{
			nRetVal = -29;
			break;
		}

//		t_ManageEnvLocal			= new CManageEnvLocal();

		t_ManageHostPolicy->LoadDBMS();
		t_ManageUserPolicy->LoadDBMS();
		t_ManageErrorCode->LoadDBMS();
		t_ManageLinkLicense->LoadDBMS();
//      t_ManageEnvLocal->LoadDBMS();

//userdel
// 		t_ManageGroupUserDel		= new CManageGroupUserDel();
// 		t_ManageUserDel				= new CManageUserDel();

        t_ThreadDBMSQuery = new CThreadDBMSQuery();
		if(t_ThreadDBMSQuery == NULL)
		{
			nRetVal = -51;
			break;
		}

		t_ThreadTimer = new CThreadTimer();
		if(t_ThreadTimer == NULL)
		{
			nRetVal = -52;
			break;
		}

		t_ThreadSocket = new CThreadSocket();
		if(t_ThreadSocket == NULL)
		{
			nRetVal = -53;
			break;
		}

		t_ThreadLog = new CThreadLog();
		if(t_ThreadLog == NULL)
		{
			nRetVal = -54;
			break;
		}

		t_ThreadPoFaClear = new CThreadPoFaClear();
		if(t_ThreadPoFaClear == NULL)
		{
			nRetVal = -55;
			break;
		}

		t_ThreadPoFaOp = new CThreadPoFaOp();
		if(t_ThreadPoFaOp == NULL)
		{
			nRetVal = -55;
			break;
		}
		
		t_ThreadPoFeScan = new CThreadPoFeScan();
		if(t_ThreadPoFeScan == NULL)
		{
			nRetVal = -56;
			break;
		}

		t_ThreadPoFePtnLo = new CThreadPoFePtnLo();
		if(t_ThreadPoFePtnLo == NULL)
		{
			nRetVal = -57;
			break;
		}

		t_ThreadChkHkNoti = new CThreadChkHkNoti();
		if(t_ThreadChkHkNoti == NULL)
		{
			nRetVal = -58;
			break;
		}

		t_ThreadEvtMon = new CThreadEvtMon();
		if(t_ThreadEvtMon == NULL)
		{
			nRetVal = -59;
			break;
		}

		t_ThreadPoInPtnScan = new CThreadPoInPtnScan();
		if(t_ThreadPoInPtnScan == NULL)
		{
			nRetVal = -60;
			break;
		}

		t_ThreadPoInPtnFile = new CThreadPoInPtnFile();
		if(t_ThreadPoInPtnFile == NULL)
		{
			nRetVal = -61;
			break;
		}

		t_ThreadPoInAccFile = new CThreadPoInAccFile();
		if(t_ThreadPoInAccFile == NULL)
		{
			nRetVal = -62;
			break;
		}

		t_ThreadPoInCreateLog = new CThreadPoInCreateLog();
		if(t_ThreadPoInCreateLog == NULL)
		{
			nRetVal = -63;
			break;
		}

		t_ThreadPoInCreateFile = new CThreadPoInCreateFile();
		if(t_ThreadPoInCreateFile == NULL)
		{
			nRetVal = -64;
			break;
		}

		t_ThreadPoInSpReq = new CThreadPoInSpReq();
		if(t_ThreadPoInSpReq == NULL)
		{
			nRetVal = -65;
			break;
		}
//      t_ThreadPoInRsBk			= new CThreadPoInRsBk();
//		t_ThreadExecute				= new CThreadExecute();
//		t_ThreadPoNcPtnLo			= new CThreadPoNcPtnLo();
//		t_ThreadPoNcPtnScan			= new CThreadPoNcPtnScan();
		
		t_LogicLocalEnvLog	= new CLogicLocalEnvLog();
		if(t_LogicLocalEnvLog == NULL)
		{
			nRetVal = -71;
			break;
		}

		t_LogicLocalEnvAuth = new CLogicLocalEnvAuth();
		if(t_LogicLocalEnvAuth == NULL)
		{
			nRetVal = -72;
			break;
		}

		t_LogicPWEvtMon = new CLogicPWEvtMon();
		if(t_LogicPWEvtMon == NULL)
		{
			nRetVal = -73;
			break;
		}

		t_LogicLgnAuth = new CLogicLgnAuth();
		if(t_LogicLgnAuth == NULL)
		{
			nRetVal = -74;
			break;
		}

		t_LogicUdtAuth = new CLogicUdtAuth();
		if(t_LogicUdtAuth == NULL)
		{
			nRetVal = -75;
			break;
		}

		t_LogicMgrAuth = new CLogicMgrAuth();
		if(t_LogicMgrAuth == NULL)
		{
			nRetVal = -81;
			break;
		}

		t_LogicMgrHost = new CLogicMgrHost();
		if(t_LogicMgrHost == NULL)
		{
			nRetVal = -82;
			break;
		}

		t_LogicMgrHostSys = new CLogicMgrHostSys();
		if(t_LogicMgrHostSys == NULL)
		{
			nRetVal = -83;
			break;
		}

		t_LogicMgrHostHw = new CLogicMgrHostHw();
		if(t_LogicMgrHostHw == NULL)
		{
			nRetVal = -84;
			break;
		}

		t_LogicMgrHostSw = new CLogicMgrHostSw();
		if(t_LogicMgrHostSw == NULL)
		{
			nRetVal = -85;
			break;
		}

		t_LogicMgrHostStatusPo = new CLogicMgrHostStatusPo();
		if(t_LogicMgrHostStatusPo == NULL)
		{
			nRetVal = -86;
			break;
		}
//        t_LogicMgrHostStatusPoNc	= new CLogicMgrHostStatusPoNc();

		t_LogicMgrHostKey			= new CLogicMgrHostKey();
		if(t_LogicMgrHostKey == NULL)
		{
			nRetVal = -86;
			break;
		}

		t_LogicMgrHostPatch			= new CLogicMgrHostPatch();
		if(t_LogicMgrHostPatch == NULL)
		{
			nRetVal = -87;
			break;
		}

		t_LogicMgrHostVulnRst		= new CLogicMgrHostVulnRst();
		if(t_LogicMgrHostVulnRst == NULL)
		{
			nRetVal = -87;
			break;
		}

		t_LogicMgrUser = new CLogicMgrUser();
		if(t_LogicMgrUser == NULL)
		{
			nRetVal = -88;
			break;
		}

		t_LogicMgrSiteFile = new CLogicMgrSiteFile();
		if(t_LogicMgrSiteFile == NULL)
		{
			nRetVal = -89;
			break;
		}
		t_LogicMgrSiteFileAlias			= new CLogicMgrSiteFileAlias();
		if(t_LogicMgrSiteFileAlias == NULL)
		{
			nRetVal = -92;
			break;
		}
		t_LogicMgrSiteVuln = new CLogicMgrSiteVuln();
		if(t_LogicMgrSiteVuln == NULL)
		{
			nRetVal = -90;
			break;
		}

		t_LogicMgrSiteVulnScan = new CLogicMgrSiteVulnScan();
		if(t_LogicMgrSiteVulnScan == NULL)
		{
			nRetVal = -91;
			break;
		}

//      t_LogicMgrInfoProcReputation	= new CLogicMgrInfoProcReputation();

		t_LogicMgrEnvLicense = new CLogicMgrEnvLicense();
		if(t_LogicMgrEnvLicense == NULL)
		{
			nRetVal = -94;
			break;
		}

		t_LogicMgrEnvLog = new CLogicMgrEnvLog();
		if(t_LogicMgrEnvLog == NULL)
		{
			nRetVal = -95;
			break;
		}

		t_LogicMgrEnvLogUnit = new CLogicMgrEnvLogUnit();
		if(t_LogicMgrEnvLogUnit == NULL)
		{
			nRetVal = -95;
			break;
		}

		t_LogicMgrEnvNotifyInfo = new CLogicMgrEnvNotifyInfo();
		if(t_LogicMgrEnvNotifyInfo == NULL)
		{
			nRetVal = -96;
			break;
		}

		t_LogicMgrEnvSelfProtectAgt	= new CLogicMgrEnvSelfProtectAgt();
		if(t_LogicMgrEnvSelfProtectAgt == NULL)
		{
			nRetVal = -97;
			break;
		}

		t_LogicMgrEnvSOrgLink = new CLogicMgrEnvSOrgLink();
		if(t_LogicMgrEnvSOrgLink == NULL)
		{
			nRetVal = -98;
			break;
		}

		t_LogicMgrEnvTrustSubject = new CLogicMgrEnvTrustSubject();
		if(t_LogicMgrEnvTrustSubject == NULL)
		{
			nRetVal = -99;
			break;
		}
		
		t_LogicMgrEnvSocket			= new CLogicMgrEnvSocket();
		if(t_LogicMgrEnvSocket == NULL)
		{
			nRetVal = -99;
			break;
		}

		t_LogicMgrPoHostRmInfo = new CLogicMgrPoHostRmInfo();
		if(t_LogicMgrPoHostRmInfo == NULL)
		{
			nRetVal = -100;
			break;
		}

		t_LogicMgrPoHostRun = new CLogicMgrPoHostRun();
		if(t_LogicMgrPoHostRun == NULL)
		{
			nRetVal = -101;
			break;
		}

		t_LogicMgrPoSvrInfoLgn = new CLogicMgrPoSvrInfoLgn();
		if(t_LogicMgrPoSvrInfoLgn == NULL)
		{
			nRetVal = -102;
			break;
		}

		t_LogicMgrPoSvrInfoUdt = new CLogicMgrPoSvrInfoUdt();
		if(t_LogicMgrPoSvrInfoUdt == NULL)
		{
			nRetVal = -103;
			break;
		}

		t_LogicMgrPoHostNotify		= new CLogicMgrPoHostNotify();
		if(t_LogicMgrPoSvrInfoUdt == NULL)
		{
			nRetVal = -103;
			break;
		}

//		t_LogicMgrPoHostWebSock		= new CLogicMgrPoHostWebSock();

		t_LogicMgrPoSch = new CLogicMgrPoSch();
		if(t_LogicMgrPoSch == NULL)
		{
			nRetVal = -104;
			break;
		}

		t_LogicMgrPoPower = new CLogicMgrPoPower();
		if(t_LogicMgrPoPower == NULL)
		{
			nRetVal = -105;
			break;
		}

		t_LogicMgrPoWinRun = new CLogicMgrPoWinRun();
		if(t_LogicMgrPoWinRun == NULL)
		{
			nRetVal = -106;
			break;
		}

		t_LogicMgrPoCtlPanel = new CLogicMgrPoCtlPanel();
		if(t_LogicMgrPoCtlPanel == NULL)
		{
			nRetVal = -107;
			break;
		}

		t_LogicMgrPoDeployFile = new CLogicMgrPoDeployFile();
		if(t_LogicMgrPoDeployFile == NULL)
		{
			nRetVal = -108;
			break;
		}
		
		t_LogicMgrPoCtlProc = new CLogicMgrPoCtlProc();
		if(t_LogicMgrPoCtlProc == NULL)
		{
			nRetVal = -109;
			break;
		}

		t_LogicMgrPoSelfProtect = new CLogicMgrPoSelfProtect();
		if(t_LogicMgrPoSelfProtect == NULL)
		{
			nRetVal = -110;
			break;
		}

		t_LogicMgrPoFaOp = new CLogicMgrPoFaOp();
		if(t_LogicMgrPoFaOp == NULL)
		{
			nRetVal = -111;
			break;
		}
		
		t_LogicMgrPoFaClear = new CLogicMgrPoFaClear();
		if(t_LogicMgrPoFaClear == NULL)
		{
			nRetVal = -112;
			break;
		}

		t_LogicMgrPoFaEnv = new CLogicMgrPoFaEnv();
		if(t_LogicMgrPoFaEnv == NULL)
		{
			nRetVal = -113;
			break;
		}

		t_LogicMgrPoFaProc = new CLogicMgrPoFaProc();
		if(t_LogicMgrPoFaProc == NULL)
		{
			nRetVal = -114;
			break;
		}
		
		t_LogicMgrPoFaBk = new CLogicMgrPoFaBk();
		if(t_LogicMgrPoFaBk == NULL)
		{
			nRetVal = -115;
			break;
		}

		t_LogicMgrPoFaNotify = new CLogicMgrPoFaNotify();
		if(t_LogicMgrPoFaNotify == NULL)
		{
			nRetVal = -116;
			break;
		}

		t_LogicMgrPoFaDelFileAfterBoot = new CLogicMgrPoFaDelFileAfterBoot();
		if(t_LogicMgrPoFaDelFileAfterBoot == NULL)
		{
			nRetVal = -156;
			break;
		}

		t_LogicMgrPoFaInotifyFile = new CLogicMgrPoFaInotifyFile();
		if(t_LogicMgrPoFaInotifyFile == NULL)
		{
			nRetVal = -157;
			break;
		}
		
		t_LogicMgrPoFePtnOp	= new CLogicMgrPoFePtnOp();
		if(t_LogicMgrPoFePtnOp == NULL)
		{
			nRetVal = -117;
			break;
		}

		t_LogicMgrPoFePtnLo = new CLogicMgrPoFePtnLo();
		if(t_LogicMgrPoFePtnLo == NULL)
		{
			nRetVal = -118;
			break;
		}

		t_LogicMgrPoFePtnBL = new CLogicMgrPoFePtnBL();	
		if(t_LogicMgrPoFePtnBL == NULL)
		{
			nRetVal = -119;
			break;
		}

		t_LogicMgrPoFePtnWL	= new CLogicMgrPoFePtnWL();	
		if(t_LogicMgrPoFePtnWL == NULL)
		{
			nRetVal = -120;
			break;
		}

		t_LogicMgrPoFeExclude = new CLogicMgrPoFeExclude();
		if(t_LogicMgrPoFeExclude == NULL)
		{
			nRetVal = -121;
			break;
		}

		t_LogicMgrPoFeNotify = new CLogicMgrPoFeNotify();
		if(t_LogicMgrPoFeNotify == NULL)
		{
			nRetVal = -122;
			break;
		}

		t_LogicMgrPoFeSinglePtn = new CLogicMgrPoFeSinglePtn();
		if(t_LogicMgrPoFeSinglePtn == NULL)
		{
			nRetVal = -123;
			break;
		}
		
		t_LogicMgrPoInPtnOp = new CLogicMgrPoInPtnOp();
		if(t_LogicMgrPoInPtnOp == NULL)
		{
			nRetVal = -124;
			break;
		}

		t_LogicMgrPoInPtnBL	= new CLogicMgrPoInPtnBL();
		if(t_LogicMgrPoInPtnBL == NULL)
		{
			nRetVal = -125;
			break;
		}

		t_LogicMgrPoInPtnWL	= new CLogicMgrPoInPtnWL();
		if(t_LogicMgrPoInPtnWL == NULL)
		{
			nRetVal = -126;
			break;
		}

		t_LogicMgrPoInPtnEx	= new CLogicMgrPoInPtnEx();
		if(t_LogicMgrPoInPtnEx == NULL)
		{
			nRetVal = -127;
			break;
		}
		
		t_LogicMgrPoInPtnExTrust = new CLogicMgrPoInPtnExTrust();
		if(t_LogicMgrPoInPtnExTrust == NULL)
		{
			nRetVal = -128;
			break;
		}

		t_LogicMgrPoInPtnSP	= new CLogicMgrPoInPtnSP();
		if(t_LogicMgrPoInPtnSP == NULL)
		{
			nRetVal = -129;
			break;
		}

		t_LogicMgrPoInPtnSPRule	= new CLogicMgrPoInPtnSPRule();
		if(t_LogicMgrPoInPtnSPRule == NULL)
		{
			nRetVal = -130;
			break;
		}

		t_LogicMgrPoInPtnNo	= new CLogicMgrPoInPtnNo();
		if(t_LogicMgrPoInPtnNo == NULL)
		{
			nRetVal = -131;
			break;
		}

		t_LogicMgrPoInVulnAx		= new CLogicMgrPoInVulnAx();
		if(t_LogicMgrPoInVulnAx == NULL)
		{
			nRetVal = -132;
			break;
		}
		t_LogicMgrPoInVulnEditApp	= new CLogicMgrPoInVulnEditApp();
		if(t_LogicMgrPoInVulnEditApp == NULL)
		{
			nRetVal = -133;
			break;
		}
		t_LogicMgrPoInVulnQna		= new CLogicMgrPoInVulnQna();
		if(t_LogicMgrPoInVulnQna == NULL)
		{
			nRetVal = -134;
			break;
		}
		t_LogicMgrPoInVulnOp = new CLogicMgrPoInVulnOp();
		if(t_LogicMgrPoInVulnOp == NULL)
		{
			nRetVal = -135;
			break;
		}


		t_LogicMgrPoInVulnScan = new CLogicMgrPoInVulnScan();
		if(t_LogicMgrPoInVulnScan == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInVulnSecuUsb	= new CLogicMgrPoInVulnSecuUsb();
		if(t_LogicMgrPoInVulnSecuUsb == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInVulnSw		= new CLogicMgrPoInVulnSw();
		if(t_LogicMgrPoInVulnSw == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInVulnPatchException = new CLogicMgrPoInVulnPatchException();
		if(t_LogicMgrPoInVulnPatchException == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInRsOp			= new CLogicMgrPoInRsOp();
		if(t_LogicMgrPoInRsOp == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInRsBk			= new CLogicMgrPoInRsBk();
		if(t_LogicMgrPoInRsBk == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInRsNo			= new CLogicMgrPoInRsNo();
		if(t_LogicMgrPoInRsNo == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInDevOOp		= new CLogicMgrPoInDevOOp();
		if(t_LogicMgrPoInDevOOp == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInDevOBL		= new CLogicMgrPoInDevOBL();
		if(t_LogicMgrPoInDevOBL == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInDevOWL		= new CLogicMgrPoInDevOWL();
		if(t_LogicMgrPoInDevOWL == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoInDevOEx		= new CLogicMgrPoInDevOEx();
		if(t_LogicMgrPoInDevOEx == NULL)
		{
			nRetVal = -136;
			break;
		}
		t_LogicMgrPoInDevOInfo		= new CLogicMgrPoInDevOInfo();
		if(t_LogicMgrPoInDevOInfo == NULL)
		{
			nRetVal = -136;
			break;
		}
		t_LogicMgrPoInDevONotify	= new CLogicMgrPoInDevONotify();
		if(t_LogicMgrPoInDevONotify == NULL)
		{
			nRetVal = -136;
			break;
		}
		
		t_LogicMgrPoDvLo			= new CLogicMgrPoDvLo();

		if(t_LogicMgrPoDvLo == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoPmOp			= new CLogicMgrPoPmOp();
		if(t_LogicMgrPoPmOp == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrPoPmDm			= new CLogicMgrPoPmDm();
		if(t_LogicMgrPoPmDm == NULL)
		{
			nRetVal = -136;
			break;
		}
		t_LogicMgrPoPmScan			= new CLogicMgrPoPmScan();
		if(t_LogicMgrPoPmScan == NULL)
		{
			nRetVal = -136;
			break;
		}
		t_LogicMgrPoPmEx			= new CLogicMgrPoPmEx();
		if(t_LogicMgrPoPmEx == NULL)
		{
			nRetVal = -136;
			break;
		}
		t_LogicMgrPoPmNo			= new CLogicMgrPoPmNo();
		if(t_LogicMgrPoPmNo == NULL)
		{
			nRetVal = -136;
			break;
		}

//		t_LogicMgrPoNcPtnOp			= new CLogicMgrPoNcPtnOp();
//		t_LogicMgrPoNcPtnLo			= new CLogicMgrPoNcPtnLo();
//		t_LogicMgrPoNcPtnBL			= new CLogicMgrPoNcPtnBL();
//		t_LogicMgrPoNcPtnWL			= new CLogicMgrPoNcPtnWL();
//		t_LogicMgrPoNcPtnEx			= new CLogicMgrPoNcPtnEx();
//		t_LogicMgrPoNcPtnExTrust	= new CLogicMgrPoNcPtnExTrust();
//		t_LogicMgrPoNcPtnSP			= new CLogicMgrPoNcPtnSP();
		t_LogicMgrPoNcPtnSPRule		= new CLogicMgrPoNcPtnSPRule();
//		t_LogicMgrPoNcPtnNo			= new CLogicMgrPoNcPtnNo();

		t_LogicMgrLogEvent			= new CLogicMgrLogEvent();
		if(t_LogicMgrLogEvent == NULL)
		{
			nRetVal = -136;
			break;
		}

		t_LogicMgrLogDeployFile	= new CLogicMgrLogDeployFile();
		if(t_LogicMgrLogDeployFile == NULL)
		{
			nRetVal = -137;
			break;
		}

		t_LogicMgrLogDoc = new CLogicMgrLogDoc();
		if(t_LogicMgrLogDoc == NULL)
		{
			nRetVal = -138;
			break;
		}

		t_LogicMgrLogSecu = new CLogicMgrLogSecu();
		if(t_LogicMgrLogSecu == NULL)
		{
			nRetVal = -139;
			break;
		}

		t_LogicMgrLogPatch			= new CLogicMgrLogPatch();
		if(t_LogicMgrLogPatch == NULL)
		{
			nRetVal = -140;
			break;
		}

		t_LogicMgrLogDevice			= new CLogicMgrLogDevice();
		if(t_LogicMgrLogDevice == NULL)
		{
			nRetVal = -140;
			break;
		}

		t_LogicMgrLogRs				= new CLogicMgrLogRs();
		if(t_LogicMgrLogRs == NULL)
		{
			nRetVal = -140;
			break;
		}


		t_LogicMgrLogRsBk			= new CLogicMgrLogRsBk();
		if(t_LogicMgrLogRsBk == NULL)
		{
			nRetVal = -140;
			break;
		}
		t_LogicMgrCtrlRemoteCtrl = new CLogicMgrCtrlRemoteCtrl();
		if(t_LogicMgrCtrlRemoteCtrl == NULL)
		{
			nRetVal = -141;
			break;
		}

		t_LogicMgrCtrlRemotePower = new CLogicMgrCtrlRemotePower();
		if(t_LogicMgrCtrlRemotePower == NULL)
		{
			nRetVal = -142;
			break;
		}

		t_LogicMgrCtrlRemoteOrder = new CLogicMgrCtrlRemoteOrder();
		if(t_LogicMgrCtrlRemoteOrder == NULL)
		{
			nRetVal = -143;
			break;
		}

		t_LogicMgrCtrlInstantMsg = new CLogicMgrCtrlInstantMsg();
		if(t_LogicMgrCtrlInstantMsg == NULL)
		{
			nRetVal = -144;
			break;
		}

		t_LogicMgrCtrlHostRes = new CLogicMgrCtrlHostRes();
		if(t_LogicMgrCtrlHostRes == NULL)
		{
			nRetVal = -145;
			break;
		}

		t_LogicMgrCtrlRestore = new CLogicMgrCtrlRestore();
		if(t_LogicMgrCtrlRestore == NULL)
		{
			nRetVal = -145;
			break;
		}

		t_LogicMgrPtnPatch			= new CLogicMgrPtnPatch();
		if(t_LogicMgrPtnPatch == NULL)
		{
			nRetVal = -145;
			break;
		}

		t_LogicMgrPtnVuln			= new CLogicMgrPtnVuln();
		if(t_LogicMgrPtnVuln == NULL)
		{
			nRetVal = -145;
			break;
		}

		t_LogicAuth	= new CLogicAuth();
		if(t_LogicAuth == NULL)
		{
			nRetVal = -151;
			break;
		}

		t_LogicInitLink	= new CLogicInitLink();
		if(t_LogicInitLink == NULL)
		{
			nRetVal = -152;
			break;
		}

		t_LogicLogEvent	= new CLogicLogEvent();
		if(t_LogicLogEvent == NULL)
		{
			nRetVal = -153;
			break;
		}

		t_LogicLogDocDScan = new CLogicLogDocDScan();
		if(t_LogicLogDocDScan == NULL)
		{
			nRetVal = -154;
			break;
		}

		t_LogicDocDeleteInfo		= new CLogicDocDeleteInfo();
		if(t_LogicDocDeleteInfo == NULL)
		{
			nRetVal = -154;
			break;
		}

		t_LogicCtrlVuln				= new CLogicCtrlVuln();
		if(t_LogicCtrlVuln == NULL)
		{
			nRetVal = -154;
			break;
		}

		t_LogicApplyPolicy = new CLogicApplyPolicy();
		if(t_LogicApplyPolicy == NULL)
		{
			nRetVal = -155;
			break;
		}
		
		nRetVal = 0;
	}while(FALSE);
	
	return nRetVal;
}
//--------------------------------------------------------------------

INT32		CMainDlg::InitLoadLibrary()
{
	INT32 nRetVal = 0;
	CHAR szDLLName[CHAR_MAX_SIZE] = {0, };

	do
	{
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_resinfo.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ResInfoDLLUtil)
			nRetVal = t_ResInfoDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -11;
		}
	
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_comp.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ASICOMPDLLUtil)
			if(t_ASICOMPDLLUtil->LoadLibraryExt(szDLLName))
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -12;
		}
		
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_rmlog.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_RemoveLogDLLUtil)
			nRetVal = t_RemoveLogDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -13;
		}
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_ff.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ASIFFDLLUtil)
			nRetVal = t_ASIFFDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -14;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_weng.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ASIFFDLLUtil)
			nRetVal = t_ASIWENGDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -15;
		}
/*
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_fdl.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ASIFDLDLLUtil->LoadLibraryExt(szDLLName)) 
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, errno);
			return -15;
		}
*/

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_network.so", t_EnvInfo->m_strDLLPath.c_str());
		nRetVal = t_NetworkDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -16;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_seed.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_SeedDLLUtil)
			nRetVal = t_SeedDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0) 
		{
			WriteLogE("fail to load library : [%s][%d] : %s", szDLLName, nRetVal, strerror(errno));
			return -17;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_procinfo.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ProcInfoDLLUtil)
			nRetVal = t_ProcInfoDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0) 
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -18;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_swinfo.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_ASISIDLLUtil)
			nRetVal = t_ASISIDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0) 
		{
			WriteLogE("load library fail : [%s][%d]", szDLLName, GetLastError());
			return -19;
		}
/*
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_devinfo.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_DevInfoDLLUtil)
			nRetVal = t_DevInfoDLLUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0) 
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -19;
		}
*/
		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_cltsock.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_LgnCltUtil)
			nRetVal = t_LgnCltUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)	
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -20;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_cltsock.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_MgrCltUtil)
			nRetVal = t_MgrCltUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)	
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -21;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_cltsock.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_UdtCltUtil)
			nRetVal = t_UdtCltUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)	
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -22;
		}

		snprintf(szDLLName, CHAR_MAX_SIZE-1, "%s/asi_svrsock.so", t_EnvInfo->m_strDLLPath.c_str());
		if(t_LinkSvrUtil)
			nRetVal = t_LinkSvrUtil->LoadLibraryExt(szDLLName);
		if(nRetVal != 0)
		{
			WriteLogE("fail to load library : [%s][%d]", szDLLName, nRetVal);
			return -23;
		}
		nRetVal = 0;
	}while(FALSE);

	return nRetVal;
}
//--------------------------------------------------------------------

INT32		CMainDlg::InitSubClass() 
{

	{
		ASI_FDL_INIT tAFI;
		t_EnvInfoOp->GetDLFileInitData(tAFI);
		SetDLInit(&tAFI);
/*
		if(t_ASIFDLDLLUtil)
			t_ASIFDLDLLUtil->SetDLInit(&tAFI);		
*/
	}

	{
		ASI_FF_INIT_INFO tAFII;
		t_EnvInfoOp->GetFFInitData(tAFII);
		if(t_ASIFFDLLUtil)		
			t_ASIFFDLLUtil->ASIFF_Init(&tAFII);
	}

	{
		ASISI_INIT tASI;
		t_EnvInfoOp->GetASInitData(tASI);
		t_ASISIDLLUtil->Init(&tASI);
	}

	{
		t_ASIWENGDLLUtil->Init();
		t_ASIWENGDLLUtil->InitDB();
	}
/*
	{
		t_ASIDVIDLLUtil->Init();
		t_ASIWRIDLLUtil->Init();
	}
*/
	{
		if(t_ProcInfoDLLUtil)
			t_ProcInfoDLLUtil->Init();
/*
		if(t_DevInfoDLLUtil)
			t_DevInfoDLLUtil->Init();
*/
	}
/*
	{
		{
			ASI_PMS_INIT tAPI;
			t_EnvInfoOp->GetASIPMSPatchInitData(tAPI);
			t_ASIPMSPatchUtil->ASIPMS_Init(&tAPI);
		}
		{
			ASI_PMS_INIT tAPI;
			t_EnvInfoOp->GetASIPMSVulnInitData(tAPI);
			t_ASIPMSVulnUtil->ASIPMS_Init(&tAPI);
		}
	}

	{
		ASI_WEBSOCK_INIT tAWSI;
		t_EnvInfoOp->GetASIWebSockInitData(tAWSI);
		t_ASIWebSockDLLUtil->ASIWEBSOCK_Init(&tAWSI);
	}
*/


	//----------------------------------------------------------------------------
/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
	{
		if(SetER(t_LogicMgrPtnPatch->InitPtnPatch()))
		{
			WriteLogN("init ptn patch fail : [%d]", g_nErrRtn);
		}
		else
		{
			WriteLogN("init ptn patch success");
		}
	}
*/
/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
	{
		if(SetER(t_LogicMgrPtnVuln->InitPtnVuln()))
		{
			WriteLogN("init ptn vuln fail : [%d]", g_nErrRtn);
		}
		else
		{
			WriteLogN("init ptn vuln success");
		}
	}
*/
	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::InitRunEnv()
{
	{
		PDB_PO_HOST_RUN pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
		if(pdphr && pdphr->nLnxBootChkType == SS_PO_HOST_RUN_BOOT_CHK_TYPE_START_SVC)
		{
			t_EnvInfoOp->SetSysBootMode();
			WriteLogN("set system boot mode on by host_run policy");
		}
	}

	{
//		t_EnvInfo->SetReg_AllUserAutoEndTasks(0);
	}

	{
		t_ManageSysDrive.InitSysDrive();
	}
/*

	{
		PDB_PO_POWER pdpp = (PDB_PO_POWER)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_POWER);
		if(pdpp)
		{
			if(pdpp->tDPH.nUsedFlag == 1 && (pdpp->nFastBoot == SS_PO_POWER_FASTBOOT_OFF))
				t_EnvInfo->SetReg_HiberbootEnabled(0);
			else if(pdpp->tDPH.nUsedFlag == 1 && (pdpp->nFastBoot == SS_PO_POWER_FASTBOOT_ON))
				t_EnvInfo->SetReg_HiberbootEnabled(1);
		}
	}

	{
		StopProcUtil();
	}
*/

	
	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::InitTimerID()
{
	t_ThreadTimer->t_TimerUtil.SetNotifyWnd((HWND)this, (UINT32)WM_GLOBAL_TIMER_EVT);

	{
		UINT32 nLgnPeriod = 5;
		PDB_PO_HOST_RUN pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
		if(pdphr)
			nLgnPeriod = pdphr->nLnxLgnConPeriod;

		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_LOGIN_SVR_AUTH, TIMER_INTERVAL_TIME_SEC*nLgnPeriod, NULL, 1, 0, -1, 1);
	}
	
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_LOGIN_SVR_AUTH_LAST,			TIMER_INTERVAL_TIME_MIN*5, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_MANAGER_SVR_AUTH,				TIMER_INTERVAL_TIME_SEC*5, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_MANAGER_SVR_AUTH_LAST,			TIMER_INTERVAL_TIME_MIN*5, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_UPDATE_SVR_AUTH,				TIMER_INTERVAL_TIME_SEC*5, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_UPDATE_SVR_AUTH_LAST,			TIMER_INTERVAL_TIME_MIN*5, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_DOWNLOAD_UPDATE_FILE,			TIMER_INTERVAL_TIME_SEC*1, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_BOOT_OPERATION_END,		TIMER_INTERVAL_TIME_SEC*1, NULL, 1, 1);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_OFF_OPERATION,				TIMER_INTERVAL_TIME_SEC, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_OFF_OPERATION_END,			TIMER_INTERVAL_TIME_SEC*5, NULL, 1, 0);
//	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_OFF_OPERATION_CHG_SESSION,	TIMER_INTERVAL_TIME_SEC*2, NULL, 1, 1);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_CHECK_CHANGE_NET_DRIVE,		TIMER_INTERVAL_TIME_SEC*10, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_CHECK_CHANGE_FIX_DRIVE,		TIMER_INTERVAL_TIME_SEC*1, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_CHECK_CHANGE_LOGIN_USER,		TIMER_INTERVAL_TIME_SEC*10, NULL, 1, 0);

	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_EPS,			TIMER_INTERVAL_TIME_SEC*1, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_HOST_NOTIFY,		TIMER_INTERVAL_TIME_MIN*1, NULL, 1, 1);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_WINDOWS_EVENT_HANDLER,		TIMER_INTERVAL_TIME_MIN, NULL, 1, 0);

	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_TS_PROTECT_MODE,			TIMER_INTERVAL_TIME_MIN*1, NULL, 1, 1);

	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_DP_FILE_EXCUTE_END,		TIMER_INTERVAL_TIME_SEC*10, NULL, 1, 0);

	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_SEND_KEEP_ALIVE,			TIMER_INTERVAL_TIME_MIN*1, NULL, 1, 0);
	t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_CONNECT_KEEP_MANAGE,		TIMER_INTERVAL_TIME_MIN*1, NULL, 1, 0);


	{		
		t_ManageLogicTimer->AddLogicBase(t_LogicMgrPoHostRun,		TIMER_INTERVAL_TIME_MIN*1);
		t_ManageLogicTimer->AddLogicBase(t_LogicMgrPoDeployFile,	TIMER_INTERVAL_TIME_SEC*1);
		t_ManageLogicTimer->AddLogicBase(t_LogicMgrPoFePtnOp,		TIMER_INTERVAL_TIME_MIN*1);
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_CHECK,	TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
	}

	{
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_FA_CLEAR,			TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_FA_OP_CLEAR,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_POWER,			TIMER_INTERVAL_TIME_MIN*1,	NULL, 1, 0);
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_IN_PTN_OP,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_IN_PTN_SP_RULE,	TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_IN_RS_BK,			TIMER_INTERVAL_TIME_HOUR*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_PM_SCAN,			TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_PM_ROLLBACK,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_PM_MON_RM_PATCH,	TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_IN_VULN_SCAN,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_NC_PTN_OP,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);
//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_APPLY_NC_PTN_SP_RULE,	TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);

//		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_POLICY_READY_IN_DEPLOY,		TIMER_INTERVAL_TIME_SEC*1,	NULL, 1, 0);

	}

	{
		t_ThreadTimer->t_TimerUtil.AddTimer(TIMER_ID_INFO_PROC_REPUTATION,			TIMER_INTERVAL_TIME_SEC*30,	NULL, 1, 0);
	}
	{
//		t_ThreadSubTimer->t_TimerUtil.SetNotifyWnd(GetSafeHwnd(), WM_GLOBAL_TIMER_EVT_SUB);
//		t_ThreadSubTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_OFF_OPERATION_CHK_WORK_TIME,		TIMER_INTERVAL_TIME_MIN*1, (OnTimerType)SubOnTimerType, 1, 0);
//		t_ThreadSubTimer->t_TimerUtil.AddTimer(TIMER_ID_SYS_OFF_OPERATION_END_CHK_WORK_TIME,	TIMER_INTERVAL_TIME_SEC*1, (OnTimerType)SubOnTimerType, 1, 0);

//		t_ThreadSubTimer->t_TimerUtil.AddTimer(TIMER_ID_CHK_EXIST_INTERVAL,						TIMER_INTERVAL_TIME_MIN*1, (OnTimerType)SubOnTimerType, 1, 1, -1, 1);
	}

	return 0;
}

//--------------------------------------------------------------------

INT32		CMainDlg::StartSubClass()
{
	BOOL bRetVal = TRUE;
	INT32 nRetVal = 0;
	pthread_t nThreadId = 0;
	pthread_t nTimerThreadId = 0;
	pthread_t nSocketThreadId = 0;
	{
		bRetVal = t_ThreadDBMSQuery->CreateThreadExt("dbquery", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("fail to start dbquery thread [%d]", errno);
			return -1;
		}
		bRetVal = t_ThreadTimer->CreateThreadExt("timer", &nTimerThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [timer] thread result : fail [%d]", errno);
			return -2;
		}
		bRetVal = t_ThreadSocket->CreateThreadExt("socket", &nSocketThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [socket] thread result : fail [%d]", errno);
			return -3;
		}
		t_ThreadEvtMon->SetThreadID(nTimerThreadId, nSocketThreadId);
		nThreadId = 0;
		bRetVal = t_ThreadEvtMon->CreateThreadExt("evt_mon", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [evt_mon] thread result : fail [%d]", errno);
			return -4;
		}

		nThreadId = 0;
		bRetVal = t_ThreadLog->CreateThreadExt("log", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [log] thread result : fail [%d]", errno);
			return -5;
		}
		nThreadId = 0;
		bRetVal = t_ThreadPoFaClear->CreateThreadExt("po_fa_clear", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_fa_clear] thread result : fail [%d]", errno);
			return -6;
		}
/*
		if(SetER(t_ThreadPoFaExPopup->CreateThreadExt("po_fa_ex_popup")) == 0)
		{
			WriteLogE("start [po_fa_ex_popup] thread result : fail [%d]", GetLastError());
			return -1;
		}
*/
		nThreadId = 0;
		bRetVal = t_ThreadPoFePtnLo->CreateThreadExt("po_fe_ptn_lo", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_fe_ptn_lo] thread result : fail [%d]", errno);
			return -7;
		}
		nThreadId = 0;
		bRetVal = t_ThreadPoFeScan->CreateThreadExt("po_fe_scan", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_fe_scan] thread result : fail [%d]", errno);
			return -8;
		}
		nThreadId = 0;
		bRetVal = t_ThreadPoInPtnScan->CreateThreadExt("po_in_scan", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_scan] thread result : fail [%d]", errno);
			return -9;
		}
/*
		if(SetER(t_ThreadPoInRsBk->CreateThreadExt("po_in_rs_bk")) == 0)
		{
			WriteLogE("start [po_in_rs_bk] thread result : fail [%d]", GetLastError());
			return -1;
		}
*/
		nThreadId = 0;
		bRetVal = t_ThreadChkHkNoti->CreateThreadExt("chk_hk_noti", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [chk_hk_noti] thread result : fail [%d]", errno);
			return -10;
		}
		nThreadId = 0;
		bRetVal = t_ThreadPoFaOp->CreateThreadExt("po_fa_op", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_fa_op] thread result : fail [%d]", errno);
			return -11;
		}

		nThreadId = 0;
		bRetVal = t_ThreadPoInPtnFile->CreateThreadExt("po_in_ptn_file", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_ptn_file] thread result : fail [%d]", errno);
			return -13;
		}

		nThreadId = 0;
		bRetVal = t_ThreadPoInAccFile->CreateThreadExt("po_in_acc_file", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_acc_file] thread result : fail [%d]", errno);
			return -14;
		}

		nThreadId = 0;
		bRetVal = t_ThreadPoInCreateLog->CreateThreadExt("po_in_log", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_log] thread result : fail [%d]", errno);
			return -15;
		}

		nThreadId = 0;
		bRetVal = t_ThreadPoInCreateFile->CreateThreadExt("po_in_create", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_create] thread result : fail [%d]", errno);
			return -16;
		}

		nThreadId = 0;
		bRetVal = t_ThreadPoInSpReq->CreateThreadExt("po_in_sp_req", &nThreadId);
		if(bRetVal == FALSE)
		{
			WriteLogE("start [po_in_sp_req] thread result : fail [%d]", errno);
			return -17;
		}
/*
		if(SetER(t_ThreadExecute->CreateThreadExt("execute")) == 0)
		{
			WriteLogE("start [execute] thread result : fail [%d]", GetLastError());
			return -1;
		}
		//--------------------------------------------------------------------------------------------------------

		if(SetER(t_ThreadPoNcPtnLo->CreateThreadExt("po_nc_ptn_lo")) == 0)
		{
			WriteLogE("start [po_nc_ptn_lo] thread result : fail [%d]", GetLastError());
			return -1;
		}

		if(SetER(t_ThreadPoNcPtnScan->CreateThreadExt("po_nc_scan")) == 0)
		{
			WriteLogE("start [po_nc_scan] thread result : fail [%d]", GetLastError());
			return -1;
		}
*/
	}	

	{		
		SOCK_INIT_DATA tSID;
		INT32 nResult = 0;
		UINT32 nIFPort = t_EnvInfo->m_nIFPort;

		{
			t_LinkSvrUtil->StopServer(1);
			t_LinkSvrUtil->CloseSocket(0, 0);
			t_LinkSvrUtil->FreeServer();
		}

		do 
		{
			{
				tSID = t_EnvInfoOp->GetSvrLinkSID(0, nIFPort);
				if((SetER(t_LinkSvrUtil->StartServer(&tSID, NULL))))
				{
//					WriteLogE("socket open fail for agent link connect! : [errcode:%d]", g_nErrRtn);			
				}
				else
					break;
			}

			{
				nIFPort = t_EnvInfo->m_nIFPortApply;

				tSID = t_EnvInfoOp->GetSvrLinkSID(0, nIFPort);
				if((SetER(t_LinkSvrUtil->StartServer(&tSID, NULL))))
				{
//					WriteLogE("socket open fail for agent link connect! : [errcode:%d]", g_nErrRtn);			
				}
				else
					break;
			}

		} while (0);
		t_EnvInfo->SetReg_IFPortApply(nIFPort);

		WriteLogN("open socket for agent link success : [%u]", nIFPort);


		t_ThreadSocket->t_TimerUtil.SetNotifyWnd((HWND)this, 0);
		t_ThreadSocket->SetLogicStatus(SSA_SOCKET_THREAD_LOGIC_TYPE_SVR_LINK, SSA_SOCKET_THREAD_LOGIC_STATUS_BIT_ENABLE);
	}

	{
/*
		if(t_ASIFDLDLLUtil)
		{
			if(t_ASIFDLDLLUtil->RunDL())
			{
				WriteLogE("start file download thread result : fail");
			}
		}
*/
		if(RunDL())
		{
			WriteLogE("start file download thread result : fail");
		}

	}
/*
	{
		PDB_PO_HOST_RUN pdphr = (PDB_PO_HOST_RUN)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_HOST_RUN);
		if(pdphr)
		{
			if(pdphr->nLogReset & SS_PO_HOST_RUN_LOG_RESET_FLAG_DOCLOG)
			{
				WriteLogN("start reset(db) doc log by po host run");
				t_ManageLogDoc->ResetDocLog();
				WriteLogN("end reset(db) doc log by po host run");
			}
		}
	}
*/
	{
		Sleep(500);
	}	
	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::PreStartOperation()
{
	{
		t_LogicPWEvtMon->InitEnv();
//		t_FileDeleteUtil->InitEnv();
		t_ManageDevOInfo->CheckDvPolicy();
	}

	{
		t_LogicApplyPolicy->ApplyBootPolicy();
	}
	
	if(t_EnvInfoOp->IsSysBootTime())
	{
		t_LogicPWEvtMon->RunBootTimeOp();

		WriteLogN("apply sys boot.");
	}

	{
		INT32 nRetVal = make_utf8_file((char *)t_EnvInfo->m_strRootPath.c_str());
		if(nRetVal < 0)
			WriteLogE("fail to make utf8 file [%d]", nRetVal);
	}

	{
		RemoveInvenFiles();
	}

	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::PreStartTimer()
{
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_FA_CLEAR);
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_FA_OP_CLEAR);		
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_POWER);	
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_PTN_OP);
	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_PTN_SP_RULE);
//	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_RS_BK);
//	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_NC_PTN_OP);
//	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_NC_PTN_SP_RULE);

/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NPMS, 0, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_SCAN);	
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_ROLLBACK);	
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_PM_MON_RM_PATCH);	
		WriteLogN("[%s] enable npms operation timer..", __FUNCTION__);
	}
*/
/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_VULN_OP, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_READY_IN_DEPLOY);	
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_VULN_SCAN);	
		WriteLogN("[%s] enable noms vuln op operation timer..", __FUNCTION__);
	}
*/
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_CLASS_TYPE_NO_PTN, 0, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_INFO_PROC_REPUTATION);	
		WriteLogN("[%s] enable noms ptn operation timer..", __FUNCTION__);
	}

/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_CLASS_TYPE_NO_RS, 0, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_IN_RS_BK);
		WriteLogN("[%s] enable noms rs operation timer..", __FUNCTION__);
	}
*/
/*
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NCMS, 0, 0, 0))
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_INFO_PROC_REPUTATION);	
		WriteLogN("[%s] enable ncms ptn operation timer..", __FUNCTION__);
	}
*/
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_PTN_BL, 0))
	{
		t_LogicMgrPoInPtnBL->LoadPtn();
	}
	if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NOMS, SS_POLICY_TYPE_IN_PTN_WL, 0))
	{
		t_LogicMgrPoInPtnWL->LoadPtn();
	}
/*
	{
		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NCMS, SS_POLICY_TYPE_NC_PTN_LO, 0))
		{
			t_LogicMgrPoNcPtnLo->LoadPtn();
		}

		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NCMS, SS_POLICY_TYPE_NC_PTN_BL, 0))
		{
			t_LogicMgrPoNcPtnBL->LoadPtn();
		}

		if(t_ManageEnvLicense->IsValidRight(SS_PACKAGE_TYPE_NCMS, SS_POLICY_TYPE_NC_PTN_WL, 0))
		{
			t_LogicMgrPoNcPtnWL->LoadPtn();
		}
	}
*/
//	t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_CHECK_CHANGE_NET_DRIVE);

	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::StartOperation()
{
	{
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_WINDOWS_EVENT_HANDLER);
		t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_LOGIN_SVR_AUTH);
	}
	return 0;
}
//--------------------------------------------------------------------

/*
INT32		CMainDlg::StopProcUtil()
{
	{
//		CProcUtil tProcUtil;
//		tProcUtil.ProcessStop(STR_PROC_NAME_NANNY_MGR);
	}

	return 0;
}
*/
//--------------------------------------------------------------------

INT32		CMainDlg::StopSubClass()
{
	INT32 nRetVal = 0;
	if(t_EnvInfoOp)
	{
		t_EnvInfoOp->SetMainContinue();
	}

	{
		StopDL();
		//if(t_ASIFDLDLLUtil)		t_ASIFDLDLLUtil->StopDL();
//		if(t_ASIEPSAPPDLLUtil)	t_ASIEPSAPPDLLUtil->Stop();
	}

	if(t_LogicApplyPolicy)
	{
		t_LogicApplyPolicy->SetEPSHookPolicy(1);
	}
	
	{
		t_ThreadEvtMon->SetContinue(0);
		SetER(StopThread_Common(t_ThreadEvtMon));
		WriteLogN("stop [evt mon] thread result : [%d]", g_nErrRtn);

		t_ThreadTimer->SetContinue(0);
		SetER(StopThread_Common(t_ThreadTimer));
		WriteLogN("stop [timer] thread result : [%d]", g_nErrRtn);
/*
		t_ThreadSubTimer->SetContinue(0);
		SetER(StopThread_Common(t_ThreadSubTimer));
		WriteLogN("stop [timer_sub] thread result : [%d]", g_nErrRtn);
*/

		t_ThreadSocket->SetContinue(0);
		SetER(StopThread_Common(t_ThreadSocket));
		WriteLogN("stop [socket] thread result : [%d]", g_nErrRtn);

		t_ThreadLog->SetContinue(0);
		SetER(StopThread_Common(t_ThreadLog));
		WriteLogN("stop [log] thread result : [%d]", g_nErrRtn);

		t_ThreadPoFaClear->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoFaClear));
		WriteLogN("stop [po_fa_clear] thread result : [%d]", g_nErrRtn);
/*
		t_ThreadPoFaExPopup->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoFaExPopup));
		WriteLogN("stop [po_fa_ex_popup] thread result : [%d]", g_nErrRtn);
*/

		t_ThreadPoFePtnLo->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoFePtnLo));
		WriteLogN("stop [po_fe_ptn_lo] thread result : [%d]", g_nErrRtn);

		t_ThreadPoFeScan->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoFeScan));
		WriteLogN("stop [po_fe_scan] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInPtnScan->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInPtnScan));
		WriteLogN("stop [po_in_scan] thread result : [%d]", g_nErrRtn);

		t_ThreadPoFaOp->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoFaOp));
		WriteLogN("stop [po_fa_op] thread result : [%d]", g_nErrRtn);
/*
		t_ThreadPoInRsBk->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInRsBk));
		WriteLogN("stop [po_in_rs_bk] thread result : [%d]", g_nErrRtn);
*/
		t_ThreadChkHkNoti->SetContinue(0);
		SetER(StopThread_Common(t_ThreadChkHkNoti));
		WriteLogN("stop [chk_hk_noti] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInPtnFile->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInPtnFile));
		WriteLogN("stop [po_in_ptn_file] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInAccFile->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInAccFile));
		WriteLogN("stop [po_in_acc_file] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInCreateFile->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInCreateFile));
		WriteLogN("stop [po_in_create] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInCreateLog->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoInCreateLog));
		WriteLogN("stop [po_in_log] thread result : [%d]", g_nErrRtn);

		t_ThreadPoInSpReq->SetContinue(0);
		t_ThreadPoInSpReq->SendExitThreadCmd();
		SetER(StopThread_Common(t_ThreadPoInSpReq));
		WriteLogN("stop [po_in_sp_req] thread result : [%d]", g_nErrRtn);

/*
		t_ThreadExecute->SetContinue(0);
		SetER(StopThread_Common(t_ThreadExecute));
		WriteLogN("stop [execute] thread result : [%d]", g_nErrRtn);

		t_ThreadPoNcPtnLo->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoNcPtnLo));
		WriteLogN("stop [po_nc_ptn_lo] thread result : [%d]", g_nErrRtn);

		t_ThreadPoNcPtnScan->SetContinue(0);
		SetER(StopThread_Common(t_ThreadPoNcPtnScan));
		WriteLogN("stop [po_nc_scan] thread result : [%d]", g_nErrRtn);

*/
	}

	{
		if(t_ASIWENGDLLUtil)
		{
			t_ASIWENGDLLUtil->UnInitDB();
			t_ASIWENGDLLUtil->Free();
		}
	}

//	if(t_ManageDvDisable->Count() > 0)	t_ManageDvDisable->DevicesEnable();

	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::DeleteSubClass()
{
	{
		SAFE_DELETE(t_LogicLgnAuth);
		SAFE_DELETE(t_LogicUdtAuth);
		SAFE_DELETE(t_LogicMgrAuth);
	}

	{
		SAFE_DELETE(t_LogicMgrPtnPatch);
		SAFE_DELETE(t_LogicMgrPtnVuln);
	}

	{
		SAFE_DELETE(t_LogicMgrCtrlRemoteCtrl);
		SAFE_DELETE(t_LogicMgrCtrlRemotePower);
		SAFE_DELETE(t_LogicMgrCtrlRemoteOrder);
		SAFE_DELETE(t_LogicMgrCtrlInstantMsg);
		SAFE_DELETE(t_LogicMgrCtrlHostRes);
		SAFE_DELETE(t_LogicMgrCtrlRestore);
	}

	{
		SAFE_DELETE(t_LogicMgrEnvLicense);
		SAFE_DELETE(t_LogicMgrEnvLog);
		SAFE_DELETE(t_LogicMgrEnvLogUnit);
		SAFE_DELETE(t_LogicMgrEnvNotifyInfo);
		SAFE_DELETE(t_LogicMgrEnvSelfProtectAgt);
		SAFE_DELETE(t_LogicMgrEnvSOrgLink);
		SAFE_DELETE(t_LogicMgrEnvTrustSubject);
		SAFE_DELETE(t_LogicMgrEnvSocket);
	}

	{
		
		SAFE_DELETE(t_LogicMgrPoCtlPanel);
		SAFE_DELETE(t_LogicMgrPoDeployFile);
		SAFE_DELETE(t_LogicMgrPoPower);
		SAFE_DELETE(t_LogicMgrPoWinRun);

		SAFE_DELETE(t_LogicMgrPoHostRmInfo);
		SAFE_DELETE(t_LogicMgrPoHostRun);
		SAFE_DELETE(t_LogicMgrPoSvrInfoLgn);
		SAFE_DELETE(t_LogicMgrPoSvrInfoUdt);
		SAFE_DELETE(t_LogicMgrPoHostNotify);
//		SAFE_DELETE(t_LogicMgrPoHostWebSock);

		SAFE_DELETE(t_LogicMgrPoSch);

		SAFE_DELETE(t_LogicMgrPoCtlProc);
		SAFE_DELETE(t_LogicMgrPoSelfProtect);

		SAFE_DELETE(t_LogicMgrPoFaOp);
		SAFE_DELETE(t_LogicMgrPoFaClear);
		SAFE_DELETE(t_LogicMgrPoFaEnv);
		SAFE_DELETE(t_LogicMgrPoFaProc);
		SAFE_DELETE(t_LogicMgrPoFaBk);
		SAFE_DELETE(t_LogicMgrPoFaNotify);
		SAFE_DELETE(t_LogicMgrPoFaDelFileAfterBoot);
		SAFE_DELETE(t_LogicMgrPoFaInotifyFile);

		SAFE_DELETE(t_LogicMgrPoFePtnOp);
		SAFE_DELETE(t_LogicMgrPoFePtnLo);
		SAFE_DELETE(t_LogicMgrPoFePtnBL);
		SAFE_DELETE(t_LogicMgrPoFePtnWL);
		SAFE_DELETE(t_LogicMgrPoFeExclude);
		SAFE_DELETE(t_LogicMgrPoFeNotify);
		SAFE_DELETE(t_LogicMgrPoFeSinglePtn);
		
		{
			SAFE_DELETE(t_LogicMgrPoInPtnOp);
			SAFE_DELETE(t_LogicMgrPoInPtnBL);
			SAFE_DELETE(t_LogicMgrPoInPtnWL);
			SAFE_DELETE(t_LogicMgrPoInPtnEx);
			SAFE_DELETE(t_LogicMgrPoInPtnExTrust);
			SAFE_DELETE(t_LogicMgrPoInPtnSP);
			SAFE_DELETE(t_LogicMgrPoInPtnSPRule);
			SAFE_DELETE(t_LogicMgrPoInPtnNo);
						
			SAFE_DELETE(t_LogicMgrPoInVulnAx);
			SAFE_DELETE(t_LogicMgrPoInVulnEditApp);
			SAFE_DELETE(t_LogicMgrPoInVulnOp);
			SAFE_DELETE(t_LogicMgrPoInVulnQna);
			SAFE_DELETE(t_LogicMgrPoInVulnScan);
			SAFE_DELETE(t_LogicMgrPoInVulnSecuUsb);
			SAFE_DELETE(t_LogicMgrPoInVulnSw);
			SAFE_DELETE(t_LogicMgrPoInVulnPatchException);

			SAFE_DELETE(t_LogicMgrPoInRsOp);
			SAFE_DELETE(t_LogicMgrPoInRsBk);
			SAFE_DELETE(t_LogicMgrPoInRsNo);
			
			SAFE_DELETE(t_LogicMgrPoInDevOOp);
			SAFE_DELETE(t_LogicMgrPoInDevOBL);
			SAFE_DELETE(t_LogicMgrPoInDevOWL);
			SAFE_DELETE(t_LogicMgrPoInDevOEx);
			SAFE_DELETE(t_LogicMgrPoInDevOInfo);
			SAFE_DELETE(t_LogicMgrPoInDevONotify);
			SAFE_DELETE(t_LogicMgrPoDvLo);
		}

		SAFE_DELETE(t_LogicMgrPoPmOp);
		SAFE_DELETE(t_LogicMgrPoPmDm);
		SAFE_DELETE(t_LogicMgrPoPmScan);
		SAFE_DELETE(t_LogicMgrPoPmEx);
		SAFE_DELETE(t_LogicMgrPoPmNo);

//		SAFE_DELETE(t_LogicMgrPoNcPtnOp);
//		SAFE_DELETE(t_LogicMgrPoNcPtnLo);
//		SAFE_DELETE(t_LogicMgrPoNcPtnBL);
//		SAFE_DELETE(t_LogicMgrPoNcPtnWL);
//		SAFE_DELETE(t_LogicMgrPoNcPtnEx);
//		SAFE_DELETE(t_LogicMgrPoNcPtnExTrust);
//		SAFE_DELETE(t_LogicMgrPoNcPtnSP);
		SAFE_DELETE(t_LogicMgrPoNcPtnSPRule);
//		SAFE_DELETE(t_LogicMgrPoNcPtnNo);
	}

	{
		SAFE_DELETE(t_LogicMgrLogEvent);
		SAFE_DELETE(t_LogicMgrLogDeployFile);
		SAFE_DELETE(t_LogicMgrLogDoc);
		SAFE_DELETE(t_LogicMgrLogSecu);
		SAFE_DELETE(t_LogicMgrLogPatch);
		SAFE_DELETE(t_LogicMgrLogDevice);
		SAFE_DELETE(t_LogicMgrLogRs);
		SAFE_DELETE(t_LogicMgrLogRsBk);

		SAFE_DELETE(t_LogicMgrHost);
		SAFE_DELETE(t_LogicMgrHostSys);
		SAFE_DELETE(t_LogicMgrHostHw);
		SAFE_DELETE(t_LogicMgrHostSw);
		SAFE_DELETE(t_LogicMgrHostStatusPo);
//		SAFE_DELETE(t_LogicMgrHostStatusPoNc);
		SAFE_DELETE(t_LogicMgrHostKey);
		SAFE_DELETE(t_LogicMgrHostPatch);
		SAFE_DELETE(t_LogicMgrHostVulnRst);

		SAFE_DELETE(t_LogicMgrUser);

	}

	{
		SAFE_DELETE(t_LogicMgrSiteFile);
		SAFE_DELETE(t_LogicMgrSiteFileAlias);
		SAFE_DELETE(t_LogicMgrSiteVuln);
		SAFE_DELETE(t_LogicMgrSiteVulnScan);
//		SAFE_DELETE(t_LogicMgrInfoProcReputation);
	}

	{
		SAFE_DELETE(t_LogicAuth);
		SAFE_DELETE(t_LogicInitLink);
		SAFE_DELETE(t_LogicLogEvent);
		SAFE_DELETE(t_LogicLogDocDScan);
		SAFE_DELETE(t_LogicDocDeleteInfo);

		SAFE_DELETE(t_LogicCtrlVuln);
	}

	{
		SAFE_DELETE(t_LogicApplyPolicy);
	}

	{
		SAFE_DELETE(t_LogicPWEvtMon);
		SAFE_DELETE(t_LogicLocalEnvLog);
		SAFE_DELETE(t_LogicLocalEnvAuth);
	}

	{
		SAFE_DELETE(t_ManageBaseUtil);

		SAFE_DELETE(t_ManageHostPolicy);
		SAFE_DELETE(t_ManageUserPolicy);

		SAFE_DELETE(t_ManageErrorCode);

		SAFE_DELETE(t_ManageEnvPolicy);		
		SAFE_DELETE(t_ManageLinkLicense);
//		SAFE_DELETE(t_ManageEnvLocal);
	}

	{
		SAFE_DELETE(t_ThreadTimer);
//        SAFE_DELETE(t_ThreadSubTimer);		//TRACE("delete thread [%s]\n", "Sub Timer");
		SAFE_DELETE(t_ThreadSocket);
		SAFE_DELETE(t_ThreadLog);
		SAFE_DELETE(t_ThreadPoFaClear);
//        SAFE_DELETE(t_ThreadPoFaExPopup);	//TRACE("delete thread [%s]\n", "FaExPopup");
		SAFE_DELETE(t_ThreadPoFePtnLo);
		SAFE_DELETE(t_ThreadPoFeScan);
		SAFE_DELETE(t_ThreadPoInPtnScan);
		SAFE_DELETE(t_ThreadChkHkNoti);
		SAFE_DELETE(t_ThreadEvtMon);
		SAFE_DELETE(t_ThreadPoFaOp);
		SAFE_DELETE(t_ThreadPoInPtnFile);
		SAFE_DELETE(t_ThreadPoInAccFile);
		SAFE_DELETE(t_ThreadPoInCreateFile);
		SAFE_DELETE(t_ThreadPoInCreateLog);
		SAFE_DELETE(t_ThreadPoInSpReq);
//		SAFE_DELETE(t_ThreadPoInRsBk);		//TRACE("delete thread [%s]\n", "InRsBk");
//		SAFE_DELETE(t_ThreadExecute);		//TRACE("delete thread [%s]\n", "Execute");
//		SAFE_DELETE(t_ThreadPoNcPtnLo);		//TRACE("delete thread [%s]\n", "FePtnLo");
//		SAFE_DELETE(t_ThreadPoNcPtnScan);	//TRACE("delete thread [%s]\n", "InPtnScan");

	}

	{
		SAFE_DELETE(t_ManageFileDown);
		SAFE_DELETE(t_ManageLogicTimer);
//		SAFE_DELETE(t_ManageDeviceMedia);
		SAFE_DELETE(t_ManageDevOInfo);
//		SAFE_DELETE(t_ManagePMSWork);
		SAFE_DELETE(t_ManageLinkEnv);
//		SAFE_DELETE(t_ManagePatchFile);
//		SAFE_DELETE(t_ManageVulnFileProtect);
//		SAFE_DELETE(t_ManageVulnRegProtect);
//		SAFE_DELETE(t_ManageDelLastNotify);
//		SAFE_DELETE(t_ManageEpsRegInfo);
	}

	{
		SAFE_DELETE(t_ValidTimeUtil);
		SAFE_DELETE(t_DeployPolicyUtil);
		SAFE_DELETE(t_ExecuteFileUtil);
		SAFE_DELETE(t_DocBackupUtil);
		SAFE_DELETE(t_FileDeleteUtil);
		SAFE_DELETE(t_ComFuncUtil);
//		SAFE_DELETE(t_ExPopupUtil);
	}

	{
		SAFE_DELETE(t_EnvInfoOp);
		SAFE_DELETE(t_EnvInfo);
	}

	{
		SAFE_DELETE(t_LgnCltUtil);
		SAFE_DELETE(t_MgrCltUtil);
		SAFE_DELETE(t_UdtCltUtil);
		SAFE_DELETE(t_LinkSvrUtil);
	}

	{
//		SAFE_DELETE(t_ASIHTTPDLLUtil);
		SAFE_DELETE(t_ASICOMPDLLUtil);
		SAFE_DELETE(t_RemoveLogDLLUtil);
		SAFE_DELETE(t_ASIFFDLLUtil);
//		SAFE_DELETE(t_ASIFDLDLLUtil);
		SAFE_DELETE(t_NetworkDLLUtil);
		SAFE_DELETE(t_SeedDLLUtil);
//		SAFE_DELETE(t_ASIEPSAPPDLLUtil);
		SAFE_DELETE(t_ASISIDLLUtil);
		SAFE_DELETE(t_ProcInfoDLLUtil);
//		SAFE_DELETE(t_DevInfoDLLUtil);
//		SAFE_DELETE(t_ASIFWDLLUtil);
//		SAFE_DELETE(t_ASIDFFDLLUtil);
		SAFE_DELETE(t_ASIWENGDLLUtil);
//		SAFE_DELETE(t_ASIDVIDLLUtil);
//		SAFE_DELETE(t_ASIWRIDLLUtil);
//		SAFE_DELETE(t_ASIDBFQDLLUtil);
//		SAFE_DELETE(t_ASIBCDLLUtil);
//		SAFE_DELETE(t_ASIWebSockDLLUtil);
//		SAFE_DELETE(t_ASIPMSPatchUtil);
//		SAFE_DELETE(t_ASIPMSVulnUtil);
		//SAFE_DELETE(t_ASISYSCHECKDLLUtil);

	}

	{
		SAFE_DELETE(t_ResInfoDLLUtil);
	}
	{
		SAFE_DELETE(t_SystemInfo);
	}
	return 0;
}
//--------------------------------------------------------------------

INT32		CMainDlg::CheckIntegrity()
{
	return 0;
}
//--------------------------------------------------------------------
