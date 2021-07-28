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


#ifndef	MANAGE_HEADER_H_12CC793F_06F3_411C_9419_61F453A83EDC
#define	MANAGE_HEADER_H_12CC793F_06F3_411C_9419_61F453A83EDC

#include "ManageBase.h"
#include "FManageBase.h"
//#include "ManageEdBase.h"
#include "ManagePoBase.h"

#include "ManageBaseUtil.h"

#include "ManageHost.h"
#include "ManageHostHw.h"
#include "ManageHostSys.h"
#include "ManageHostSw.h"
#include "ManageHostStatusPo.h"
#include "ManageHostStatusPoNc.h"
#include "ManageHostPolicy.h"
#include "ManageHostKey.h"
#include "ManageHostPatch.h"
#include "ManageHostVulnRst.h"
#include "ManageHostVulnRstUnit.h"

#include "ManageUser.h"
#include "ManageUserPolicy.h"

#include "ManageEnvLicense.h"
#include "ManageEnvLog.h"
#include "ManageEnvLogUnit.h"
#include "ManageEnvNotifyInfo.h"
#include "ManageEnvSelfProtectAgt.h"
#include "ManageEnvSOrgLink.h"
#include "ManageEnvTrustSubject.h"
#include "ManageEnvPolicy.h"
#include "ManageEnvLocal.h"
#include "ManageEnvSocket.h"

#include "ManageErrorCode.h"
#include "ManageOsType.h"
#include "ManageDocDeleteInfo.h"

#include "ManageSiteFile.h"
#include "ManageSiteFileAlias.h"
#include "ManageSiteVuln.h"
#include "ManageSiteVulnScan.h"
#include "ManageDocDeleteInfo.h"
#include "ManageInfoProcReputation.h"

#include "ManageLinkLicense.h"

#include "ManageLogDeployFile.h"
#include "ManageLogDoc.h"
#include "ManageLogEvent.h"
#include "ManageLogSecu.h"
#include "ManageLogDocDScan.h"
#include "ManageLogDocHost.h"
#include "ManageLogPatch.h"
#include "ManageLogDevice.h"
#include "ManageLogRs.h"
#include "ManageLogRsBk.h"

#include "ManagePoCtlPanel.h"
#include "ManagePoCtlPanelPkg.h"
#include "ManagePoCtlPanelUnit.h"
#include "ManagePoCtlProc.h"
#include "ManagePoCtlProcPkg.h"
#include "ManagePoCtlProcUnit.h"
#include "ManagePoDeployFile.h"
#include "ManagePoDeployFilePkg.h"
#include "ManagePoDeployFileUnit.h"
#include "ManagePoDeployFileUnitInfoPkg.h"
#include "ManagePoDeployFileUnitChkPkg.h"
#include "ManagePoDeployFileUnitTarPkg.h"
#include "ManagePoDeployFileUnitSchPkg.h"
#include "ManagePoDeployFileInfoUnit.h"
#include "ManagePoDeployFileChkUnit.h"
#include "ManagePoDeployFileTarUnit.h"
#include "ManagePoDeployFileSchUnit.h"
#include "ManagePoHostRmInfo.h"
#include "ManagePoHostRun.h"

#include "ManagePoSch.h"
#include "ManagePoSchPkg.h"
#include "ManagePoSchUnit.h"

#include "ManagePoHostNotify.h"
#include "ManagePoHostNotifyPkg.h"
#include "ManagePoHostNotifyUnit.h"
#include "ManagePoHostWebSock.h"

