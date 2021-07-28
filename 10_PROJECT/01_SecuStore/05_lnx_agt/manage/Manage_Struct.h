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

#ifndef MEM_MGR_PKG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_MGR_PKG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_mgr_ptn_pkg
{
	_mem_mgr_ptn_pkg()
	{
		t_ManagePtnVersion				= NULL;

		t_ManagePtnPatch				= NULL;
		t_ManagePtnPatchFile			= NULL;
		t_ManagePtnPatchWsus			= NULL;
		t_ManagePtnPatchScan			= NULL;
		t_ManagePtnPatchScanEnv			= NULL;
		t_ManagePtnPatchScanEnvUnit		= NULL;
		t_ManagePtnDeploy				= NULL;
		t_ManagePtnVuln					= NULL;
		t_ManagePtnVulnScan				= NULL;
		t_ManagePtnVulnMP				= NULL;

		t_ManagePtnProcFile				= NULL;
	}

	~_mem_mgr_ptn_pkg()
	{

	}

	void _init()
	{
		t_ManagePtnVersion				= new CManagePtnVersion();

		t_ManagePtnPatch				= new CManagePtnPatch();
		t_ManagePtnPatchFile			= new CManagePtnPatchFile();
		t_ManagePtnPatchWsus			= new CManagePtnPatchWsus();
		t_ManagePtnPatchScan			= new CManagePtnPatchScan();
		t_ManagePtnPatchScanEnv			= new CManagePtnPatchScanEnv();
		t_ManagePtnPatchScanEnvUnit		= new CManagePtnPatchScanEnvUnit();
		t_ManagePtnDeploy				= new CManagePtnDeploy();
		t_ManagePtnVuln					= new CManagePtnVuln();
		t_ManagePtnVulnScan				= new CManagePtnVulnScan();
		t_ManagePtnVulnMP				= new CManagePtnVulnMP();

		t_ManagePtnProcFile				= new CManagePtnProcFile();
		return;
	}

	void _set_dbfq(CASIDBFQDLLUtil* tASIDBFQDLLUtil)
	{
		t_ManagePtnVersion->SetDBFQUtil(tASIDBFQDLLUtil);
		t_ManagePtnProcFile->SetDBFQUtil(tASIDBFQDLLUtil);

//		t_ManagePtnPatch->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnPatchFile->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnPatchWsus->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnPatchScan->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnPatchScanEnv->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnPatchScanEnvUnit->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnDeploy->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnVuln->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnVulnScan->SetDBFQUtil(tASIDBFQDLLUtil);
//		t_ManagePtnVulnMP->SetDBFQUtil(tASIDBFQDLLUtil);
	}


	void _free()
	{
		SAFE_DELETE(t_ManagePtnVersion);

		SAFE_DELETE(t_ManagePtnPatch);
		SAFE_DELETE(t_ManagePtnPatchFile);
		SAFE_DELETE(t_ManagePtnPatchWsus);
		SAFE_DELETE(t_ManagePtnPatchScan);
		SAFE_DELETE(t_ManagePtnPatchScanEnv);
		SAFE_DELETE(t_ManagePtnPatchScanEnvUnit);
		SAFE_DELETE(t_ManagePtnDeploy);
		SAFE_DELETE(t_ManagePtnVuln);
		SAFE_DELETE(t_ManagePtnVulnScan);
		SAFE_DELETE(t_ManagePtnVulnMP);

		SAFE_DELETE(t_ManagePtnProcFile);
		return;
	}

	void _clear()
	{
		t_ManagePtnVersion->Clear();

		t_ManagePtnPatch->Clear();
		t_ManagePtnPatchFile->Clear();
		t_ManagePtnPatchWsus->Clear();
		t_ManagePtnPatchScan->Clear();
		t_ManagePtnPatchScanEnv->Clear();
		t_ManagePtnPatchScanEnvUnit->Clear();
		t_ManagePtnDeploy->Clear();
		t_ManagePtnVuln->Clear();
		t_ManagePtnVulnScan->Clear();
		t_ManagePtnVulnMP->Clear();

		t_ManagePtnProcFile->Clear();
		return;
	}

	CManagePtnVersion*			t_ManagePtnVersion;

	CManagePtnPatch*			t_ManagePtnPatch;
	CManagePtnPatchFile*		t_ManagePtnPatchFile;
	CManagePtnPatchWsus*		t_ManagePtnPatchWsus;
	CManagePtnPatchScan*		t_ManagePtnPatchScan;
	CManagePtnPatchScanEnv*		t_ManagePtnPatchScanEnv;
	CManagePtnPatchScanEnvUnit*	t_ManagePtnPatchScanEnvUnit;
	CManagePtnDeploy*			t_ManagePtnDeploy;
	CManagePtnVuln*				t_ManagePtnVuln;
	CManagePtnVulnScan*			t_ManagePtnVulnScan;
	CManagePtnVulnMP*			t_ManagePtnVulnMP;

	CManagePtnProcFile*			t_ManagePtnProcFile;

}MEM_MGR_PTN_PKG, *PMEM_MGR_PTN_PKG;

extern PMEM_MGR_PTN_PKG		t_MMPPPatch;
extern PMEM_MGR_PTN_PKG		t_MMPPVuln;
extern PMEM_MGR_PTN_PKG		t_MMPPGWO;
extern PMEM_MGR_PTN_PKG		t_MMPPGBO;

extern PMEM_MGR_PTN_PKG		t_MMPPLWC;
extern PMEM_MGR_PTN_PKG		t_MMPPGWC;
extern PMEM_MGR_PTN_PKG		t_MMPPGBC;

#endif //MEM_MGR_PKG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



