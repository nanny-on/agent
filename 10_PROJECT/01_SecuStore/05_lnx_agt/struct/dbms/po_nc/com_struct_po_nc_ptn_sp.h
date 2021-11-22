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

#ifndef DB_PO_NC_PTN_SP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_NC_PTN_SP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_nc_ptn_sp
{
	_db_po_nc_ptn_sp()
	{
		nConfirmLevel	= 0;
		nConfirmDay		= 0;	
		
	}

	DB_PO_HEADER		tDPH;

	UINT32				nConfirmLevel;
	UINT32				nConfirmDay;
		
}DB_PO_NC_PTN_SP, *PDB_PO_NC_PTN_SP;

typedef list<DB_PO_NC_PTN_SP>				TListDBPoNcPtnSP;
typedef TListDBPoNcPtnSP::iterator			TListDBPoNcPtnSPItor;

typedef map<UINT32, DB_PO_NC_PTN_SP>		TMapDBPoNcPtnSP;
typedef TMapDBPoNcPtnSP::iterator			TMapDBPoNcPtnSPItor;
//---------------------------------------------------------------------------

typedef struct _db_po_nc_ptn_sp_pkg
{
	_db_po_nc_ptn_sp_pkg()
	{
		nChkOrder		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nChkOrder;

}DB_PO_NC_PTN_SP_PKG, *PDB_PO_NC_PTN_SP_PKG;

typedef list<DB_PO_NC_PTN_SP_PKG>			TListDBPoNcPtnSPPkg;
typedef TListDBPoNcPtnSPPkg::iterator		TListDBPoNcPtnSPPkgItor;

typedef map<UINT32, DB_PO_NC_PTN_SP_PKG>	TMapDBPoNcPtnSPPkg;
typedef TMapDBPoNcPtnSPPkg::iterator		TMapDBPoNcPtnSPPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_nc_ptn_sp_unit
{
	_db_po_nc_ptn_sp_unit()
	{
		nBlockMode		= 0;
		nConfirmLevel	= 0;
		nConfirmDay		= 0;	

		nFltChkType		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nBlockMode;

	UINT32				nConfirmLevel;
	UINT32				nConfirmDay;

	UINT32				nFltChkType;
	DB_FILE_INFO		tDFI;

}DB_PO_NC_PTN_SP_UNIT, *PDB_PO_NC_PTN_SP_UNIT;

typedef list<DB_PO_NC_PTN_SP_UNIT>			TListDBPoNcPtnSPUnit;
typedef TListDBPoNcPtnSPUnit::iterator		TListDBPoNcPtnSPUnitItor;

typedef map<UINT32, DB_PO_NC_PTN_SP_UNIT>	TMapDBPoNcPtnSPUnit;
typedef TMapDBPoNcPtnSPUnit::iterator		TMapDBPoNcPtnSPUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


#endif //DB_PO_NC_PTN_SP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



