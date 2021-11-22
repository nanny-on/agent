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


#ifndef ManagePoInDevOWLH
#define ManagePoInDevOWLH
//---------------------------------------------------------------------------

class CManagePoInDevOWL : public CManagePoBase<DB_PO_IN_DEVO_WL>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoInDevOWL(DB_PO_IN_DEVO_WL&	dpfn);
    INT32					EditPoInDevOWL(DB_PO_IN_DEVO_WL&	dpfn);
	INT32					DelPoInDevOWL(UINT32 nID);
	INT32					ApplyPoInDevOWL(DB_PO_IN_DEVO_WL&	dpfn);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_DEVO_WL pdpfn, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_WL& dpfn);

public:
	CManagePoInDevOWL();
    ~CManagePoInDevOWL();

};

extern CManagePoInDevOWL*	t_ManagePoInDevOWL;

#endif
