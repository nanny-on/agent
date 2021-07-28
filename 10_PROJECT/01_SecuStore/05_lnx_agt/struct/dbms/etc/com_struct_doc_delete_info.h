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
 */
#ifndef DB_DOC_DELETE_INFO_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_DOC_DELETE_INFO_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_doc_delete_info
{
	_db_doc_delete_info()
    {
        nID				= 0;
        nUsedFlag		= 1;
		nRegDate		= 0;
		nLastScanTime	= 0;
		nLastDelTime	= 0;
		nDocCnt			= 0;
		nSchCnt			= 0;
		nMenualCnt		= 0;  
    }

    UINT32				nID;
	UINT32				nUsedFlag;
    UINT32				nRegDate;
    UINT32				nLastScanTime;
	UINT32				nLastDelTime;
	UINT32				nDocCnt;
	UINT32				nSchCnt;
	UINT32				nMenualCnt;

}DB_DOC_DELETE_INFO, *PDB_DOC_DELETE_INFO;

typedef list<DB_DOC_DELETE_INFO>				TListDBDocDeleteInfo;
typedef TListDBDocDeleteInfo::iterator			TListDBDocDeleteInfoItor;

typedef map<UINT32, DB_DOC_DELETE_INFO>			TMapDBDocDeleteInfo;
typedef TMapDBDocDeleteInfo::iterator			TMapDBDocDeleteInfoItor;
typedef TMapDBDocDeleteInfo::reverse_iterator	TMapDBDocDeleteInfoRItor;

#endif //DB_DOC_DELETE_INFO_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



