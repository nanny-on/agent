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

#ifndef ManagePoFaOpUnitObjPkgH
#define ManagePoFaOpUnitObjPkgH
//---------------------------------------------------------------------------

class CManagePoFaOpUnitObjPkg : public CManagePoBase<DB_PO_FA_OP_UNIT_OBJ_PKG>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitPkg();
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoFaOpUnitObjPkg(DB_PO_FA_OP_UNIT_OBJ_PKG&	dpfouop);
    INT32					EditPoFaOpUnitObjPkg(DB_PO_FA_OP_UNIT_OBJ_PKG&	dpfouop);
    INT32					DelPoFaOpUnitObjPkg(UINT32 nID);
	INT32					ClearPoFaOpUnitObjPkg();
	INT32					GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID);

public:
	INT32					ClearItemByPolicyID(UINT32 nPolicyID);
	INT32					ClearPkgUnitByPolicyID(UINT32 nPolicyID);
	INT32					ClearPkgUnit();

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FA_OP_UNIT_OBJ_PKG pdpfouop, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FA_OP_UNIT_OBJ_PKG& dpfouop);

public:
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetInotifyObjPkg(UINT32 nPolID, UINT32 nExtOption, UINT32 nObjPkgID);

public:
	CManagePoFaOpUnitObjPkg();
    ~CManagePoFaOpUnitObjPkg();

};

extern CManagePoFaOpUnitObjPkg*	t_ManagePoFaOpUnitObjPkg;

#endif
