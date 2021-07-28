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

#ifndef ManageLogSecuH
#define ManageLogSecuH
//---------------------------------------------------------------------------

class CManageLogSecu : public CManageBase<DB_LOG_SECU>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddLogSecu(DB_LOG_SECU&	dls);
    INT32					EditLogSecu(DB_LOG_SECU&	dls);
    INT32					DelLogSecu(UINT32 nID);

public:
	INT32					SetPktSync(TListPVOID& tIDList);

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(PDB_LOG_SECU pdls, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_SECU& dls);

	INT32					SetPkt_Link(MemToken& SendToken);

public:
	CManageLogSecu();
    ~CManageLogSecu();

};

extern CManageLogSecu*	t_ManageLogSecu;

#endif
