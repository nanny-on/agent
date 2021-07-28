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


#ifndef ManagePoPmNoH
#define ManagePoPmNoH
//---------------------------------------------------------------------------

class CManagePoPmNo : public CManagePoBase<DB_PO_PM_NO>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoPmNo(DB_PO_PM_NO&	data);
    INT32					EditPoPmNo(DB_PO_PM_NO&	data);
	INT32					DelPoPmNo(UINT32 nID);
	INT32					ApplyPoPmNo(DB_PO_PM_NO&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_PM_NO pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_PM_NO& data);

public:
	CManagePoPmNo();
    ~CManagePoPmNo();

};

extern CManagePoPmNo*	t_ManagePoPmNo;

#endif
