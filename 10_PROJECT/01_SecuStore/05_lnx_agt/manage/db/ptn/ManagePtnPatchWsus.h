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

#ifndef ManagePtnPatchWsusH
#define ManagePtnPatchWsusH
//---------------------------------------------------------------------------

class CManagePtnPatchWsus : public CManageBase<DB_PTN_PATCH_WSUS>
{

private:
	CDBMgrPtnPatchWsus*	m_tDBMgrPtnPatchWsus;

public:
	INT32			LoadDBMS();

public:
	INT32				AddPtnPatchWsus(DB_PTN_PATCH_WSUS& data);
    INT32				EditPtnPatchWsus(DB_PTN_PATCH_WSUS& data);
    INT32				DelPtnPatchWsus(UINT32 nID);
	PDB_PTN_PATCH_WSUS	GetPtnPatchWsus(UINT64 nSysID, UINT32 nSpID);
	INT32				IsValidPtnFile(PDB_PTN_PATCH_WSUS pdata, UINT32 nDnType);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_PTN_PATCH_WSUS& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_PTN_PATCH_WSUS pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManagePtnPatchWsus();
    ~CManagePtnPatchWsus();

};

#endif
