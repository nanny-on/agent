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

//---------------------------------------------------------------------------

#ifndef ManageSysDriveH
#define ManageSysDriveH
//---------------------------------------------------------------------------

class CManageSysDrive : public CManageBase<MEM_SYS_DRIVE>
{
private:
	UINT32				m_nGlobalID;

public:
	INT32				InitSysDrive();
	INT32				AddSysDrive(MEM_SYS_DRIVE& tMSD);
	INT32				DelSysDrive(String strDriveName);
	PMEM_SYS_DRIVE		FindSysDrive(String strDriveName);

public:
	INT32				GetNetDriveMap(UINT32 nChkMode, TMapStrID& tDrvMap);
	INT32				GetFixDriveMap(UINT32 nChkMode, TMapStrID& tDrvMap);
	INT32				GetUSBDriveMap(UINT32 nChkMode, TMapStrID& tDrvMap);

public:
	INT32				IsChangeNetDrive();
	INT32				IsChangeFixDrive();
	INT32				IsChangeUSBDrive();

public:
	void				PrintDbgInfo();

public:
	CManageSysDrive();
    ~CManageSysDrive();

};

extern CManageSysDrive	t_ManageSysDrive;

#endif
