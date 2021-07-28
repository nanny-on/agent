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


#ifndef ManagePoPmScanUnitH
#define ManagePoPmScanUnitH
//---------------------------------------------------------------------------

class CManagePoPmScanUnit : public CManagePoBase<DB_PO_PM_SCAN_UNIT>
{
public:
	INT32					LoadDBMS();

public:	
	PDB_PO_PM_SCAN_UNIT		GetDefaultData();


public:
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data);
    INT32					EditPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data);
	INT32					DelPoPmScanUnit(UINT32 nID);
	INT32					ApplyPoPmScanUnit(DB_PO_PM_SCAN_UNIT&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_PM_SCAN_UNIT pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_PM_SCAN_UNIT& data);

public:
	CManagePoPmScanUnit();
    ~CManagePoPmScanUnit();

};

extern CManagePoPmScanUnit*	t_ManagePoPmScanUnit;

#endif
