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


#ifndef MEM_PATCH_FILE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_PATCH_FILE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_patch_file
{
	_mem_patch_file()
	{
		nID					= 0;		
	}

	UINT32			nID;	
	TMapIDStr		tDnFileMap;
}MEM_PATCH_FILE, *PMEM_PATCH_FILE;

typedef list<MEM_PATCH_FILE>			TListMemPatchFile;
typedef TListMemPatchFile::iterator		TListMemPatchFileItor;

typedef map<UINT32, MEM_PATCH_FILE>		TMapMemPatchFile;
typedef TMapMemPatchFile::iterator		TMapMemPatchFileItor;

#endif //MEM_PATCH_FILE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