#include "ManagePoPower.h"
#include "ManagePoPowerPkg.h"
#include "ManagePoPowerUnit.h"
#include "ManagePoWinRun.h"
#include "ManagePoSelfProtect.h"
#include "ManagePoSelfProtectPkg.h"
#include "ManagePoSelfProtectUnit.h"
#include "ManagePoSvrInfoLgn.h"
#include "ManagePoSvrInfoUdt.h"
#include "ManagePoFaOp.h"
#include "ManagePoFaOpPkg.h"
#include "ManagePoFaOpUnit.h"
#include "ManagePoFaOpUnitObjPkg.h"
#include "ManagePoFaOpUnitSubPkg.h"
#include "ManagePoFaOpUnitSchPkg.h"
#include "ManagePoFaOpUnitRulPkg.h"
#include "ManagePoFaOpObjUnit.h"
#include "ManagePoFaOpSubUnit.h"
#include "ManagePoFaOpSchUnit.h"
#include "ManagePoFaClear.h"
#include "ManagePoFaClearPkg.h"
#include "ManagePoFaClearUnit.h"
#include "ManagePoFaProc.h"
#include "ManagePoFaProcPkg.h"
#include "ManagePoFaProcUnit.h"
#include "ManagePoFaEnv.h"
#include "ManagePoFaEnvPkg.h"
#include "ManagePoFaEnvUnit.h"
#include "ManagePoFaBk.h"
#include "ManagePoFaNotify.h"
#include "ManagePoFaNotifyPkg.h"
#include "ManagePoFaDelFileAfterBoot.h"
#include "ManagePoFaInotifyFile.h"

#include "ManagePoFePtnOp.h"
#include "ManagePoFePtnLo.h"
#include "ManagePoFePtnBL.h"
#include "ManagePoFePtnWL.h"
#include "ManagePoFeNotify.h"
#include "ManagePoFeNotifyPkg.h"
#include "ManagePoFeExclude.h"
#include "ManagePoFeExcludePkg.h"
#include "ManagePoFeExcludeUnit.h"
#include "ManagePoFeSinglePtn.h"
#include "ManagePoFeSinglePtnUnit.h"

#include "ManageLocalEnvLog.h"
#include "ManageLocalEnvAuth.h"

// Nanny-On Manage
#include "ManagePoInPtnOp.h"
#include "ManagePoInPtnOpPkg.h"
#include "ManagePoInPtnOpUnit.h"
#include "ManagePoInPtnBL.h"
#include "ManagePoInPtnBLPkg.h"
#include "ManagePoInPtnBLUnit.h"
#include "ManagePoInPtnWL.h"
#include "ManagePoInPtnWLPkg.h"
#include "ManagePoInPtnWLUnit.h"
#include "ManagePoInPtnEx.h"
#include "ManagePoInPtnExPkg.h"
#include "ManagePoInPtnExUnit.h"
#include "ManagePoInPtnExTrust.h"
#include "ManagePoInPtnSP.h"
#include "ManagePoInPtnSPPkg.h"
#include "ManagePoInPtnSPUnit.h"
#include "ManagePoInPtnSPRule.h"
#include "ManagePoInPtnNo.h"
#include "ManagePoInPtnNoPkg.h"

#include "ManagePoInVulnAx.h"
#include "ManagePoInVulnAxPkg.h"
#include "ManagePoInVulnAxUnit.h"
#include "ManagePoInVulnEditApp.h"
#include "ManagePoInVulnEditAppPkg.h"
#include "ManagePoInVulnEditAppUnit.h"
#include "ManagePoInVulnOp.h"
#include "ManagePoInVulnOpPkg.h"
#include "ManagePoInVulnQna.h"
#include "ManagePoInVulnQnaPkg.h"
#include "ManagePoInVulnQnaUnit.h"
#include "ManagePoInVulnScan.h"
#include "ManagePoInVulnScanPkg.h"
#include "ManagePoInVulnScanUnit.h"
#include "ManagePoInVulnScanUnitPkg.h"
#include "ManagePoInVulnSecuUsb.h"
#include "ManagePoInVulnSecuUsbPkg.h"
#include "ManagePoInVulnSecuUsbUnit.h"
#include "ManagePoInVulnSw.h"
#include "ManagePoInVulnSwPkg.h"
#include "ManagePoInVulnSwUnit.h"
#include "ManagePoInVulnPatchException.h"
#include "ManagePoInVulnPatchExceptionPkg.h"
#include "ManagePoInVulnPatchExceptionUnit.h"

