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

#ifndef COM_HEADER_MBR_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define COM_HEADER_MBR_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#pragma pack(1)
//--------------------------------------------------------------------------
#define MODE_MBR_DATA		1
#define MODE_ACTIVE_DATA	2
//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

typedef BOOL (*fpAsiDumpMbrData)(char*, DWORD, char*);
typedef BOOL (*fpAsiCompareMbrData)(char*, DWORD, DWORD *, char*);
typedef BOOL (*fpAsiRestoreMbrData)(char*, DWORD, char*);
typedef BOOL (*fpAsiGetLastDumpPath)(char*, DWORD, char*);
//--------------------------------------------------------------------------

#pragma pack()

#endif //COM_HEADER_MBR_DLL_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



