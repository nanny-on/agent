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

#ifndef _RES_INFO_H__
#define _RES_INFO_H__

extern "C" {

INT32 GetCpuUsageTotal(UINT32* nUsage);
INT32 GetMemoryInfo(UINT64* nTotalPhySize, UINT64* nTotalPageSize, UINT64* nAvailPhySize, UINT64* nAvailPageSize);
INT32 GetHddInfo(UINT64* nTotalHddSize, UINT64* nUsageHddSize, LPCTSTR lpDriveName);
INT32 GetTrafficBandWidht(INT32 nItemID, DOUBLE* dBandWidth);
INT32 GetTrafficValue(INT32 nItemID, DOUBLE* dTrafficValue);

}

#endif /*_RES_INFO_H__*/