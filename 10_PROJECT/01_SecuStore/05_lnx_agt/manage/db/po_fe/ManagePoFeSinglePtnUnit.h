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

#ifndef ManagePoFeSinglePtnUnitH
#define ManagePoFeSinglePtnUnitH
//---------------------------------------------------------------------------

class CManagePoFeSinglePtnUnit : public CManagePoBase<DB_PO_FE_SINGLE_PTN_UNIT>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu);
    INT32					EditPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu);
	INT32					DelPoFeSinglePtnUnit(UINT32 nID);
	INT32					ApplyPoFeSinglePtnUnit(DB_PO_FE_SINGLE_PTN_UNIT&	dpfspu);

public:
	String					GetName(UINT32 nID);   

public:

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FE_SINGLE_PTN_UNIT pdpfspu, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FE_SINGLE_PTN_UNIT& dpfspu);

public:
	CManagePoFeSinglePtnUnit();
    ~CManagePoFeSinglePtnUnit();

};

extern CManagePoFeSinglePtnUnit*	t_ManagePoFeSinglePtnUnit;

#endif
