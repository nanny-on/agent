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

#ifndef DB_POLICY_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_POLICY_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_policy
{
	_db_policy()
	{
		nPUpdateID				= 0;
		nPSvrInfoLgnID			= 0;
		nPHostRmInfoID			= 0;
		nPHostRunID				= 0;
		nPHostClearID			= 0;
		nPPowerID				= 0;
		nPCtlPanelID			= 0;
		nPCtlProcID				= 0;
		nPSelfProtectID			= 0;
		nPDeployFileID			= 0;
		nPFaOpID				= 0;
		nPFaClearID				= 0;
		nPFaProcID				= 0;
		nPFaEnvID				= 0;
		nPFaBkID				= 0;
		nPWinRunID				= 0;
		nPSvrInfoUdtID			= 0;
		nPSvrInfoMgrID			= 0;
		nPFePtnOpID				= 0;
		nPFePtnBLID				= 0;
		nPFePtnWLID				= 0;
		nPFeExcludeID			= 0;
		nPFeNotifyID			= 0;
		nPFeSinglePtnID			= 0;
		nPFePtnLoID				= 0;

		nPFaNotifyID			= 0;
		nPSchID					= 0;

		nPInPtnOpID				= 0;
		nPInPtnBLID				= 0;
		nPInPtnWLID				= 0;
		nPInPtnExID				= 0;
		nPInPtnSPID				= 0;
		nPInPtnNoID				= 0;	

		nReservedA				= 0;
		nReservedB				= 0;
		nReservedC				= 0;			

		nPInVulnOpID			= 0;
		nPInVulnScanID			= 0;

		nPHostNotifyID			= 0;

		nPInDevOOpID			= 0;
		nPInDevOBLID			= 0;
		nPInDevOWLID			= 0;
		nPInDevOExID			= 0;
		nPInDevONotifyID		= 0;

		nPPmOpID				= 0;
		nPPmDmID				= 0;
		nPPmScanID				= 0;
		nPPmExID				= 0;
		nPPmNoID				= 0;

		nPInVulnAxID			= 0;
		nPInVulnEditAppID		= 0;
		nPInVulnQnaID			= 0;
		nPInVulnSecuUsbID		= 0;
		nPInVulnSwID			= 0;
		nPInVulnPatchExceptionID= 0;

		nPInRsOpID				= 0;
		nPInRsNoID				= 0;
		nPInRsBkID				= 0;

		nPNcPtnOpID				= 0;
		nPNcPtnLoID				= 0;
		nPNcPtnBLID				= 0;
		nPNcPtnWLID				= 0;
		nPNcPtnExID				= 0;
		nPNcPtnSPID				= 0;
		nPNcPtnNoID				= 0;

		nPHostWebSockID			= 0;
		nPSvrInfoFBID			= 0;
		nPCtlShareFileID		= 0;
	}

	UINT32		nPUpdateID;
	UINT32		nPSvrInfoLgnID;
	UINT32		nPHostRmInfoID;
	UINT32		nPHostRunID;
	UINT32		nPHostClearID;
	UINT32		nPPowerID;
	UINT32		nPCtlPanelID;
	UINT32		nPCtlProcID;
	UINT32		nPSelfProtectID;
	UINT32		nPDeployFileID;
	UINT32		nPFaOpID;
	UINT32		nPFaClearID;
	UINT32		nPFaProcID;
	UINT32		nPFaEnvID;
	UINT32		nPFaBkID;
	UINT32		nPWinRunID;
	UINT32		nPSvrInfoUdtID;
	UINT32		nPSvrInfoMgrID;
	UINT32		nPFePtnOpID;
	UINT32		nPFePtnBLID;
	UINT32		nPFePtnWLID;
	UINT32		nPFeExcludeID;
	UINT32		nPFeNotifyID;
	UINT32		nPFeSinglePtnID;
	UINT32		nPFePtnLoID;

	UINT32		nPFaNotifyID;
	UINT32		nPSchID;

	UINT32		nPInPtnOpID;
	UINT32		nPInPtnBLID;
	UINT32		nPInPtnWLID;
	UINT32		nPInPtnExID;
	UINT32		nPInPtnSPID;
	UINT32		nPInPtnNoID;

	UINT32		nReservedA;
	UINT32		nReservedB;
	UINT32		nReservedC;

	UINT32		nPInVulnOpID;
	UINT32		nPInVulnScanID;

	UINT32		nPHostNotifyID;

	UINT32		nPInDevOOpID;
	UINT32		nPInDevOBLID;
	UINT32		nPInDevOWLID;
	UINT32		nPInDevOExID;
	UINT32		nPInDevONotifyID;

	UINT32		nPPmOpID;
	UINT32		nPPmDmID;
	UINT32		nPPmScanID;
	UINT32		nPPmExID;
	UINT32		nPPmNoID;

	UINT32		nPInVulnAxID;
	UINT32		nPInVulnEditAppID;
	UINT32		nPInVulnQnaID;
	UINT32		nPInVulnSecuUsbID;
	UINT32		nPInVulnSwID;
	UINT32		nPInVulnPatchExceptionID;

	UINT32		nPInRsOpID;
	UINT32		nPInRsNoID;
	UINT32		nPInRsBkID;

	UINT32		nPNcPtnOpID;
	UINT32		nPNcPtnLoID;
	UINT32		nPNcPtnBLID;
	UINT32		nPNcPtnWLID;
	UINT32		nPNcPtnExID;
	UINT32		nPNcPtnSPID;
	UINT32		nPNcPtnNoID;

	UINT32		nPHostWebSockID;

	UINT32		nPSvrInfoFBID;
	UINT32		nPCtlShareFileID;
	
}DB_POLICY, *PDB_POLICY;

#endif //DB_POLICY_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



