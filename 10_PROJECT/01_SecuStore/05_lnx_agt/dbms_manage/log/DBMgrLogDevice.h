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


#ifndef DBMgrLogDeviceH
#define DBMgrLogDeviceH
//---------------------------------------------------------------------------

class CDBMgrLogDevice : public CDBMgrBase
{
public:
	INT32			SetInitalize();

public:
	INT32			LoadDB(TListDBLogDevice& tDBLogDeviceList);

public:
    INT32			InsertLogDevice(DB_LOG_DEVICE& dld);
    INT32			UpdateLogDevice(DB_LOG_DEVICE& dld);
    INT32			DeleteLogDevice(UINT32 nID);    

public:
	virtual INT32	LoadExecute(PVOID lpTempletList);     
	virtual INT32	InsertExecute(PVOID lpTemplet);
	virtual INT32	UpdateExecute(PVOID lpTemplet);
	virtual INT32	DeleteExecute(UINT32 nID);

public:
	INT32			LoadDB(UINT32 nLogMode, UINT32 nLogNum, TListDBLogDevice& tDBLogDeviceList);


public:
	CDBMgrLogDevice();
    ~CDBMgrLogDevice();
};

extern CDBMgrLogDevice*		t_DBMgrLogDevice;

#endif
