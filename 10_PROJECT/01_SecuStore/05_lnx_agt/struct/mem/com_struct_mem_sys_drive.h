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

#ifndef MEM_SYS_DRIVE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_SYS_DRIVE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_sys_drive
{
	_mem_sys_drive()
	{
		nID					= 0;
		strDriveName		= "";
	}

	UINT32			nID;	
	UINT32			nDriveType;
	String			strDriveName;
}MEM_SYS_DRIVE, *PMEM_SYS_DRIVE;

typedef list<MEM_SYS_DRIVE>				TListMemSysDriver;
typedef TListMemSysDriver::iterator		TListMemSysDriverItor;

typedef map<UINT32, MEM_SYS_DRIVE>		TMapMemSysDriver;
typedef TMapMemSysDriver::iterator		TMapMemSysDriverItor;

#endif //MEM_SYS_DRIVE_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



