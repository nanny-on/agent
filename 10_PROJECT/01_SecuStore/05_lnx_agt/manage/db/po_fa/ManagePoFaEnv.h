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

#ifndef ManagePoFaEnvH
#define ManagePoFaEnvH
//---------------------------------------------------------------------------

class CManagePoFaEnv : public CManagePoBase<DB_PO_FA_ENV>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoFaEnv(DB_PO_FA_ENV&	dpfe);
    INT32					EditPoFaEnv(DB_PO_FA_ENV&	dpfe);
	INT32					DelPoFaEnv(UINT32 nID);
	INT32					ClearPoFaEnv();
	INT32					ApplyPoFaEnv(DB_PO_FA_ENV&	dpfe);

public:
	String					GetName(UINT32 nID);
   	INT32					ChkMaxWorkTime(UINT32& nWorkTime);    
	INT32					GetHostSysTypePkgListID(TMapID tSubIDMap, TListID& tPkgIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost_EPS(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_FA_ENV pdpfe, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_FA_ENV& dpfe);

public:
	CManagePoFaEnv();
    ~CManagePoFaEnv();

};

extern CManagePoFaEnv*	t_ManagePoFaEnv;

#endif
