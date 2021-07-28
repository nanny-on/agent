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

#ifndef DB_PO_HOST_RUN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_HOST_RUN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_host_run
{
	_db_po_host_run()
	{
		nRunOption		= 0;
		nRmLogDay		= 0;
		nLgnConPeriod	= 0;
		nMgrShowType	= 0;
		nShowLogDay		= 0;
		nBootChkType	= 0;
		nIntegrityMode	= 0;
		nCrossAuthMode	= 0;
		nOncePktNum		= 0;
		nConnManage		= 0;
		nLogReset		= 0;

		nLnxRunOption  = 0;
		nLnxRmLogDay	= 0;
		nLnxLgnConPeriod= 0;
		nLnxBootChkType	= 0;
		nLnxOncePktNum	= 0;
		nLnxConnManage	= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nRunOption;
	UINT32				nRmLogDay;
	UINT32				nLgnConPeriod;
	UINT32				nMgrShowType;
	UINT32				nShowLogDay;
	UINT32				nBootChkType;
	UINT32				nIntegrityMode;
	UINT32				nCrossAuthMode;
	UINT32				nOncePktNum;

	TMapID64			tMgrShowOrderPkgMap;
	TMapID64			tMgrShowOrderClsMap;

	UINT32				nConnManage;
	UINT32				nLogReset;

	// linux
	UINT32				nLnxRunOption;
	UINT32				nLnxRmLogDay;
	UINT32				nLnxLgnConPeriod;
	UINT32				nLnxBootChkType;
	UINT32				nLnxOncePktNum;
	UINT32				nLnxConnManage;

}DB_PO_HOST_RUN, *PDB_PO_HOST_RUN;

typedef list<DB_PO_HOST_RUN>				TListDBPoHostRun;
typedef TListDBPoHostRun::iterator			TListDBPoHostRunItor;

typedef map<UINT32, DB_PO_HOST_RUN>			TMapDBPoHostRun;
typedef TMapDBPoHostRun::iterator			TMapDBPoHostRunItor;

#endif //DB_PO_HOST_RUN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



