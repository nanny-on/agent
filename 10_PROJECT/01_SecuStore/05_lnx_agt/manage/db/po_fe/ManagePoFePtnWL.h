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

#ifndef ManagePoFePtnWLH
#define ManagePoFePtnWLH
//---------------------------------------------------------------------------

class CManagePoFePtnWL : public CManagePoBase<DB_PO_FE_PTN_WL>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw);
    INT32					EditPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw);
	INT32					DelPoFePtnWL(UINT32 nID);
	INT32					ApplyPoFePtnWL(DB_PO_FE_PTN_WL&	dpfpw);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					IsValidPtnFile(UINT32 nID); 
	INT32					IsValidPtnFile(PDB_PO_FE_PTN_WL pdpfpw);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FE_PTN_WL pdpfpw, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FE_PTN_WL& dpfpw);

public:
	CManagePoFePtnWL();
    ~CManagePoFePtnWL();

};

extern CManagePoFePtnWL*	t_ManagePoFePtnWL;

#endif
