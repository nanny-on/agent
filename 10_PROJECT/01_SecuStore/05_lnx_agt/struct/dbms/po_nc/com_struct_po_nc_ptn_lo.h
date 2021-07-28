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
#ifndef DB_PO_NC_PTN_LO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_NC_PTN_LO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_nc_ptn_lo
{
	_db_po_nc_ptn_lo()
	{	  
		nPtnRangeType			= 0;
		nPtnObjType				= 0;
		nUserEditPtn			= 0;

		nPtnUnitNum				= 0;
		nPtnChkValue			= 0;
	}

	DB_PO_HEADER		tDPH;		

	UINT32				nPtnRangeType;
	UINT32				nPtnObjType;
	UINT32				nUserEditPtn;

	String				strSavePath;
	String				strSaveName;
	String				strFileHash;
	String				strPtnVersion;
	UINT32				nPtnUnitNum;
	UINT32				nPtnChkValue;
}DB_PO_NC_PTN_LO, *PDB_PO_NC_PTN_LO;

typedef list<DB_PO_NC_PTN_LO>				TListDBPoNcPtnLo;
typedef TListDBPoNcPtnLo::iterator			TListDBPoNcPtnLoItor;

typedef map<UINT32, DB_PO_NC_PTN_LO>		TMapDBPoNcPtnLo;
typedef TMapDBPoNcPtnLo::iterator			TMapDBPoNcPtnLoItor;

#endif //DB_PO_NC_PTN_LO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



