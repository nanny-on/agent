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


#ifndef ManagePoPmDmUnitH
#define ManagePoPmDmUnitH
//---------------------------------------------------------------------------

class CManagePoPmDmUnit : public CManagePoBase<DB_PO_PM_DM_UNIT>
{
public:
	INT32					LoadDBMS();

public:	
	PDB_PO_PM_DM_UNIT		GetDefaultData();


public:
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoPmDmUnit(DB_PO_PM_DM_UNIT&	data);
    INT32					EditPoPmDmUnit(DB_PO_PM_DM_UNIT&	data);
	INT32					DelPoPmDmUnit(UINT32 nID);
	INT32					ApplyPoPmDmUnit(DB_PO_PM_DM_UNIT&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_PM_DM_UNIT pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_PM_DM_UNIT& data);

public:
	CManagePoPmDmUnit();
    ~CManagePoPmDmUnit();

};

extern CManagePoPmDmUnit*	t_ManagePoPmDmUnit;

#endif
