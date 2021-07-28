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

#ifndef ManagePtnPatchScanEnvUnitH
#define ManagePtnPatchScanEnvUnitH
//---------------------------------------------------------------------------

class CManagePtnPatchScanEnvUnit : public CManageBase<DB_PTN_PATCH_SCAN_ENV_UNIT>
{

private:
	CDBMgrPtnPatchScanEnvUnit*	m_tDBMgrPtnPatchScanEnvUnit;

public:
	INT32			LoadDBMS();

public:
	INT32				AddPtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT& data);
    INT32				EditPtnPatchScanEnvUnit(DB_PTN_PATCH_SCAN_ENV_UNIT& data);
    INT32				DelPtnPatchScanEnvUnit(UINT32 nID);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_PTN_PATCH_SCAN_ENV_UNIT& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_PTN_PATCH_SCAN_ENV_UNIT pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManagePtnPatchScanEnvUnit();
    ~CManagePtnPatchScanEnvUnit();

};

#endif
