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
 */


#ifndef COM_HEADER_NOTIFY_LOG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_NOTIFY_LOG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////


#define		FILE_NOTIFY_TYPE_CREATE				1
#define		FILE_NOTIFY_TYPE_DELETE				2

//------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
typedef struct _notify_path
{
	INT32 nOrderID;
	INT32 nExtOption;
	INT32 nWatchd;
	INT32 nPathLen;
	char acNotifyPath[CHAR_MAX_SIZE];
} NOTIFY_PATH, *PNOTIFY_PATH;

typedef map<INT32, NOTIFY_PATH>			TMapNotifyPath;
typedef TMapNotifyPath::iterator		TMapNotifyPathItor;

#endif //COM_HEADER_NOTIFY_LOG_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
