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

#ifndef ManagePoNcPtnSPH
#define ManagePoNcPtnSPH
//---------------------------------------------------------------------------

class CManagePoNcPtnSP : public CManagePoBase<DB_PO_NC_PTN_SP>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoNcPtnSP(DB_PO_NC_PTN_SP&	data);
    INT32					EditPoNcPtnSP(DB_PO_NC_PTN_SP&	data);
	INT32					DelPoNcPtnSP(UINT32 nID);
	INT32					ApplyPoNcPtnSP(DB_PO_NC_PTN_SP&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_NC_PTN_SP pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_SP& data);

public:
	CManagePoNcPtnSP();
    ~CManagePoNcPtnSP();

};

extern CManagePoNcPtnSP*	t_ManagePoNcPtnSP;

#endif
