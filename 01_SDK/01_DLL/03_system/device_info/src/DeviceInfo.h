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


#ifndef _DEVICE_INFO_H__

#define _DEVICE_INFO_H__

extern "C" {

INT32 InitDevInfo(PVOID* lpObject);
INT32 FreeDevInfo(PVOID lpObject);
INT32 ASIDVI_EnumDeviceInfoList(PVOID lpObject, LPVOID lParam, CallBack_DeviceInfo fnCallback);
INT32 ASIDVI_StateChange(PVOID lpObject, LPCTSTR pszDeviceInstanceID, DWORD nEnableFlag);
INT32 ASIDVI_GetDeviceStatus(PVOID lpObject, LPCTSTR pszDeviceInstanceID);
INT32 ASIDVI_RemoveDevice(PVOID lpObject, LPCTSTR pszDeviceInstanceID);
INT32 ASIDVI_IsExistDevice(PVOID lpObject, LPCTSTR pszDeviceInstanceID);

}

#endif /*_DEVICE_INFO_H__*/

