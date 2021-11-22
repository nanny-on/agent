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


#ifndef	LOGIC_HEADER_H_35E2DC5F_18B1_42E7_B2B0_1CD01894D161
#define	LOGIC_HEADER_H_35E2DC5F_18B1_42E7_B2B0_1CD01894D161
                          

//------------------------------------------------------------------------------
//link svr

#include	"LogicAuth.h"
#include	"LogicInitLink.h"
#include	"LogicLogDeployFile.h"
#include	"LogicLogDoc.h"
#include	"LogicLogEvent.h"
#include	"LogicLogSecu.h"
#include	"LogicLogPatch.h"
#include	"LogicLogDevice.h"
#include	"LogicLogRs.h"
#include	"LogicLogRsBk.h"

#include	"LogicSiteFile.h"
#include	"LogicSiteVuln.h"
#include	"LogicSiteVulnScan.h"
#include	"LogicInfoProcReputation.h"

#include	"LogicPoFaClear.h"
#include	"LogicLogDocDScan.h"
#include	"LogicUser.h"
#include	"LogicPoInPtnSPRule.h"
#include	"LogicPoInPtnExTrust.h"
#include	"LogicPoNcPtnSPRule.h"
#include	"LogicPoNcPtnLoPtn.h"
#include	"LogicPoNcPtnExTrust.h"

#include	"LogicEnvNotifyInfo.h"

#include	"LogicPtnPatch.h"
#include	"LogicPtnVuln.h"
#include	"LogicHostVulnRst.h"

#include	"LogicCtrlVuln.h"

//------------------------------------------------------------------------------

//agent mgr svr
#include	"LogicMgrAuth.h"
#include	"LogicMgrHost.h"
#include	"LogicMgrHostHw.h"
#include	"LogicMgrHostSw.h"
#include	"LogicMgrHostStatusPo.h"
#include	"LogicMgrHostStatusPoNc.h"
#include	"LogicMgrHostSys.h"
#include	"LogicMgrHostKey.h"
#include	"LogicMgrHostPatch.h"
#include	"LogicMgrHostVulnRst.h"

#include	"LogicMgrUser.h"

#include	"LogicMgrSiteFile.h"
#include	"LogicMgrSiteFileAlias.h"
#include	"LogicMgrSiteVuln.h"
#include	"LogicMgrSiteVulnScan.h"
#include	"LogicMgrInfoProcReputation.h"

#include	"LogicMgrPoCtlPanel.h"
#include	"LogicMgrPoCtlProc.h"
#include	"LogicMgrPoDeployFile.h"
#include	"LogicMgrPoPower.h"
#include	"LogicMgrPoWinRun.h"

#include	"LogicMgrPoHostRmInfo.h"
#include	"LogicMgrPoHostRun.h"
#include	"LogicMgrPoSelfProtect.h"
#include	"LogicMgrPoSvrInfoUdt.h"
#include	"LogicMgrPoSvrInfoLgn.h"
#include	"LogicMgrPoHostNotify.h"
#include	"LogicMgrPoHostWebSock.h"

#include	"LogicMgrPoSch.h"

#include	"LogicMgrPoFaEnv.h"
#include	"LogicMgrPoFaOp.h"
#include	"LogicMgrPoFaProc.h"
#include	"LogicMgrPoFaClear.h"
#include	"LogicMgrPoFaBk.h"
#include	"LogicMgrPoFaNotify.h"
#include	"LogicMgrPoFaDelFileAfterBoot.h"
#include	"LogicMgrPoFaInotifyFile.h"

#include	"LogicMgrPoFePtnOp.h"
#include	"LogicMgrPoFePtnLo.h"
#include	"LogicMgrPoFePtnBL.h"
#include	"LogicMgrPoFePtnWL.h"
#include	"LogicMgrPoFeNotify.h"
#include	"LogicMgrPoFeExclude.h"
#include	"LogicMgrPoFeSinglePtn.h"

#include	"LogicMgrPoInPtnOp.h"
#include	"LogicMgrPoInPtnBL.h"
#include	"LogicMgrPoInPtnWL.h"
#include	"LogicMgrPoInPtnEx.h"
#include	"LogicMgrPoInPtnExTrust.h"
#include	"LogicMgrPoInPtnSP.h"
#include	"LogicMgrPoInPtnSPRule.h"
#include	"LogicMgrPoInPtnNo.h"

