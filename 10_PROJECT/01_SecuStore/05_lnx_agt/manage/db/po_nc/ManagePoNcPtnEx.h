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

#ifndef ManagePoNcPtnExH
#define ManagePoNcPtnExH
//---------------------------------------------------------------------------

class CManagePoNcPtnEx : public CManagePoBase<DB_PO_NC_PTN_EX>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoNcPtnEx(DB_PO_NC_PTN_EX&	data);
    INT32					EditPoNcPtnEx(DB_PO_NC_PTN_EX&	data);
	INT32					DelPoNcPtnEx(UINT32 nID);
	INT32					ApplyPoNcPtnEx(DB_PO_NC_PTN_EX&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_NC_PTN_EX pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_NC_PTN_EX& data);

public:
	CManagePoNcPtnEx();
    ~CManagePoNcPtnEx();

};

extern CManagePoNcPtnEx*	t_ManagePoNcPtnEx;

#endif
