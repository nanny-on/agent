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

#ifndef DB_SITE_FILE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_SITE_FILE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_site_file
{
	_db_site_file()
	{
		nID						= 0;
		nUsedFlag				= STATUS_USED_MODE_ON;
		nRegDate				= 0;
		nUsedMode				= 0;
		
		nRegSvrID				= 0;
		nSyncSvrStep			= 0;
	
		nFileSize				= 0;
		nPeType					= 0;
		nPeBit					= 0;
	} 

	UINT32				nID ;
	UINT32				nUsedFlag;
	UINT32				nRegDate;
	UINT32				nUsedMode;

	UINT32				nRegSvrID;
	UINT32				nSyncSvrStep;
	
	String				strFeKey;
	String				strFileHash;

	String				strFilePath;
	String				strFileName;
	String				strFileDescr;
	String				strFileVersion;
	UINT32				nFileSize;
	String				strPubName;
	String				strPubSN;
	String				strCntName;
	String				strCntSN;
	String				strPublisherName;
	String				strProductName;
	UINT32				nPeType;
	UINT32				nPeBit;
}DB_SITE_FILE, *PDB_SITE_FILE;

typedef list<DB_SITE_FILE>				TListDBSiteFile;
typedef TListDBSiteFile::iterator		TListDBSiteFileItor;

typedef map<UINT32, DB_SITE_FILE>		TMapDBSiteFile;
typedef TMapDBSiteFile::iterator		TMapDBSiteFileItor;

#endif //DB_SITE_FILE_H_4D39CDDB_E289_4d56_9F98_FE42776F4467