#include "ManagePoInRsOp.h"
#include "ManagePoInRsOpPkg.h"
#include "ManagePoInRsOpUnit.h"
#include "ManagePoInRsOpUnitObjPkg.h"
#include "ManagePoInRsOpUnitSubPkg.h"
#include "ManagePoInRsOpObjUnit.h"
#include "ManagePoInRsOpSubUnit.h"
#include "ManagePoInRsNo.h"
#include "ManagePoInRsBk.h"
#include "ManagePoInRsNoPkg.h"


#include "ManagePoInDevOOp.h"
#include "ManagePoInDevOBL.h"
#include "ManagePoInDevOBLPkg.h"
#include "ManagePoInDevOWL.h"
#include "ManagePoInDevOWLPkg.h"
#include "ManagePoInDevOEx.h"
#include "ManagePoInDevOExPkg.h"
#include "ManagePoInDevOExUnit.h"
#include "ManagePoInDevOInfo.h"
#include "ManagePoInDevONotify.h"
#include "ManagePoInDevONotifyPkg.h"
#include "ManagePoDvDefault.h"

#include "ManagePoPmOp.h"
#include "ManagePoPmDm.h"
#include "ManagePoPmDmPkg.h"
#include "ManagePoPmDmUnit.h"
#include "ManagePoPmScan.h"
#include "ManagePoPmScanPkg.h"
#include "ManagePoPmScanUnit.h"
#include "ManagePoPmEx.h"
#include "ManagePoPmExPkg.h"
#include "ManagePoPmExUnit.h"
#include "ManagePoPmNo.h"
#include "ManagePoPmNoPkg.h"

#include "ManagePoNcPtnOp.h"
#include "ManagePoNcPtnOpPkg.h"
#include "ManagePoNcPtnOpUnit.h"
#include "ManagePoNcPtnLo.h"
#include "ManagePoNcPtnBL.h"
#include "ManagePoNcPtnBLPkg.h"
#include "ManagePoNcPtnBLUnit.h"
#include "ManagePoNcPtnWL.h"
#include "ManagePoNcPtnWLPkg.h"
#include "ManagePoNcPtnWLUnit.h"
#include "ManagePoNcPtnEx.h"
#include "ManagePoNcPtnExPkg.h"
#include "ManagePoNcPtnExUnit.h"
#include "ManagePoNcPtnExTrust.h"
#include "ManagePoNcPtnSP.h"
#include "ManagePoNcPtnSPPkg.h"
#include "ManagePoNcPtnSPUnit.h"
#include "ManagePoNcPtnSPRule.h"
#include "ManagePoNcPtnNo.h"
#include "ManagePoNcPtnNoPkg.h"



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include "ManageFileDown.h"
#include "ManageLogicTimer.h"
#include "ManageWinSession.h"
#include "ManageSysDrive.h"
#include "ManageEvtMon.h"
#include "ManageDeviceMedia.h"
#include "ManageDevOInfo.h"
#include "ManageDvDisable.h"
#include "ManagePMSWork.h"
#include "ManageLinkEnv.h"
#include "ManagePatchFile.h"
#include "ManageChkExistInterval.h"
#include "ManageVulnRegProtect.h"
#include "ManageVulnFileProtect.h"
#include "ManageVulnPreScan.h"
#include "ManageProcessInfo.h"
#include "ManageDelLastNotify.h"

#include "ManageEpsRegInfo.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "ManagePtnVersion.h"

#include "ManagePtnPatch.h"
#include "ManagePtnPatchFile.h"
#include "ManagePtnPatchWsus.h"
#include "ManagePtnPatchScan.h"
#include "ManagePtnPatchScanEnv.h"
#include "ManagePtnPatchScanEnvUnit.h"
#include "ManagePtnDeploy.h"
#include "ManagePtnVuln.h"
#include "ManagePtnVulnScan.h"
#include "ManagePtnVulnMP.h"
#include "ManagePtnProcFile.h"

// #include "ManagePtnPatchScan.h"
// #include "ManagePtnPatchEnv.h"
// #include "ManagePtnPatchCmd.h"
// #include "ManagePtnPatchUtil.h"
// #include "ManagePtnMimsPatch.h"

#include "Manage_Struct.h"

#endif //MANAGE_HEADER_H_12CC793F_06F3_411C_9419_61F453A83EDC
