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

#ifndef MEM_CTRL_REMOTE_ORDER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_CTRL_REMOTE_ORDER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_ctrl_remote_order
{
	_mem_ctrl_remote_order()
	{
		nType			= 0;
		nSubGroup		= 0;
		nOption			= 0;

		nMakeType		= 0;
		nMakeID			= 0;
	}

	UINT32			nType;	
	UINT32			nSubGroup;
	UINT32			nOption;
	String			strValue;
	TMapID			tValueMap;
	TMapStr			tStrValueMap;

	UINT32			nMakeType;
	UINT32			nMakeID;
}MEM_CTRL_REMOTE_ORDER, *PMEM_CTRL_REMOTE_ORDER;

typedef list<MEM_CTRL_REMOTE_ORDER>				TListMemCtrlRemoteOrder;
typedef TListMemCtrlRemoteOrder::iterator		TListMemCtrlRemoteOrderItor;

#endif //MEM_CTRL_COMMON_REMOTE_POWER_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



