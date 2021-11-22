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

#ifndef ManagePoFaOpH
#define ManagePoFaOpH
//---------------------------------------------------------------------------

class CManagePoFaOp : public CManagePoBase<DB_PO_FA_OP>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoFaOp(DB_PO_FA_OP&	dpfp);
    INT32					EditPoFaOp(DB_PO_FA_OP&	dpfp);
	INT32					DelPoFaOp(UINT32 nID);
	INT32					ClearPoFaOp();
	INT32					ApplyPoFaOp(DB_PO_FA_OP&	dpfp);

public:
	String					GetName(UINT32 nID);    
	INT32					GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost_EPS(UINT32 nID, MemToken& SendToken);
	INT32					SetInotifyFaOp(UINT32 nID);
	INT32					SetPkt(PDB_PO_FA_OP pdpfp, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FA_OP& dpfp);

public:
	CManagePoFaOp();
    ~CManagePoFaOp();

};

extern CManagePoFaOp*	t_ManagePoFaOp;

#endif
