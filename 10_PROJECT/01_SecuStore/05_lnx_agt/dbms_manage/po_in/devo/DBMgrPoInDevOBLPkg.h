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


#ifndef DBMgrPoInDevOBLPkgH
#define DBMgrPoInDevOBLPkgH
//---------------------------------------------------------------------------

class CDBMgrPoInDevOBLPkg : public CDBMgrBase
{
public:
	INT32			SetInitalize();

public:
	INT32			LoadDB(TListDBPoInDevOBLPkg& tDBPoInDevOBLPkgList);

public:
    INT32			InsertPoInDevOBLPkg(DB_PO_IN_DEVO_BL_PKG& dpfnp);
    INT32			UpdatePoInDevOBLPkg(DB_PO_IN_DEVO_BL_PKG& dpfnp);

public:
	virtual INT32	LoadExecute(PVOID lpTempletList);     
	virtual INT32	InsertExecute(PVOID lpTemplet);
	virtual INT32	UpdateExecute(PVOID lpTemplet);
	virtual INT32	DeleteExecute(UINT32 nID);


public:
	CDBMgrPoInDevOBLPkg();
    ~CDBMgrPoInDevOBLPkg();
};

extern CDBMgrPoInDevOBLPkg*		t_DBMgrPoInDevOBLPkg;

#endif
