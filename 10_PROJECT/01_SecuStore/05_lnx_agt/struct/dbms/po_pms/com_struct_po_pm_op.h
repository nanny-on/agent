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

#ifndef DB_PO_PM_OP_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_PO_PM_OP_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pm_op
{
	_db_po_pm_op()
    {
		nLogSyncMode		= 0;
		nDnTryNum			= 0;
		nMonRmPatch			= 0;
		nAutoPatchType		= 0;

		nPtnDnFileType		= 0;
    }

    DB_PO_HEADER		tDPH;
    
	UINT32		nLogSyncMode;
	UINT32		nDnTryNum;
	UINT64		nMonRmPatch;
	UINT32		nAutoPatchType;

	String		strPtnFileSvName;

	String		strPtnVersion;
	String		strPtnFileName;
	String		strPtnFileHash;

	UINT32		nPtnDnFileType;
	String		strPtnDnFileName;
	String		strPtnDnFileHash;

}DB_PO_PM_OP, *PDB_PO_PM_OP;

typedef list<DB_PO_PM_OP>				TListDBPoPmOp;
typedef TListDBPoPmOp::iterator			TListDBPoPmOpItor;

typedef map<UINT32, DB_PO_PM_OP>		TMapDBPoPmOp;
typedef TMapDBPoPmOp::iterator			TMapDBPoPmOpItor;

#endif //DB_PO_PM_OP_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



