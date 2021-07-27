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


#ifndef COM_HEADER_HTTP_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_HTTP_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

typedef INT32 (*ASIHTTP_GetFileType)(LPCTSTR szHost, LPCTSTR lpFile, LPCTSTR lpSaveedFileName, INT32 nPort, PVOID hRecvStatus);
typedef INT32 (*ASIHTTP_GetFileSType)(LPCTSTR szHost, LPCTSTR lpFile, LPCTSTR lpSaveedFileName, INT32 nPort, PVOID hRecvStatus);
typedef INT32 (*ASIHTTP_PostFileSType)(LPCTSTR szHost, LPCTSTR lpURL, INT32 nPort, PVOID hRecvStatus);
typedef INT32 (*ASIHTTP_IsLiveTargetType)(LPCTSTR szHost, INT32 nPort);
typedef INT32 (*ASIHTTP_SetLogPolicyType)(LPCTSTR lpLogPath, LPCTSTR lpLogFile, INT32 nRemainFlag);

//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_HTTP_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
