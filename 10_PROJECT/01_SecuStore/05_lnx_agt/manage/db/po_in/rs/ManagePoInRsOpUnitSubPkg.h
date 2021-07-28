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


#ifndef ManagePoInRsOpUnitSubPkgH
#define ManagePoInRsOpUnitSubPkgH
//---------------------------------------------------------------------------

class CManagePoInRsOpUnitSubPkg : public CManagePoBase<DB_PO_IN_RS_OP_UNIT_SUB_PKG>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitPkg();
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoInRsOpUnitSubPkg(DB_PO_IN_RS_OP_UNIT_SUB_PKG&	dpfousp);
    INT32					EditPoInRsOpUnitSubPkg(DB_PO_IN_RS_OP_UNIT_SUB_PKG&	dpfousp);
	INT32					DelPoInRsOpUnitSubPkg(UINT32 nID);

	INT32					GetPoIDListFromUnitPkg(UINT32 nUnitID, TListID& tListID);

public:
	INT32					ClearItemByPolicyID(UINT32 nPolicyID);
	INT32					ClearPkgUnitByPolicyID(UINT32 nPolicyID);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_RS_OP_UNIT_SUB_PKG pdpfousp, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_RS_OP_UNIT_SUB_PKG& dpfousp);

public:
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);

public:
	CManagePoInRsOpUnitSubPkg();
    ~CManagePoInRsOpUnitSubPkg();

};

extern CManagePoInRsOpUnitSubPkg*	t_ManagePoInRsOpUnitSubPkg;

#endif
