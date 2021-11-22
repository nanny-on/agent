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

#ifndef COM_GLOBAL_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984
#define COM_GLOBAL_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984

INT32	GetUnitIDtoPolicyType(INT32 nPolicyType);
INT32	GetPoGroupIDtoPolicyType(INT32 nPolicyType);

INT32	GetDayDiffWeek(INT32 nStWeek, INT32 nEdWeek);
INT32	GetHourDiffHour(INT32 nStTime, INT32 nEdTime);
INT32	GetMinDiffMin(INT32 nStTime, INT32 nEdTime);
INT32   GetTimeIntAddMonth(INT32 nTime, INT32 nAddMonth);
String ConvertStrToInt(UINT32 nValue);
#endif //COM_GLOBAL_METHOD_H_15BE18C4_D543_413E_96F8_61C24AD45984