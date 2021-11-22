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

#ifndef DB_PTN_PROC_FILE_H_348683B5_F316_4a4d_842D_63E68B189A5D
#define DB_PTN_PROC_FILE_H_348683B5_F316_4a4d_842D_63E68B189A5D

typedef struct _db_ptn_proc_file
{
    _db_ptn_proc_file()
    {
        _init();        
    }

	void _init()
	{
		nID 				= 0;
		nRegDate			= 0;
		nUsedMode			= 0;

		nFileSize			= 0;

		nOsID				= 0;
		nBuildSN			= 0;

		nPeType				= 0;
		nPeBit				= 0;

		nPtnSrcType			= 0;
		nPtnType			= 0;
		nPtnRisk			= 0;

		nChecked			= 0;
	}
  
	UINT32		nID;
	UINT32		nRegDate;
	UINT32		nUsedMode;

	CString		strFeKey;
	CString		strFileHash;

	CString		strFilePath;
	CString		strFileName;
	CString		strFileDescr;
	CString		strFileVersion;
	UINT32		nFileSize;
	CString		strPubName;
	CString		strPubSN;
	CString		strCntName;
	CString		strCntSN;
	CString		strPublisherName;
	CString		strProductName;
	UINT64		nOsID;
	UINT32		nBuildSN;
	UINT32		nPeType;
	UINT32		nPeBit;

	UINT32		nPtnSrcType;
	UINT32		nPtnType;
	UINT32		nPtnRisk;

	UINT32		nChecked;

}DB_PTN_PROC_FILE, *PDB_PTN_PROC_FILE;

typedef list<DB_PTN_PROC_FILE>					TListDBPtnProcFile;
typedef TListDBPtnProcFile::iterator			TListDBPtnProcFileItor;

typedef map<UINT32, DB_PTN_PROC_FILE>			TMapDBPtnProcFile;
typedef TMapDBPtnProcFile::iterator				TMapDBPtnProcFileItor;
typedef TMapDBPtnProcFile::reverse_iterator		TMapDBPtnProcFileRItor;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#endif //DB_PTN_PROC_FILE_H_348683B5_F316_4a4d_842D_63E68B189A5D



