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

#ifndef ManagePtnProcFileH
#define ManagePtnProcFileH
//---------------------------------------------------------------------------

class CManagePtnProcFile : public CFManageBase<DB_PTN_PROC_FILE>
{
private:
	CDBMgrPtnProcFile*	m_tDBMgrPtnProcFile;

public:
	void			SetDBFQUtil(CASIDBFQDLLUtil*	tASIDBFQDLLUtil);

public:
	INT32			LoadDBMS();

public:
	INT32			AddPtnProcFile(DB_PTN_PROC_FILE& data);
    INT32			EditPtnProcFile(DB_PTN_PROC_FILE& data);
    INT32			DelPtnProcFile(UINT32 nID);

public:
	INT32			GetInitPktList(UINT64 nKey, TListDBPtnProcFile& tInitPktList, UINT32 nOnceNum);
	INT32			SetPkt(MemToken& SendToken, UINT64 nKey, UINT32 nOnceNum);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_PTN_PROC_FILE& data);
	INT32			SetPkt(MemToken& SendToken);
	INT32			SetPkt(TListID& tIDList, MemToken& SendToken);
    INT32			SetPkt(PDB_PTN_PROC_FILE pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManagePtnProcFile();
    ~CManagePtnProcFile();

};

#endif
