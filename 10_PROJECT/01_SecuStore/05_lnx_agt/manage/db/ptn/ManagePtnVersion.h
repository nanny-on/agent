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

#ifndef ManagePtnVersionH
#define ManagePtnVersionH
//---------------------------------------------------------------------------

class CManagePtnVersion : public CFManageBase<DB_PTN_VERSION>
{
public:
	CDBMgrPtnVersion*	m_tDBMgrPtnVersion;

public:
	INT32			LoadDBMS();

public:
	void			SetDBFQUtil(CASIDBFQDLLUtil*	tASIDBFQDLLUtil);

public:
	INT32			AddPtnVersion(DB_PTN_VERSION& data);
    INT32			EditPtnVersion(DB_PTN_VERSION& data);
    INT32			DelPtnVersion(UINT32 nID);
	String			GetDefaultVersion();

public:
	INT32			GetPkt(MemToken& RecvToken, DB_PTN_VERSION& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_PTN_VERSION pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManagePtnVersion();
    ~CManagePtnVersion();

};

#endif
