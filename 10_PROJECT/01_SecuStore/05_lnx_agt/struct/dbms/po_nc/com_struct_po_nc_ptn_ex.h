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

#ifndef DB_PO_NC_PTN_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_NC_PTN_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_nc_ptn_ex
{
	_db_po_nc_ptn_ex()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_NC_PTN_EX, *PDB_PO_NC_PTN_EX;

typedef list<DB_PO_NC_PTN_EX>				TListDBPoNcPtnEx;
typedef TListDBPoNcPtnEx::iterator			TListDBPoNcPtnExItor;

typedef map<UINT32, DB_PO_NC_PTN_EX>		TMapDBPoNcPtnEx;
typedef TMapDBPoNcPtnEx::iterator			TMapDBPoNcPtnExItor;
//---------------------------------------------------------------------------

typedef struct _db_po_nc_ptn_ex_pkg
{
	_db_po_nc_ptn_ex_pkg()
	{
		nChkOrder		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nChkOrder;

}DB_PO_NC_PTN_EX_PKG, *PDB_PO_NC_PTN_EX_PKG;

typedef list<DB_PO_NC_PTN_EX_PKG>			TListDBPoNcPtnExPkg;
typedef TListDBPoNcPtnExPkg::iterator		TListDBPoNcPtnExPkgItor;

typedef map<UINT32, DB_PO_NC_PTN_EX_PKG>	TMapDBPoNcPtnExPkg;
typedef TMapDBPoNcPtnExPkg::iterator		TMapDBPoNcPtnExPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_nc_ptn_ex_unit
{
	_db_po_nc_ptn_ex_unit()
	{
		nBlockMode			= 0;

		nUnitType			= 0;
		nFltChkType			= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nBlockMode;

	UINT32				nUnitType;
	UINT32				nFltChkType;

	String				strUserName;
	DB_FILE_INFO		tDFI;

}DB_PO_NC_PTN_EX_UNIT, *PDB_PO_NC_PTN_EX_UNIT;

typedef list<DB_PO_NC_PTN_EX_UNIT>			TListDBPoNcPtnExUnit;
typedef TListDBPoNcPtnExUnit::iterator		TListDBPoNcPtnExUnitItor;

typedef map<UINT32, DB_PO_NC_PTN_EX_UNIT>	TMapDBPoNcPtnExUnit;
typedef TMapDBPoNcPtnExUnit::iterator		TMapDBPoNcPtnExUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_NC_PTN_EX_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



