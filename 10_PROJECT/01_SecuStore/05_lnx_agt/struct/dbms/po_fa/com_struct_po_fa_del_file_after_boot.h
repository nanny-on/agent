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

#ifndef _COM_STRUCT_PO_FA_DEL_FILE_AFTER_BOOT__
#define _COM_STRUCT_PO_FA_DEL_FILE_AFTER_BOOT__

typedef struct _db_del_file_after_boot
{
    _db_del_file_after_boot()
    {
        nID 	= 0;
    }

    UINT32		nID;
    String		strDelFilePath;
}DB_DEL_FILE_AFTER_BOOT, *PDB_DEL_FILE_AFTER_BOOT;

typedef list<DB_DEL_FILE_AFTER_BOOT>				TListDelFileAfterBoot;
typedef TListDelFileAfterBoot::iterator				TListDelFileAfterBootItor;

typedef map<UINT32, DB_DEL_FILE_AFTER_BOOT>			TMapDelFileAfterBoot;
typedef TMapDelFileAfterBoot::iterator				TMapDelFileAfterBootItor;

#endif //_COM_STRUCT_PO_FA_DEL_FILE_AFTER_BOOT__



