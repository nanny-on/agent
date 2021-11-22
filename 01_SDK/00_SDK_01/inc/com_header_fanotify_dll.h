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


#ifndef _COM_HEADER_FANOTIFY_DLL_H__
#define _COM_HEADER_FANOTIFY_DLL_H__


typedef struct _fanotify_path
{
	INT32 nOrderID;
	INT32 nPathLen;
	INT32 nIndex;
	INT32 nReserved;
	char acNotifyPath[MAX_PATH];
} FANOTIFY_PATH, *PFANOTIFY_PATH;

typedef map<INT32, FANOTIFY_PATH>		TMapFaNotifyPath;
typedef TMapFaNotifyPath::iterator		TMapFaNotifyPathItor;

#endif //_COM_HEADER_FANOTIFY_DLL_H__
