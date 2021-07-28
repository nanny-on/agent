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

#ifndef DB_PO_FE_SINGLE_PTN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_FE_SINGLE_PTN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_fe_single_ptn
{
	_db_po_fe_single_ptn()
	{
	}

	DB_PO_HEADER		tDPH;
	
}DB_PO_FE_SINGLE_PTN, *PDB_PO_FE_SINGLE_PTN;

typedef list<DB_PO_FE_SINGLE_PTN>					TListDBPoFeSinglePtn;
typedef TListDBPoFeSinglePtn::iterator				TListDBPoFeSinglePtnItor;

typedef map<UINT32, DB_PO_FE_SINGLE_PTN>			TMapDBPoFeSinglePtn;
typedef TMapDBPoFeSinglePtn::iterator				TMapDBPoFeSinglePtnItor;
	
	
typedef struct _db_po_fe_single_ptn_unit
{
    _db_po_fe_single_ptn_unit()
    {
		nTargetType				= 0;
		nTargetID	 			= 0;

		nBlockType				= 0;
		nPeType					= 0;
		nPeBit					= 0;
		nLimitTime				= 0;
    }
  
	DB_PO_HEADER		tDPH;

	UINT32				nTargetType;
	UINT32				nTargetID;	
	UINT32				nBlockType;
	String				strFeKey;
	UINT32				nPeType;
	UINT32				nPeBit;
	UINT32				nLimitTime;
}DB_PO_FE_SINGLE_PTN_UNIT, *PDB_PO_FE_SINGLE_PTN_UNIT;

typedef list<DB_PO_FE_SINGLE_PTN_UNIT>				TListDBPoFeSinglePtnUnit;
typedef TListDBPoFeSinglePtnUnit::iterator			TListDBPoFeSinglePtnUnitItor;

typedef map<UINT32, DB_PO_FE_SINGLE_PTN_UNIT>		TMapDBPoFeSinglePtnUnit;
typedef TMapDBPoFeSinglePtnUnit::iterator			TMapDBPoFeSinglePtnUnitItor;

#endif //DB_PO_FE_SINGLE_PTN_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



