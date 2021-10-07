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

#ifndef ManagePoInPtnBLH
#define ManagePoInPtnBLH
//---------------------------------------------------------------------------

class CManagePoInPtnBL : public CManagePoBase<DB_PO_IN_PTN_BL>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoInPtnBL(DB_PO_IN_PTN_BL&	data);
    INT32					EditPoInPtnBL(DB_PO_IN_PTN_BL&	data);
	INT32					DelPoInPtnBL(UINT32 nID);
	INT32					ApplyPoInPtnBL(DB_PO_IN_PTN_BL&	data);

public:
	String					GetName(UINT32 nID);   
	INT32					IsValidPtnFile(UINT32 nID);
	INT32					IsValidPtnFile(PDB_PO_IN_PTN_BL pdata);


public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_PTN_BL pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_PTN_BL& data);
	UINT32					GetUsedMode(UINT32 nID = 0);
public:
	CManagePoInPtnBL();
    ~CManagePoInPtnBL();

};

extern CManagePoInPtnBL*	t_ManagePoInPtnBL;

#endif
