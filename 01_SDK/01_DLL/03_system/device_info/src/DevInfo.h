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

#if !defined(AFX_DEVINFO_H__A00D015F_9061_11D2_8D3B_FC17F33B2809__INCLUDED_)
#define AFX_DEVINFO_H__A00D015F_9061_11D2_8D3B_FC17F33B2809__INCLUDED_

class CDevInfo  
{
	// Construction
public:
	TMapDeviceItemClass		m_tDeviceItemClassMap;

private:
	INT32	EnumAddDevices();

public:
	INT32	LoadStorageController();

public:
	INT32	StateChange(LPCTSTR pszDeviceInstanceID, DWORD nEnableFlag);
	INT32	GetDeviceStatus(LPCTSTR pszDeviceInstanceID);
	INT32	RemoveDevice(LPCTSTR pszDeviceInstanceID);
	INT32	IsExistDevice(LPCTSTR pszDeviceInstanceID);

public:
	CDevInfo();
	virtual ~CDevInfo();
};

#endif // !defined(AFX_DEVINFO_H__A00D015F_9061_11D2_8D3B_FC17F33B2809__INCLUDED_)