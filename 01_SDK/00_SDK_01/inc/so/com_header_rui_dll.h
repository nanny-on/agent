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


#ifndef COM_HEADER_RUI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_RUI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

typedef INT32 (*ASIRUI_GetCpuUsageTotalType)(PUINT32 nUsage);
typedef INT32 (*ASIRUI_GetCpuUsageItemType)(INT32 nItemID, PUINT32 nUsage);
typedef INT32 (*ASIRUI_AddCpuItemType)(LPCTSTR lpItemName, PUINT32 nItemID);
typedef INT32 (*ASIRUI_DelCpuItemType)(INT32 nItemID);

typedef INT32 (*ASIRUI_GetMemInfoType)(PUINT64 nTotalPhySize, PUINT64 nTotalPageSize, PUINT64 nAvailPhySize, PUINT64 nAvailPageSize);
typedef INT32 (*ASIRUI_GetMemInfoItemByNameType)(LPCTSTR lpItemName, PUINT64 nTotalMemSize, PUINT64 nUsageMemSize);
typedef INT32 (*ASIRUI_GetMemInfoItemByIDType)(UINT32 nProcID, PUINT64 nTotalMemSize, PUINT64 nUsageMemSize);

typedef INT32 (*ASIRUI_GetHddInfoType)(PUINT64 nTotalHddSize, PUINT64 nUsageHddSize, LPCTSTR lpDriveName);

typedef INT32 (*ASIRUI_GetTrafficBandWidhtType)(INT32 nItemID, DOUBLE* dBandWidth);
typedef INT32 (*ASIRUI_GetTrafficValueType)(INT32 nItemID, DOUBLE* dTrafficValue);

//---------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_RUI_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
