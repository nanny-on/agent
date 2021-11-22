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

//---------------------------------------------------------------------------

#ifndef ManagePoFeSinglePtnH
#define ManagePoFeSinglePtnH
//---------------------------------------------------------------------------

class CManagePoFeSinglePtn : public CManagePoBase<DB_PO_FE_SINGLE_PTN>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp);
    INT32					EditPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp);
	INT32					DelPoFeSinglePtn(UINT32 nID);
	INT32					ApplyPoFeSinglePtn(DB_PO_FE_SINGLE_PTN&	dpfsp);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FE_SINGLE_PTN pdpfsp, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FE_SINGLE_PTN& dpfsp);

public:
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(PDB_PO_FE_SINGLE_PTN pdpfsp, MemToken& SendToken);

public:
	CManagePoFeSinglePtn();
    ~CManagePoFeSinglePtn();

};

extern CManagePoFeSinglePtn*	t_ManagePoFeSinglePtn;

#endif
