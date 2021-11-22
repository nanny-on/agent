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

#ifndef DB_PO_FA_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_FA_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_fa_op
{
	_db_po_fa_op()
	{
		nSchTime		= 0;
		nDelCount		= 0;
		nLimitSize		= 0;
		nLimitCnt		= 0;
		nChkFDTType		= 0;
		nChkFDTValue	= 0;
		nDelMethod		= 0;

		nScanTime		= 0;

		nLnxSchTime		= 0;
		nLnxDelCount	= 0;
		nLnxLimitSize	= 0;
		nLnxLimitCnt	= 0;
		nLnxChkFDTType	= 0;
		nLnxChkFDTValue	= 0;
		nLnxDelMethod	= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nSchTime;
	UINT32				nDelCount;
	UINT32				nLimitSize;
	UINT32				nLimitCnt;
	UINT32				nChkFDTType;
	UINT32				nChkFDTValue;
	UINT32				nDelMethod;

	UINT32				nScanTime;

	// linux
	UINT32				nLnxSchTime;
	UINT32				nLnxDelCount;
	UINT32				nLnxLimitSize;
	UINT32				nLnxLimitCnt;
	UINT32				nLnxChkFDTType;
	UINT32				nLnxChkFDTValue;
	UINT32				nLnxDelMethod;

}DB_PO_FA_OP, *PDB_PO_FA_OP;

typedef list<DB_PO_FA_OP>					TListDBPoFaOp;
typedef TListDBPoFaOp::iterator				TListDBPoFaOpItor;

typedef map<UINT32, DB_PO_FA_OP>			TMapDBPoFaOp;
typedef TMapDBPoFaOp::iterator				TMapDBPoFaOpItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_pkg
{
	_db_po_fa_op_pkg()
	{
		nRuleOrder		= 0;
	}

	DB_PO_HEADER		tDPH;
	UINT32				nRuleOrder;

}DB_PO_FA_OP_PKG, *PDB_PO_FA_OP_PKG;

typedef list<DB_PO_FA_OP_PKG>				TListDBPoFaOpPkg;
typedef TListDBPoFaOpPkg::iterator			TListDBPoFaOpPkgItor;

typedef map<UINT32, DB_PO_FA_OP_PKG>		TMapDBPoFaOpPkg;
typedef TMapDBPoFaOpPkg::iterator			TMapDBPoFaOpPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_unit
{
	_db_po_fa_op_unit()
	{
		nUnitType		= 0;
	}

	DB_PO_HEADER		tDPH;

	TMapID64			tLogModeMap;
	UINT32				nUnitType;

}DB_PO_FA_OP_UNIT, *PDB_PO_FA_OP_UNIT;

typedef list<DB_PO_FA_OP_UNIT>				TListDBPoFaOpUnit;
typedef TListDBPoFaOpUnit::iterator			TListDBPoFaOpUnitItor;

typedef map<UINT32, DB_PO_FA_OP_UNIT>		TMapDBPoFaOpUnit;
typedef TMapDBPoFaOpUnit::iterator			TMapDBPoFaOpUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_unit_sub_pkg
{
	_db_po_fa_op_unit_sub_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_FA_OP_UNIT_SUB_PKG, *PDB_PO_FA_OP_UNIT_SUB_PKG;

typedef list<DB_PO_FA_OP_UNIT_SUB_PKG>				TListDBPoFaOpUnitSubPkg;
typedef TListDBPoFaOpUnitSubPkg::iterator			TListDBPoFaOpUnitSubPkgItor;

typedef map<UINT32, DB_PO_FA_OP_UNIT_SUB_PKG>		TMapDBPoFaOpUnitSubPkg;
typedef TMapDBPoFaOpUnitSubPkg::iterator			TMapDBPoFaOpUnitSubPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_unit_obj_pkg
{
	_db_po_fa_op_unit_obj_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_FA_OP_UNIT_OBJ_PKG, *PDB_PO_FA_OP_UNIT_OBJ_PKG;

typedef list<DB_PO_FA_OP_UNIT_OBJ_PKG>				TListDBPoFaOpUnitObjPkg;
typedef TListDBPoFaOpUnitObjPkg::iterator			TListDBPoFaOpUnitObjPkgItor;

typedef map<UINT32, DB_PO_FA_OP_UNIT_OBJ_PKG>		TMapDBPoFaOpUnitObjPkg;
typedef TMapDBPoFaOpUnitObjPkg::iterator			TMapDBPoFaOpUnitObjPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_unit_sch_pkg
{
	_db_po_fa_op_unit_sch_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_FA_OP_UNIT_SCH_PKG, *PDB_PO_FA_OP_UNIT_SCH_PKG;

typedef list<DB_PO_FA_OP_UNIT_SCH_PKG>				TListDBPoFaOpUnitSchPkg;
typedef TListDBPoFaOpUnitSchPkg::iterator			TListDBPoFaOpUnitSchPkgItor;

typedef map<UINT32, DB_PO_FA_OP_UNIT_SCH_PKG>		TMapDBPoFaOpUnitSchPkg;
typedef TMapDBPoFaOpUnitSchPkg::iterator			TMapDBPoFaOpUnitSchPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_unit_rul_pkg
{
	_db_po_fa_op_unit_rul_pkg()
	{
		nRuleOrder	= 0;
		nAcMode		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nRuleOrder;
	UINT32				nAcMode;

}DB_PO_FA_OP_UNIT_RUL_PKG, *PDB_PO_FA_OP_UNIT_RUL_PKG;

typedef list<DB_PO_FA_OP_UNIT_RUL_PKG>				TListDBPoFaOpUnitRulPkg;
typedef TListDBPoFaOpUnitRulPkg::iterator			TListDBPoFaOpUnitRulPkgItor;

typedef map<UINT32, DB_PO_FA_OP_UNIT_RUL_PKG>		TMapDBPoFaOpUnitRulPkg;
typedef TMapDBPoFaOpUnitRulPkg::iterator			TMapDBPoFaOpUnitRulPkgItor;
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

typedef struct _db_po_fa_op_sub_unit
{
	_db_po_fa_op_sub_unit()
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

}DB_PO_FA_OP_SUB_UNIT, *PDB_PO_FA_OP_SUB_UNIT;

typedef list<DB_PO_FA_OP_SUB_UNIT>				TListDBPoFaOpSubUnit;
typedef TListDBPoFaOpSubUnit::iterator			TListDBPoFaOpSubUnitItor;

typedef map<UINT32, DB_PO_FA_OP_SUB_UNIT>		TMapDBPoFaOpSubUnit;
typedef TMapDBPoFaOpSubUnit::iterator			TMapDBPoFaOpSubUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_obj_unit
{
	_db_po_fa_op_obj_unit()
	{
		nChkAcPath		= 0;
		nChkSelfPath	= 0;
	}

	DB_PO_HEADER		tDPH;

	String				strFilePath;
	UINT32				nChkAcPath;
	UINT32				nChkSelfPath;

}DB_PO_FA_OP_OBJ_UNIT, *PDB_PO_FA_OP_OBJ_UNIT;

typedef list<DB_PO_FA_OP_OBJ_UNIT>				TListDBPoFaOpObjUnit;
typedef TListDBPoFaOpObjUnit::iterator			TListDBPoFaOpObjUnitItor;

typedef map<UINT32, DB_PO_FA_OP_OBJ_UNIT>		TMapDBPoFaOpObjUnit;
typedef TMapDBPoFaOpObjUnit::iterator			TMapDBPoFaOpObjUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_fa_op_sch_unit
{
	_db_po_fa_op_sch_unit()
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
}DB_PO_FA_OP_SCH_UNIT, *PDB_PO_FA_OP_SCH_UNIT;

typedef list<DB_PO_FA_OP_SCH_UNIT>				TListDBPoFaOpSchUnit;
typedef TListDBPoFaOpSchUnit::iterator			TListDBPoFaOpSchUnitItor;

typedef map<UINT32, DB_PO_FA_OP_SCH_UNIT>		TMapDBPoFaOpSchUnit;
typedef TMapDBPoFaOpSchUnit::iterator			TMapDBPoFaOpSchUnitItor;
//---------------------------------------------------------------------------

#endif //DB_PO_FA_OP_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



