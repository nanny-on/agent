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

#ifndef ManagePoFaClearUnitH
#define ManagePoFaClearUnitH
//---------------------------------------------------------------------------

class CManagePoFaClearUnit : public CManagePoBase<DB_PO_FA_CLEAR_UNIT>
{
public:
	INT32					LoadDBMS();

public:
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu);
    INT32					EditPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu);
	INT32					DelPoFaClearUnit(UINT32 nID);
	INT32					ClearPoFaClearUnit();
	INT32					ApplyPoFaClearUnit(DB_PO_FA_CLEAR_UNIT&	dpfcu);

public:
	String					GetName(UINT32 nID);
	INT32					GetLastChkTime(UINT32 nID);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FA_CLEAR_UNIT pdpfcu, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FA_CLEAR_UNIT& dpfcu);

	INT32					SetPkt_Link(MemToken& SendToken);
	INT32					SetPkt_Link(PDB_PO_FA_CLEAR_UNIT pdpfcu, MemToken& SendToken);

public:
	CManagePoFaClearUnit();
    ~CManagePoFaClearUnit();

};

extern CManagePoFaClearUnit*	t_ManagePoFaClearUnit;

#endif
