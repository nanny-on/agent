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


#ifndef ManagePoInDevOExH
#define ManagePoInDevOExH
//---------------------------------------------------------------------------

class CManagePoInDevOEx : public CManagePoBase<DB_PO_IN_DEVO_EX>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoInDevOEx(DB_PO_IN_DEVO_EX&	data);
    INT32					EditPoInDevOEx(DB_PO_IN_DEVO_EX&	data);
	INT32					DelPoInDevOEx(UINT32 nID);
	INT32					ApplyPoInDevOEx(DB_PO_IN_DEVO_EX&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_DEVO_EX pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_DEVO_EX& data);

public:
	CManagePoInDevOEx();
    ~CManagePoInDevOEx();

};

extern CManagePoInDevOEx*	t_ManagePoInDevOEx;

#endif
