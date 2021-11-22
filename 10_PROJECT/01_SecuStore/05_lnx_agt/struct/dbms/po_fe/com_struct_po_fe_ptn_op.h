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

#ifndef DB_PO_FE_PTN_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_FE_PTN_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_fe_ptn_op
{
	_db_po_fe_ptn_op()
	{	  
		nBlockMode				= 0;
		nPtnPkgID				= 0;
		nPtnSchTime				= 0;
		nPtnScanTime			= 0;
		nPtnRangeType			= 0;
		nPtnObjType				= 0;
		nNewFileSendType		= 0;
		nNewFileConfirmMode		= 0;
	}

	DB_PO_HEADER		tDPH;		

	UINT32				nBlockMode;
	UINT32				nPtnPkgID;
	UINT32				nPtnSchTime;
	UINT32				nPtnScanTime;
	UINT32				nPtnRangeType;
	UINT32				nPtnObjType;
	UINT32				nNewFileSendType;
	UINT32				nNewFileConfirmMode;

}DB_PO_FE_PTN_OP, *PDB_PO_FE_PTN_OP;

typedef list<DB_PO_FE_PTN_OP>				TListDBPoFePtnOp;
typedef TListDBPoFePtnOp::iterator			TListDBPoFePtnOpItor;

typedef map<UINT32, DB_PO_FE_PTN_OP>		TMapDBPoFePtnOp;
typedef TMapDBPoFePtnOp::iterator			TMapDBPoFePtnOpItor;

#endif //DB_PO_FE_PTN_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