#include	"LogicMgrPoInDevOOp.h"
#include	"LogicMgrPoInDevOBL.h"
#include	"LogicMgrPoInDevOWL.h"
#include	"LogicMgrPoInDevOEx.h"
#include	"LogicMgrPoInDevOInfo.h"
#include	"LogicMgrPoInDevONotify.h"
#include	"LogicMgrPoDvLo.h"

#include	"LogicMgrPoInVulnAx.h"
#include	"LogicMgrPoInVulnEditApp.h"
#include	"LogicMgrPoInVulnOp.h"
#include	"LogicMgrPoInVulnQna.h"
#include	"LogicMgrPoInVulnScan.h"
#include	"LogicMgrPoInVulnSecuUsb.h"
#include	"LogicMgrPoInVulnSw.h"
#include	"LogicMgrPoInVulnPatchException.h"

#include	"LogicMgrPoInRsOp.h"
#include	"LogicMgrPoInRsBk.h"
#include	"LogicMgrPoInRsNo.h"

#include	"LogicMgrPoPmOp.h"
#include	"LogicMgrPoPmDm.h"
#include	"LogicMgrPoPmScan.h"
#include	"LogicMgrPoPmEx.h"
#include	"LogicMgrPoPmNo.h"

#include	"LogicMgrPoNcPtnOp.h"
#include	"LogicMgrPoNcPtnLo.h"
#include	"LogicMgrPoNcPtnBL.h"
#include	"LogicMgrPoNcPtnWL.h"
#include	"LogicMgrPoNcPtnEx.h"
#include	"LogicMgrPoNcPtnExTrust.h"
#include	"LogicMgrPoNcPtnSP.h"
#include	"LogicMgrPoNcPtnSPRule.h"
#include	"LogicMgrPoNcPtnNo.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


#include	"LogicMgrEnvLicense.h"
#include	"LogicMgrEnvLog.h"
#include	"LogicMgrEnvLogUnit.h"
#include	"LogicMgrEnvNotifyInfo.h"
#include	"LogicMgrEnvSelfProtectAgt.h"
#include	"LogicMgrEnvSOrgLink.h"
#include	"LogicMgrEnvTrustSubject.h"
#include	"LogicMgrEnvSocket.h"

#include	"LogicMgrLogEvent.h"
#include	"LogicMgrLogDeployFile.h"
#include	"LogicMgrLogDoc.h"
#include	"LogicMgrLogSecu.h"
#include	"LogicMgrLogPatch.h"
#include	"LogicMgrLogDevice.h"
#include	"LogicMgrLogRs.h"
#include	"LogicMgrLogRsBk.h"

#include	"LogicMgrCtrlRemoteCtrl.h"
#include	"LogicMgrCtrlRemotePower.h"
#include	"LogicMgrCtrlInstantMsg.h"
#include	"LogicMgrCtrlHostRes.h"
#include	"LogicMgrCtrlRestore.h"
#include	"LogicMgrCtrlRemoteOrder.h"

#include	"LogicMgrPtnPatch.h"
#include	"LogicMgrPtnVuln.h"
#include	"LogicMgrPtnGWO.h"
#include	"LogicMgrPtnGBO.h"
#include	"LogicMgrPtnLWC.h"
#include	"LogicMgrPtnGWC.h"
#include	"LogicMgrPtnGBC.h"
// 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#include	"LogicApplyPolicy.h"
//------------------------------------------------------------------------------

#include	"LogicPWEvtMon.h"
#include	"LogicLocalEnvLog.h"
#include	"LogicLocalEnvAuth.h"
#include	"LogicDocDeleteInfo.h"
//------------------------------------------------------------------------------

//agent lgn svr
#include	"LogicLgnAuth.h"

//------------------------------------------------------------------------------

//agent udt svr
#include	"LogicUdtAuth.h"

//------------------------------------------------------------------------------

#endif	//LOGIC_HEADER_H_35E2DC5F_18B1_42E7_B2B0_1CD01894D161
