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

#ifndef DB_PO_DEPLOY_FILE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_DEPLOY_FILE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_deploy_file
{
	_db_po_deploy_file()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE, *PDB_PO_DEPLOY_FILE;

typedef list<DB_PO_DEPLOY_FILE>					TListDBPoDeployFile;
typedef TListDBPoDeployFile::iterator			TListDBPoDeployFileItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE>			TMapDBPoDeployFile;
typedef TMapDBPoDeployFile::iterator			TMapDBPoDeployFileItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_pkg
{
	_db_po_deploy_file_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE_PKG, *PDB_PO_DEPLOY_FILE_PKG;

typedef list<DB_PO_DEPLOY_FILE_PKG>				TListDBPoDeployFilePkg;
typedef TListDBPoDeployFilePkg::iterator		TListDBPoDeployFilePkgItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_PKG>		TMapDBPoDeployFilePkg;
typedef TMapDBPoDeployFilePkg::iterator			TMapDBPoDeployFilePkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_unit
{
	_db_po_deploy_file_unit()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE_UNIT, *PDB_PO_DEPLOY_FILE_UNIT;

typedef list<DB_PO_DEPLOY_FILE_UNIT>			TListDBPoDeployFileUnit;
typedef TListDBPoDeployFileUnit::iterator		TListDBPoDeployFileUnitItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_UNIT>		TMapDBPoDeployFileUnit;
typedef TMapDBPoDeployFileUnit::iterator		TMapDBPoDeployFileUnitItor;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_unit_info_pkg
{
	_db_po_deploy_file_unit_info_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE_UNIT_INFO_PKG, *PDB_PO_DEPLOY_FILE_UNIT_INFO_PKG;

typedef list<DB_PO_DEPLOY_FILE_UNIT_INFO_PKG>			TListDBPoDeployFileUnitInfoPkg;
typedef TListDBPoDeployFileUnitInfoPkg::iterator			TListDBPoDeployFileUnitInfoPkgItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_UNIT_INFO_PKG>		TMapDBPoDeployFileUnitInfoPkg;
typedef TMapDBPoDeployFileUnitInfoPkg::iterator			TMapDBPoDeployFileUnitInfoPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_unit_chk_pkg
{
	_db_po_deploy_file_unit_chk_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE_UNIT_CHK_PKG, *PDB_PO_DEPLOY_FILE_UNIT_CHK_PKG;

typedef list<DB_PO_DEPLOY_FILE_UNIT_CHK_PKG>			TListDBPoDeployFileUnitChkPkg;
typedef TListDBPoDeployFileUnitChkPkg::iterator			TListDBPoDeployFileUnitChkPkgItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_UNIT_CHK_PKG>		TMapDBPoDeployFileUnitChkPkg;
typedef TMapDBPoDeployFileUnitChkPkg::iterator			TMapDBPoDeployFileUnitChkPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_unit_tar_pkg
{
	_db_po_deploy_file_unit_tar_pkg()
	{
	}

	DB_PO_HEADER		tDPH;

}DB_PO_DEPLOY_FILE_UNIT_TAR_PKG, *PDB_PO_DEPLOY_FILE_UNIT_TAR_PKG;

typedef list<DB_PO_DEPLOY_FILE_UNIT_TAR_PKG>			TListDBPoDeployFileUnitTarPkg;
typedef TListDBPoDeployFileUnitTarPkg::iterator			TListDBPoDeployFileUnitTarPkgItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_UNIT_TAR_PKG>		TMapDBPoDeployFileUnitTarPkg;
typedef TMapDBPoDeployFileUnitTarPkg::iterator			TMapDBPoDeployFileUnitTarPkgItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_unit_sch_pkg
{
	_db_po_deploy_file_unit_sch_pkg()
	{
		nScanTime		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nScanTime;

}DB_PO_DEPLOY_FILE_UNIT_SCH_PKG, *PDB_PO_DEPLOY_FILE_UNIT_SCH_PKG;

typedef list<DB_PO_DEPLOY_FILE_UNIT_SCH_PKG>			TListDBPoDeployFileUnitSchPkg;
typedef TListDBPoDeployFileUnitSchPkg::iterator			TListDBPoDeployFileUnitSchPkgItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_UNIT_SCH_PKG>		TMapDBPoDeployFileUnitSchPkg;
typedef TMapDBPoDeployFileUnitSchPkg::iterator			TMapDBPoDeployFileUnitSchPkgItor;
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_info_unit
{
	_db_po_deploy_file_info_unit()
	{
		nExeUserType			= 0;
		nFileType				= 0;
		nFileEncType			= 0;

		nExeNumType				= 0;
		nSchUsedMode			= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nExeUserType;
	UINT32				nFileType;
	UINT32				nFileEncType;
	String				strFileOrName;
	String				strFileSaveName;
	String				strFileHash;
	String				strFileCommand;

	String				strTargetPath;
	UINT32				nExeNumType;
	UINT32				nSchUsedMode;

}DB_PO_DEPLOY_FILE_INFO_UNIT, *PDB_PO_DEPLOY_FILE_INFO_UNIT;

typedef list<DB_PO_DEPLOY_FILE_INFO_UNIT>				TListDBPoDeployFileInfoUnit;
typedef TListDBPoDeployFileInfoUnit::iterator			TListDBPoDeployFileInfoUnitItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_INFO_UNIT>		TMapDBPoDeployFileInfoUnit;
typedef TMapDBPoDeployFileInfoUnit::iterator			TMapDBPoDeployFileInfoUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_chk_unit
{
	_db_po_deploy_file_chk_unit()
	{
		nCompType			= 0;
		nCompValType		= 0;
		nCompOpType			= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nCompType;
	UINT32				nCompValType;
	UINT32				nCompOpType;
	String				strCompValue;

}DB_PO_DEPLOY_FILE_CHK_UNIT, *PDB_PO_DEPLOY_FILE_CHK_UNIT;

typedef list<DB_PO_DEPLOY_FILE_CHK_UNIT>				TListDBPoDeployFileChkUnit;
typedef TListDBPoDeployFileChkUnit::iterator			TListDBPoDeployFileChkUnitItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_CHK_UNIT>			TMapDBPoDeployFileChkUnit;
typedef TMapDBPoDeployFileChkUnit::iterator				TMapDBPoDeployFileChkUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_tar_unit
{
	_db_po_deploy_file_tar_unit()
	{
		nSysMode			= 0;
		nSysInfo			= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT64				nSysMode;
	UINT64				nSysInfo;

}DB_PO_DEPLOY_FILE_TAR_UNIT, *PDB_PO_DEPLOY_FILE_TAR_UNIT;

typedef list<DB_PO_DEPLOY_FILE_TAR_UNIT>				TListDBPoDeployFileTarUnit;
typedef TListDBPoDeployFileTarUnit::iterator			TListDBPoDeployFileTarUnitItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_TAR_UNIT>			TMapDBPoDeployFileTarUnit;
typedef TMapDBPoDeployFileTarUnit::iterator				TMapDBPoDeployFileTarUnitItor;
//---------------------------------------------------------------------------

typedef struct _db_po_deploy_file_sch_unit
{
	_db_po_deploy_file_sch_unit()
	{
		nSchType		= 0;
		nSchTime		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nSchType;
	UINT32				nSchTime;

}DB_PO_DEPLOY_FILE_SCH_UNIT, *PDB_PO_DEPLOY_FILE_SCH_UNIT;

typedef list<DB_PO_DEPLOY_FILE_SCH_UNIT>				TListDBPoDeployFileSchUnit;
typedef TListDBPoDeployFileSchUnit::iterator			TListDBPoDeployFileSchUnitItor;

typedef map<UINT32, DB_PO_DEPLOY_FILE_SCH_UNIT>			TMapDBPoDeployFileSchUnit;
typedef TMapDBPoDeployFileSchUnit::iterator				TMapDBPoDeployFileSchUnitItor;
//---------------------------------------------------------------------------


#endif //DB_PO_DEPLOY_FILE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



