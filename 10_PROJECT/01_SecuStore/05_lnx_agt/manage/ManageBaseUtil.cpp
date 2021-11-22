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
#include "ManageBaseUtil.h"

//---------------------------------------------------------------------------

CManageBaseUtil*	t_ManageBaseUtil = NULL;

//---------------------------------------------------------------------------

CManageBaseUtil::CManageBaseUtil()
{	
}
//---------------------------------------------------------------------------

CManageBaseUtil::~CManageBaseUtil()
{	
}
//---------------------------------------------------------------------------

INT32		CManageBaseUtil::GetPktPolicy(PDB_POLICY pDP, MemToken& RecvToken)
{
	if(!RecvToken.TokenDel_32(pDP->nPUpdateID))					goto INVALID_PKT;		
	if(!RecvToken.TokenDel_32(pDP->nPSvrInfoLgnID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPHostRmInfoID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPHostRunID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPHostClearID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPPowerID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPCtlPanelID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPCtlProcID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPSelfProtectID))			goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPDeployFileID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFaOpID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFaClearID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFaProcID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFaEnvID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFaBkID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPWinRunID))					goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPSvrInfoUdtID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPSvrInfoMgrID))				goto INVALID_PKT;	

	if(!RecvToken.TokenDel_32(pDP->nPFePtnOpID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFePtnBLID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFePtnWLID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFeExcludeID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFeNotifyID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPFeSinglePtnID))			goto INVALID_PKT;		
	if(!RecvToken.TokenDel_32(pDP->nPFePtnLoID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPFaNotifyID))				goto INVALID_PKT;	
	if(!RecvToken.TokenDel_32(pDP->nPSchID))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPInPtnOpID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInPtnBLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInPtnWLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInPtnExID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInPtnSPID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInPtnNoID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nReservedA))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nReservedB))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nReservedC))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPInVulnOpID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnScanID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPHostNotifyID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPInDevOOpID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInDevOBLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInDevOWLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInDevOExID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPPmOpID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPPmDmID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPPmScanID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPPmExID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPPmNoID))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPInVulnAxID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnEditAppID))			goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnQnaID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnSecuUsbID))			goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnSwID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInVulnPatchExceptionID))	goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInDevONotifyID))			goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPInRsOpID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInRsNoID))					goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPInRsBkID))					goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPNcPtnOpID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnLoID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnBLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnWLID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnExID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnSPID))				goto INVALID_PKT;
	if(!RecvToken.TokenDel_32(pDP->nPNcPtnNoID))				goto INVALID_PKT;

	if(!RecvToken.TokenDel_32(pDP->nPHostWebSockID))			goto INVALID_PKT;


	RecvToken.TokenSkip_Block();

	return 0;

INVALID_PKT:
	return -1;
}
//---------------------------------------------------------------------------

