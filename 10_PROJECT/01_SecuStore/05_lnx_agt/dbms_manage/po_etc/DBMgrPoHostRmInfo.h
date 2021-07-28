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

//---------------------------------------------------------------------------

#ifndef DBMgrPoHostRmInfoH
#define DBMgrPoHostRmInfoH
//---------------------------------------------------------------------------

class CDBMgrPoHostRmInfo : public CDBMgrBase
{
public:
	INT32			SetInitalize();

public:
	INT32			LoadDB(TListDBPoHostRmInfo& tDBPoHostRmInfoList);

public:
    INT32			InsertPoHostRmInfo(DB_PO_HOST_RM_INFO& dphri);
    INT32			UpdatePoHostRmInfo(DB_PO_HOST_RM_INFO& dphri);
    INT32			DeletePoHostRmInfo(UINT32 nID);    

public:
	virtual INT32	LoadExecute(PVOID lpTempletList);     
	virtual INT32	InsertExecute(PVOID lpTemplet);
	virtual INT32	UpdateExecute(PVOID lpTemplet);
	virtual INT32	DeleteExecute(UINT32 nID);


public:
	CDBMgrPoHostRmInfo();
    ~CDBMgrPoHostRmInfo();
};

extern CDBMgrPoHostRmInfo*		t_DBMgrPoHostRmInfo;

#endif
