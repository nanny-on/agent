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

#ifndef MEM_CTRL_INSTANT_MSG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_CTRL_INSTANT_MSG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_ctrl_instant_msg
{
	_mem_ctrl_instant_msg()
	{
		nAreaType			= 0;
	}

	String			strTitle;
	String			strMsg;
	UINT32			nAreaType;
}MEM_CTRL_INSTANT_MSG, *PMEM_CTRL_INSTANT_MSG;

typedef list<MEM_CTRL_INSTANT_MSG>				TListMemCtrlInstantMsg;
typedef TListMemCtrlInstantMsg::iterator		TListMemCtrlInstantMsgItor;

#endif //MEM_CTRL_COMMON_INSTANT_MSG_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