INT32		CManageBaseUtil::SetPktPolicy(PDB_POLICY pDP, MemToken& SendToken)
{
	SendToken.TokenAdd_32(pDP->nPUpdateID);
	SendToken.TokenAdd_32(pDP->nPSvrInfoLgnID);
	SendToken.TokenAdd_32(pDP->nPHostRmInfoID);
	SendToken.TokenAdd_32(pDP->nPHostRunID);
	SendToken.TokenAdd_32(pDP->nPHostClearID);
	SendToken.TokenAdd_32(pDP->nPPowerID);
	SendToken.TokenAdd_32(pDP->nPCtlPanelID);
	SendToken.TokenAdd_32(pDP->nPCtlProcID);
	SendToken.TokenAdd_32(pDP->nPSelfProtectID);
	SendToken.TokenAdd_32(pDP->nPDeployFileID);
	SendToken.TokenAdd_32(pDP->nPFaOpID);
	SendToken.TokenAdd_32(pDP->nPFaClearID);
	SendToken.TokenAdd_32(pDP->nPFaProcID);
	SendToken.TokenAdd_32(pDP->nPFaEnvID);
	SendToken.TokenAdd_32(pDP->nPFaBkID);
	SendToken.TokenAdd_32(pDP->nPWinRunID);
	SendToken.TokenAdd_32(pDP->nPSvrInfoUdtID);
	SendToken.TokenAdd_32(pDP->nPSvrInfoMgrID);

	SendToken.TokenAdd_32(pDP->nPFePtnOpID);
	SendToken.TokenAdd_32(pDP->nPFePtnBLID);
	SendToken.TokenAdd_32(pDP->nPFePtnWLID);
	SendToken.TokenAdd_32(pDP->nPFeExcludeID);
	SendToken.TokenAdd_32(pDP->nPFeNotifyID);
	SendToken.TokenAdd_32(pDP->nPFeSinglePtnID);
	SendToken.TokenAdd_32(pDP->nPFePtnLoID);

	SendToken.TokenAdd_32(pDP->nPFaNotifyID);
	SendToken.TokenAdd_32(pDP->nPSchID);

	SendToken.TokenAdd_32(pDP->nPInPtnOpID);
	SendToken.TokenAdd_32(pDP->nPInPtnBLID);
	SendToken.TokenAdd_32(pDP->nPInPtnWLID);
	SendToken.TokenAdd_32(pDP->nPInPtnExID);
	SendToken.TokenAdd_32(pDP->nPInPtnSPID);
	SendToken.TokenAdd_32(pDP->nPInPtnNoID);

	SendToken.TokenAdd_32(pDP->nReservedA);
	SendToken.TokenAdd_32(pDP->nReservedB);
	SendToken.TokenAdd_32(pDP->nReservedC);

	SendToken.TokenAdd_32(pDP->nPInVulnOpID);
	SendToken.TokenAdd_32(pDP->nPInVulnScanID);

	SendToken.TokenAdd_32(pDP->nPHostNotifyID);

	SendToken.TokenAdd_32(pDP->nPInDevOOpID);
	SendToken.TokenAdd_32(pDP->nPInDevOBLID);
	SendToken.TokenAdd_32(pDP->nPInDevOWLID);
	SendToken.TokenAdd_32(pDP->nPInDevOExID);

	SendToken.TokenAdd_32(pDP->nPPmOpID);
	SendToken.TokenAdd_32(pDP->nPPmDmID);
	SendToken.TokenAdd_32(pDP->nPPmScanID);
	SendToken.TokenAdd_32(pDP->nPPmExID);
	SendToken.TokenAdd_32(pDP->nPPmNoID);

	SendToken.TokenAdd_32(pDP->nPInVulnAxID);
	SendToken.TokenAdd_32(pDP->nPInVulnEditAppID);
	SendToken.TokenAdd_32(pDP->nPInVulnQnaID);
	SendToken.TokenAdd_32(pDP->nPInVulnSecuUsbID);
	SendToken.TokenAdd_32(pDP->nPInVulnSwID);
	SendToken.TokenAdd_32(pDP->nPInVulnPatchExceptionID);
	SendToken.TokenAdd_32(pDP->nPInDevONotifyID);

	SendToken.TokenAdd_32(pDP->nPInRsOpID);
	SendToken.TokenAdd_32(pDP->nPInRsNoID);
	SendToken.TokenAdd_32(pDP->nPInRsBkID);

	SendToken.TokenAdd_32(pDP->nPNcPtnOpID);
	SendToken.TokenAdd_32(pDP->nPNcPtnLoID);
	SendToken.TokenAdd_32(pDP->nPNcPtnBLID);
	SendToken.TokenAdd_32(pDP->nPNcPtnWLID);
	SendToken.TokenAdd_32(pDP->nPNcPtnExID);
	SendToken.TokenAdd_32(pDP->nPNcPtnSPID);
	SendToken.TokenAdd_32(pDP->nPNcPtnNoID);

	SendToken.TokenAdd_32(pDP->nPHostWebSockID);

	SendToken.TokenSet_Block();

	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



