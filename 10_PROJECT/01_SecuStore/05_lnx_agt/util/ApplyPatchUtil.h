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

#ifndef     _APPLY_PATCH_UTIL_H__
#define     _APPLY_PATCH_UTIL_H__


#define SS_APPLY_PATCH_FILE_TYPE_NONE					0
#define SS_APPLY_PATCH_FILE_TYPE_EXE					1
#define SS_APPLY_PATCH_FILE_TYPE_MSU					2
#define SS_APPLY_PATCH_FILE_TYPE_MSI					3
#define SS_APPLY_PATCH_FILE_TYPE_MSP					4
#define SS_APPLY_PATCH_FILE_TYPE_CAB					5


class CApplyPatchUtil
{

private:
//	CSystemInfo m_tSysInfo;

public:
	INT32		GetPatchFileType(String strFileName);
	String		GetLogFileName(String strFileName);
	String		GetLogFileNameForRB(UINT32 nKBID);
	String		GetCommandByExeType(String strFileName);

	INT32		MakeExecuteWork(MEM_WORK_EXECUTE& tMWE, String strFileName, String strCmd);
	INT32		MakeRollBackWork(MEM_WORK_EXECUTE& tMWE, PDB_HOST_PATCH pdata);	

public:
	INT32		IsApplyPatch(UINT32 nPatchID, UINT32 nPoPmScanUnitID);
	INT32		GetMSUPatchExecuteRst(String strLogFile);

	INT32		IsValidExecuteDISM(String strLogFile);
	INT32		IsValidExecutePkgMgr(String strLogFile);
	INT32		GetCabPkgMgrRst(String strLogFile);
	INT32		GetCabDISMRst(String strLogFile);

	INT32		GetCabMSIRst(String strLogFile);


public:
	CApplyPatchUtil(void);
	~CApplyPatchUtil(void);	
};

#endif /*_APPLY_PATCH_UTIL_H__*/