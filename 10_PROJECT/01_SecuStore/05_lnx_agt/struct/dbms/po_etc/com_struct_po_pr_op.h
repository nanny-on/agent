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

#ifndef DB_PO_PR_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_PR_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_pr_op
{
	_db_po_pr_op()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_OP, *PDB_PO_PR_OP;

typedef list<DB_PO_PR_OP>					TListDBPoPrOp;
typedef TListDBPoPrOp::iterator				TListDBPoPrOpItor;

typedef map<UINT32, DB_PO_PR_OP>			TMapDBPoPrOp;
typedef TMapDBPoPrOp::iterator				TMapDBPoPrOpItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_pkg
{
	_db_po_pr_op_pkg()
	{
		nRuleOrder		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nRuleOrder;

}DB_PO_PR_OP_PKG, *PDB_PO_PR_OP_PKG;

typedef list<DB_PO_PR_OP_PKG>				TListDBPoPrOpPkg;
typedef TListDBPoPrOpPkg::iterator			TListDBPoPrOpPkgItor;

typedef map<UINT32, DB_PO_PR_OP_PKG>		TMapDBPoPrOpPkg;
typedef TMapDBPoPrOpPkg::iterator			TMapDBPoPrOpPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_unit
{
	_db_po_pr_op_unit()
	{		
		nUnitType		= 0;
	}

	DB_PO_HEADER		tDPH;

	TMapID64			tLogModeMap;
	UINT32				nUnitType;

}DB_PO_PR_OP_UNIT, *PDB_PO_PR_OP_UNIT;

typedef list<DB_PO_PR_OP_UNIT>				TListDBPoPrOpUnit;
typedef TListDBPoPrOpUnit::iterator			TListDBPoPrOpUnitItor;

typedef map<UINT32, DB_PO_PR_OP_UNIT>		TMapDBPoPrOpUnit;
typedef TMapDBPoPrOpUnit::iterator			TMapDBPoPrOpUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_unit_sub_pkg
{
	_db_po_pr_op_unit_sub_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_OP_UNIT_SUB_PKG, *PDB_PO_PR_OP_UNIT_SUB_PKG;

typedef list<DB_PO_PR_OP_UNIT_SUB_PKG>				TListDBPoPrOpUnitSubPkg;
typedef TListDBPoPrOpUnitSubPkg::iterator			TListDBPoPrOpUnitSubPkgItor;

typedef map<UINT32, DB_PO_PR_OP_UNIT_SUB_PKG>		TMapDBPoPrOpUnitSubPkg;
typedef TMapDBPoPrOpUnitSubPkg::iterator			TMapDBPoPrOpUnitSubPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_unit_obj_pkg
{
	_db_po_pr_op_unit_obj_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_OP_UNIT_OBJ_PKG, *PDB_PO_PR_OP_UNIT_OBJ_PKG;

typedef list<DB_PO_PR_OP_UNIT_OBJ_PKG>				TListDBPoPrOpUnitObjPkg;
typedef TListDBPoPrOpUnitObjPkg::iterator			TListDBPoPrOpUnitObjPkgItor;

typedef map<UINT32, DB_PO_PR_OP_UNIT_OBJ_PKG>		TMapDBPoPrOpUnitObjPkg;
typedef TMapDBPoPrOpUnitObjPkg::iterator			TMapDBPoPrOpUnitObjPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_unit_sch_pkg
{
	_db_po_pr_op_unit_sch_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_PR_OP_UNIT_SCH_PKG, *PDB_PO_PR_OP_UNIT_SCH_PKG;

typedef list<DB_PO_PR_OP_UNIT_SCH_PKG>				TListDBPoPrOpUnitSchPkg;
typedef TListDBPoPrOpUnitSchPkg::iterator			TListDBPoPrOpUnitSchPkgItor;

typedef map<UINT32, DB_PO_PR_OP_UNIT_SCH_PKG>		TMapDBPoPrOpUnitSchPkg;
typedef TMapDBPoPrOpUnitSchPkg::iterator			TMapDBPoPrOpUnitSchPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_unit_rul_pkg
{
	_db_po_pr_op_unit_rul_pkg()
	{
		nRuleOrder	= 0;
		nAcMode		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nRuleOrder;
	UINT32				nAcMode;

}DB_PO_PR_OP_UNIT_RUL_PKG, *PDB_PO_PR_OP_UNIT_RUL_PKG;

typedef list<DB_PO_PR_OP_UNIT_RUL_PKG>				TListDBPoPrOpUnitRulPkg;
typedef TListDBPoPrOpUnitRulPkg::iterator			TListDBPoPrOpUnitRulPkgItor;

typedef map<UINT32, DB_PO_PR_OP_UNIT_RUL_PKG>		TMapDBPoPrOpUnitRulPkg;
typedef TMapDBPoPrOpUnitRulPkg::iterator			TMapDBPoPrOpUnitRulPkgItor;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_sub_unit
{
	_db_po_pr_op_sub_unit()
	{	
		nSbType			= 0;
		nSbLinkID		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nSbType;
	UINT32				nSbLinkID;
	String				strSbUnicode;
	String				strSbDomain;
	String				strSbName;
	String				strSbProc;
	TMapID64			tLogModeMap;

}DB_PO_PR_OP_SUB_UNIT, *PDB_PO_PR_OP_SUB_UNIT;

typedef list<DB_PO_PR_OP_SUB_UNIT>				TListDBPoPrOpSubUnit;
typedef TListDBPoPrOpSubUnit::iterator			TListDBPoPrOpSubUnitItor;

typedef map<UINT32, DB_PO_PR_OP_SUB_UNIT>		TMapDBPoPrOpSubUnit;
typedef TMapDBPoPrOpSubUnit::iterator			TMapDBPoPrOpSubUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_obj_unit
{
	_db_po_pr_op_obj_unit()
	{
		nChkAcPath		= 0;
		nChkSelfPath	= 0;
	}

	DB_PO_HEADER		tDPH;

	String				strFilePath;
	UINT32				nChkAcPath;
	UINT32				nChkSelfPath;

}DB_PO_PR_OP_OBJ_UNIT, *PDB_PO_PR_OP_OBJ_UNIT;

typedef list<DB_PO_PR_OP_OBJ_UNIT>				TListDBPoPrOpObjUnit;
typedef TListDBPoPrOpObjUnit::iterator			TListDBPoPrOpObjUnitItor;

typedef map<UINT32, DB_PO_PR_OP_OBJ_UNIT>		TMapDBPoPrOpObjUnit;
typedef TMapDBPoPrOpObjUnit::iterator			TMapDBPoPrOpObjUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_pr_op_sch_unit
{
	_db_po_pr_op_sch_unit()
	{	
		nSchMon		= 0;
		nSchDay		= 0;
		nSchWeek	= 0;
		nSchHour	= 0;
		nSchMin		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nSchMon;
	UINT32				nSchDay;
	UINT32				nSchWeek;
	UINT32				nSchHour;
	UINT32				nSchMin;
}DB_PO_PR_OP_SCH_UNIT, *PDB_PO_PR_OP_SCH_UNIT;

typedef list<DB_PO_PR_OP_SCH_UNIT>				TListDBPoPrOpSchUnit;
typedef TListDBPoPrOpSchUnit::iterator			TListDBPoPrOpSchUnitItor;

typedef map<UINT32, DB_PO_PR_OP_SCH_UNIT>		TMapDBPoPrOpSchUnit;
typedef TMapDBPoPrOpSchUnit::iterator			TMapDBPoPrOpSchUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_PR_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



