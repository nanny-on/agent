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


#include "stdafx.h"
#include "com_struct.h"
#include "ComGlobalMethod.h"

//-------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

INT32	GetUnitIDtoPolicyType(INT32 nPolicyType)
{
	INT32 nUnitID = 0;
	nUnitID = nPolicyType%1000;

	return nUnitID;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	GetPoGroupIDtoPolicyType(INT32 nPolicyType)
{
	INT32 nType = nPolicyType/1000;
	nType *= 1000;
	return nType;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	GetDayDiffWeek(INT32 nStWeek, INT32 nEdWeek)
{
	INT32 nCount = 0;
	INT32 nWeek = nStWeek;
	for(; nWeek != nEdWeek; nWeek++)
	{
		nWeek = nWeek % 7;
		nCount++;
	}

	return nCount;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	GetHourDiffHour(INT32 nStTime, INT32 nEdTime)
{
	INT32 nCount = 0;
	INT32 nTime = nStTime;
	for(; nTime != nEdTime; nTime++)
	{
		nTime = nTime % 24;
		nCount++;
	}

	return nCount;
}
//---------------------------------------------------------------------------------------------------------------------

INT32	GetMinDiffMin(INT32 nStTime, INT32 nEdTime)
{
	INT32 nCount = 0;
	INT32 nTime = nStTime;
	for(; nTime != nEdTime; nTime++)
	{
		nTime = nTime % 60;
		nCount++;
	}

	return nCount;
}
//---------------------------------------------------------------------------------------------------------------------

INT32   GetTimeIntAddMonth(INT32 nTime, INT32 nAddMonth)
{
	struct tm *dt;
	time_t tInput = (time_t)nTime;
	dt = localtime(&tInput);

	if(nAddMonth >= 12)
		return 0;

	dt->tm_mon += nAddMonth;
	if(dt->tm_mon > 12)
	{
		dt->tm_mon = dt->tm_mon % 12;
		dt->tm_year += 1;
	}

	time_t t;
	t = mktime(dt);

	struct timeb tstruct;
	ftime(&tstruct);

	return UINT32(t + (tstruct.timezone * 60 * -1));
}
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------

String ConvertStrToInt(UINT32 nValue)
{
	String strValue;
	strValue = SPrintf("%d", nValue);

	return strValue;
}

