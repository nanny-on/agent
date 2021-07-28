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

#ifndef DB_PO_IN_RS_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_IN_RS_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_in_rs_op
{
	_db_po_in_rs_op()
	{	  
		
	}

	DB_PO_HEADER		tDPH;
	
}DB_PO_IN_RS_OP, *PDB_PO_IN_RS_OP;

typedef list<DB_PO_IN_RS_OP>				TListDBPoInRsOp;
typedef TListDBPoInRsOp::iterator			TListDBPoInRsOpItor;

typedef map<UINT32, DB_PO_IN_RS_OP>			TMapDBPoInRsOp;
typedef TMapDBPoInRsOp::iterator			TMapDBPoInRsOpItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_pkg
{
	_db_po_in_rs_op_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_RS_OP_PKG, *PDB_PO_IN_RS_OP_PKG;

typedef list<DB_PO_IN_RS_OP_PKG>			TListDBPoInRsOpPkg;
typedef TListDBPoInRsOpPkg::iterator		TListDBPoInRsOpPkgItor;

typedef map<UINT32, DB_PO_IN_RS_OP_PKG>	TMapDBPoInRsOpPkg;
typedef TMapDBPoInRsOpPkg::iterator		TMapDBPoInRsOpPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_unit
{
	_db_po_in_rs_op_unit()
	{
		nAcMode = 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nAcMode;
	TMapID64			tLogModeMap;

}DB_PO_IN_RS_OP_UNIT, *PDB_PO_IN_RS_OP_UNIT;

typedef list<DB_PO_IN_RS_OP_UNIT>			TListDBPoInRsOpUnit;
typedef TListDBPoInRsOpUnit::iterator		TListDBPoInRsOpUnitItor;

typedef map<UINT32, DB_PO_IN_RS_OP_UNIT>	TMapDBPoInRsOpUnit;
typedef TMapDBPoInRsOpUnit::iterator		TMapDBPoInRsOpUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_unit_sub_pkg
{
	_db_po_in_rs_op_unit_sub_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_RS_OP_UNIT_SUB_PKG, *PDB_PO_IN_RS_OP_UNIT_SUB_PKG;

typedef list<DB_PO_IN_RS_OP_UNIT_SUB_PKG>			TListDBPoInRsOpUnitSubPkg;
typedef TListDBPoInRsOpUnitSubPkg::iterator			TListDBPoInRsOpUnitSubPkgItor;

typedef map<UINT32, DB_PO_IN_RS_OP_UNIT_SUB_PKG>	TMapDBPoInRsOpUnitSubPkg;
typedef TMapDBPoInRsOpUnitSubPkg::iterator			TMapDBPoInRsOpUnitSubPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_unit_obj_pkg
{
	_db_po_in_rs_op_unit_obj_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_IN_RS_OP_UNIT_OBJ_PKG, *PDB_PO_IN_RS_OP_UNIT_OBJ_PKG;

typedef list<DB_PO_IN_RS_OP_UNIT_OBJ_PKG>			TListDBPoInRsOpUnitObjPkg;
typedef TListDBPoInRsOpUnitObjPkg::iterator			TListDBPoInRsOpUnitObjPkgItor;

typedef map<UINT32, DB_PO_IN_RS_OP_UNIT_OBJ_PKG>	TMapDBPoInRsOpUnitObjPkg;
typedef TMapDBPoInRsOpUnitObjPkg::iterator			TMapDBPoInRsOpUnitObjPkgItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_sub_unit
{
	_db_po_in_rs_op_sub_unit()
	{	
		nAcMode			= 0;
	}

	DB_PO_HEADER		tDPH;

	String				strSbType;
	String				strSbProc;
	UINT32				nAcMode;

}DB_PO_IN_RS_OP_SUB_UNIT, *PDB_PO_IN_RS_OP_SUB_UNIT;

typedef list<DB_PO_IN_RS_OP_SUB_UNIT>				TListDBPoInRsOpSubUnit;
typedef TListDBPoInRsOpSubUnit::iterator			TListDBPoInRsOpSubUnitItor;

typedef map<UINT32, DB_PO_IN_RS_OP_SUB_UNIT>		TMapDBPoInRsOpSubUnit;
typedef TMapDBPoInRsOpSubUnit::iterator				TMapDBPoInRsOpSubUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_in_rs_op_obj_unit
{
	_db_po_in_rs_op_obj_unit()
	{
	}

	DB_PO_HEADER		tDPH;

	String				strChkType;
	String				strChkPath;
	String				strChkExt;

}DB_PO_IN_RS_OP_OBJ_UNIT, *PDB_PO_IN_RS_OP_OBJ_UNIT;

typedef list<DB_PO_IN_RS_OP_OBJ_UNIT>				TListDBPoInRsOpObjUnit;
typedef TListDBPoInRsOpObjUnit::iterator			TListDBPoInRsOpObjUnitItor;

typedef map<UINT32, DB_PO_IN_RS_OP_OBJ_UNIT>		TMapDBPoInRsOpObjUnit;
typedef TMapDBPoInRsOpObjUnit::iterator				TMapDBPoInRsOpObjUnitItor;
//---------------------------------------------------------------------------


#endif //DB_PO_IN_RS_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



