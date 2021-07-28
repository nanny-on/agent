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

#ifndef ManagePoHostRunH
#define ManagePoHostRunH
//---------------------------------------------------------------------------

class CManagePoHostRun : public CManagePoBase<DB_PO_HOST_RUN>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoHostRun(DB_PO_HOST_RUN&	dphr);
    INT32					EditPoHostRun(DB_PO_HOST_RUN&	dphr);
	INT32					DelPoHostRun(UINT32 nID);
	INT32					ClearPoHostRun();
	INT32					ApplyPoHostRun(DB_PO_HOST_RUN&	dphr);

public:
	String					GetName(UINT32 nID);   

public:
	INT32					IsApplyPolicyFromBootTime();

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_HOST_RUN pdphr, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_HOST_RUN& dphr);

public:
	CManagePoHostRun();
    ~CManagePoHostRun();

};

extern CManagePoHostRun*	t_ManagePoHostRun;

#endif
