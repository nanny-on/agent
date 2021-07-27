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


#ifndef COM_HEADER_REMOVE_LOG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_REMOVE_LOG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////


#define		FILE_DT_CHK_TYPE_CREATE				0
#define		FILE_DT_CHK_TYPE_WRITE				1
#define		FILE_DT_CHK_TYPE_ACCESS				2

//------------------------------------------------------------------------------

typedef INT32	(*RemoveFileByDTType)(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay);
typedef INT32	(*RemoveFileByLastDayType)(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay);
typedef INT32	(*AddSkipFilePathType)(LPCTSTR lpSkipFile);
typedef INT32	(*ClearSkipFilePathType)();

//------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_FILE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
