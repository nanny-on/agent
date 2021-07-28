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

#ifndef ManagePoSchPkgH
#define ManagePoSchPkgH
//---------------------------------------------------------------------------

class CManagePoSchPkg : public CManagePoBase<DB_PO_SCH_PKG>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitPkg();
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoSchPkg(DB_PO_SCH_PKG&	dphpsp);
	INT32					EditPoSchPkg(DB_PO_SCH_PKG&	dphpsp);
	INT32					DelPoSchPkg(UINT32 nID);

public:
	INT32					ClearItemByPolicyID(UINT32 nPolicyID);
	INT32					ClearPkgUnitByPolicyID(UINT32 nPolicyID);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_SCH_PKG pdphpsp, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_SCH_PKG& dphpsp);

public:
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);

public:
	CManagePoSchPkg();
    ~CManagePoSchPkg();

};

extern CManagePoSchPkg*	t_ManagePoSchPkg;

#endif
