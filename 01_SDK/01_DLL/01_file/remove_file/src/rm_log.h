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

#ifndef _SC_SERVER_RM_LOG_DLL_
#define _SC_SERVER_RM_LOG_DLL_

#include <dirent.h>

extern "C" {
typedef struct DeleteFileList_t {

	int nChkDay;
	char acFilePath[CHAR_MAX_SIZE];
	char acFileName[CHAR_MAX_SIZE];
	DeleteFileList_t *pNext;

}DeleteFileList;

INT32 RemoveFileByDT(const char* lpFindPath, unsigned int nChkType, unsigned int nDay);
int RemoveFileByLastDay(const char* lpFindPath, unsigned int nChkType, unsigned int nDay);

}

#endif
