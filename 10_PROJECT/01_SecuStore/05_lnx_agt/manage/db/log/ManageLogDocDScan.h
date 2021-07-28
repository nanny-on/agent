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

#pragma once

class CManageLogDocDScan : public CManageBase<DB_LOG_DOC_DSCAN>
{

public:
	INT32		LoadDBMS();

public:
	INT32		AddLogDocDScan(DB_LOG_DOC_DSCAN& dldd);
	INT32		EditLogDocDScan(DB_LOG_DOC_DSCAN& dldd);
	INT32		ClearItem();

public:
	INT32		SetPkt(PDB_LOG_DOC_DSCAN pdldd, MemToken& SendToken);
	INT32		GetPkt(MemToken& RecvToken, DB_LOG_DOC_DSCAN& dldd);

public:
	INT32		SetPkt_Link(MemToken& SendToken);

public:
	CManageLogDocDScan(void);
	~CManageLogDocDScan(void);
};

extern CManageLogDocDScan*	t_ManageLogDocDScan;
