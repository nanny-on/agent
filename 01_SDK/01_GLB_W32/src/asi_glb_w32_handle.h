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
 * * 
 */

#ifndef _ASI_GLB_W32_HANDLE_H_
#define _ASI_GLB_W32_HANDLE_H_

extern TMapID64 glb_tHandleMap;

extern BOOL CloseHandle(HANDLE hObject);
extern BOOL	GetHandleInformation(HANDLE hObject, LPDWORD lpdwFlags);
extern BOOL	SetHandleInformation(HANDLE hObject, DWORD dwMask, DWORD dwFlags);

#endif /* _ASI_GLB_W32_HANDLE_H_ */


