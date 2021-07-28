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

#ifndef MEM_WORK_EXECUTE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_WORK_EXECUTE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_work_execute
{
	_mem_work_execute()
	{
		nID					= 0;
		nType				= 0;
		nCode				= 0;
		nItemID				= 0;
		nItemPkg			= 0;
		nExeType			= 0;

		nExeSession			= 0;
		nWaitChild			= 0;

		hNotifyHwnd			= NULL;
		nNotifyID			= 0;

		nEndWork			= 0;
		nErrCode			= 0;		
	}

	UINT32			nID;	
	UINT32			nType;
	UINT32			nCode;
	UINT32			nItemID;
	UINT32			nItemPkg;
	UINT32			nExeType;
	
	UINT32			nExeSession;
	String			strExecute;
	String			strCommand;
	String			strFileName;
	String			strWorkPath;
	UINT32			nWaitChild;	

	HANDLE			hNotifyHwnd;
	UINT32			nNotifyID;

	String			strLogFile;
	UINT32			nEndWork;
	UINT32			nErrCode;
}MEM_WORK_EXECUTE, *PMEM_WORK_EXECUTE;

typedef list<MEM_WORK_EXECUTE>				TListMemWorkExecute;
typedef TListMemWorkExecute::iterator		TListMemWorkExecuteItor;

typedef map<UINT32, MEM_WORK_EXECUTE>		TMapMemWorkExecute;
typedef TMapMemWorkExecute::iterator		TMapMemWorkExecuteItor;

typedef map<UINT64, MEM_WORK_EXECUTE>		TMapMemKeyWorkExecute;
typedef TMapMemKeyWorkExecute::iterator		TMapMemKeyWorkExecuteItor;

#endif //MEM_WORK_EXECUTE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



