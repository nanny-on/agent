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


#ifndef _COM_STRUCT_HEADER_H__
#define _COM_STRUCT_HEADER_H__

typedef struct _dl_svr_info
{
	_dl_svr_info()
	{
		nPort		= 0;
	}
	String			strAddress;
	UINT32			nPort;
}DL_SVR_INFO, *PDL_SVR_INFO;

typedef list<DL_SVR_INFO>			TListDLSvrInfo;
typedef TListDLSvrInfo::iterator	TListDLSvrInfoItor;

//**********************************************************************************************//

typedef list<UINT32>			TListID;
typedef TListID::iterator		TListIDItor;

typedef map<UINT32, UINT32>		TMapID;
typedef TMapID::iterator		TMapIDItor;

typedef list<String>			TListStr;
typedef TListStr::iterator		TListStrItor;
//**********************************************************************************************//

#endif //_COM_STRUCT_HEADER_H__