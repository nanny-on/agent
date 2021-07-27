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
 */


#include "stdafx.h"
#include "as_string.h"
#include "as_time.h"

void UnixTimeToFileTime(time_t* pTime, LPFILETIME pFileTime)
{
	LONGLONG llValue = 0; // 64 bit value
	if(pTime == NULL || pFileTime == NULL)
		return;
	llValue = Int32x32To64(*pTime, SECS_TO_100NS) + SECS_BETWEEN_EPOCHS;
	pFileTime->dwLowDateTime = (DWORD)llValue;
	pFileTime->dwHighDateTime = (DWORD)(llValue >> 32);
}

//-------------------------------------------------------------------------

// This function converts the FILETIME structure to the 32 bit
// Unix time structure.
// The time_t is a 32-bit value for the number of seconds since
// January 1, 1970. A FILETIME is a 64-bit for the number of
// 100-nanosecond periods since January 1, 1601. Convert by
// subtracting the number of 100-nanosecond period betwee 01-01-1970
// and 01-01-1601, from time_t the divide by 1e+7 to get to the same
// base granularity.

void FileTimeToUnixTime(LPFILETIME pFileTime, time_t* pTime)
{
	LONGLONG llValue = 0; // 64 bit value
	if(pTime == NULL || pFileTime == NULL)
		return;
	llValue = MAKELONGLONG(pFileTime->dwLowDateTime, pFileTime->dwHighDateTime);
	*pTime = (time_t)((llValue - SECS_BETWEEN_EPOCHS)/SECS_TO_100NS);
}
//-------------------------------------------------------------------------

// This function converts the 32 bit Unix time structure to
// the SYSTEMTIME structure

void UnixTimeToSystemTime(time_t* pTime, LPSYSTEMTIME pSystemTime)
{
	struct tm *pLocalTime = NULL;
	if(pTime == NULL||pSystemTime==NULL)
		return;
	pLocalTime = localtime(pTime);
	if(pLocalTime != NULL)
	{
		pSystemTime->wYear = (WORD)pLocalTime->tm_year+1900;
		pSystemTime->wMonth = (WORD)pLocalTime->tm_mon+1;
		pSystemTime->wDayOfWeek = (WORD)pLocalTime->tm_wday;
		pSystemTime->wDay = (WORD)pLocalTime->tm_mday;
		pSystemTime->wHour = (WORD)pLocalTime->tm_hour;
		pSystemTime->wMinute = (WORD)pLocalTime->tm_min;
		pSystemTime->wSecond = (WORD)pLocalTime->tm_sec;
		pSystemTime->wMilliseconds = 0;
	}
}
//-------------------------------------------------------------------------

// This function coverts the SYSTEMTIME structure to
// the 32 bit Unix time structure
void SystemTimeToUnixTime(LPSYSTEMTIME pSystemTime, time_t* pTime)
{
	struct tm stLocalTime;
	if(pSystemTime == NULL||pTime==NULL)
		return;

	memset(&stLocalTime, 0, sizeof(struct tm));
	stLocalTime.tm_year = (int)pSystemTime->wYear-1900;
	stLocalTime.tm_mon = (int)pSystemTime->wMonth-1;
	stLocalTime.tm_wday = (int)pSystemTime->wDayOfWeek;
	stLocalTime.tm_mday = (int)pSystemTime->wDay;
	stLocalTime.tm_hour = (int)pSystemTime->wHour;
	stLocalTime.tm_min = (int)pSystemTime->wMinute;
	stLocalTime.tm_sec = (int)pSystemTime->wSecond;
	*pTime = mktime(&stLocalTime);
}
//-------------------------------------------------------------------------

void FileTimeToDBTime( LPFILETIME pFileTime, PTIMSTM pSqlTime )
{
	LONG lMicroseconds = 0;
	time_t tTime = 0;
	char acSqlTime[64] = {0,};
	SYSTEMTIME stSysTime;

	if(pFileTime == NULL || pSqlTime == NULL)
		return;

	memset(&stSysTime, 0, sizeof(SYSTEMTIME));
	FileTimeToUnixTime(pFileTime, &tTime);
	UnixTimeToSystemTime(&tTime, &stSysTime);
	snprintf(acSqlTime, 63, "%04hd-%02hd-%02hd %02hd:%02hd:%02hd.%06ld",
		stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute,
		stSysTime.wSecond, lMicroseconds );
	acSqlTime[63] = 0;
	memcpy(pSqlTime, (PTIMSTM)acSqlTime, sizeof(TIMSTM));
}
//-------------------------------------------------------------------------

// This function converts the SQL database time string to the
// FILETIME structure. Could call it SqlTimeToFileTime.
void DBTimeToFileTime( PTIMSTM pSqlTime, LPFILETIME pFileTime )
{
	LONG lMicroseconds = 0;
	SYSTEMTIME stSysTime;
	time_t tTime = 0;

	if(pFileTime == NULL || pSqlTime == NULL)
		return;

	memset(&stSysTime, 0, sizeof(SYSTEMTIME));

	sscanf((char *)pSqlTime, "%hd-%hd-%hd %hd:%hd:%hd.%ld", &stSysTime.wYear, &stSysTime.wMonth,
		&stSysTime.wDay, &stSysTime.wHour, &stSysTime.wMinute, &stSysTime.wSecond, &lMicroseconds );
	SystemTimeToUnixTime(&stSysTime, &tTime);
	UnixTimeToFileTime(&tTime, pFileTime);
}
//-------------------------------------------------------------------------

// This function converts the SQL database time string to
// the SYSTEMTIME structure aka SqlTimeToSystemTime
void DBTimeToSystemTime(PTIMSTM pSqlTime, LPSYSTEMTIME pSystemTime)
{
	LONG lMicroseconds = 0;
	if(pSystemTime == NULL || pSqlTime == NULL)
		return;
	sscanf((char *)pSqlTime, "%hd-%hd-%hd %hd:%hd:%hd.%ld", &pSystemTime->wYear, &pSystemTime->wMonth,
		&pSystemTime->wDay, &pSystemTime->wHour, &pSystemTime->wMinute, &pSystemTime->wSecond, &lMicroseconds );
}
//-------------------------------------------------------------------------

// This function coverts the SYSTEMTIME structure to
// the SQL database time string aka SystemTimeToSqlTime
void SystemTimeToDBTime(LPSYSTEMTIME pSystemTime, PTIMSTM pSqlTime)
{
	LONG lMicroseconds = 0;
	int nLen = 0;
	if(pSystemTime == NULL || pSqlTime == NULL)
		return;
	nLen = sizeof(TIMSTM);
	snprintf( (char *)pSqlTime, nLen-1, "%04hd-%02hd-%02hd %02hd:%02hd:%02hd.%06ld",
		pSystemTime->wYear, pSystemTime->wMonth, pSystemTime->wDay, pSystemTime->wHour, pSystemTime->wMinute,
		pSystemTime->wSecond, lMicroseconds );
	pSqlTime->chNull = 0;
}
//-------------------------------------------------------------------------

// This function converts the 32 bit Unix time structure to
// the SQL database time string aka UnixTimeToSqlTime
void UnixTimeToDBTime(time_t* pt, PTIMSTM pst)
{
	FILETIME ft;
	UnixTimeToFileTime(pt, &ft);
	FileTimeToDBTime(&ft, pst);
}
//-------------------------------------------------------------------------

// This function coverts the SQL database string to
// the 32 bit Unix time structure aka SqlTimeToUnixTime
void DBTimeToUnixTime(PTIMSTM pst, time_t* pt)
{
	FILETIME ft;
	DBTimeToFileTime(pst, &ft);
	FileTimeToUnixTime(&ft, pt);
}
//-------------------------------------------------------------------------

time_t LocalTimeToGlobalTime(time_t& nTime)
{
	struct timeb tstruct;
	ftime(&tstruct);

	return (nTime - (tstruct.timezone * 60 * -1));
}
//-------------------------------------------------------------------------

time_t GlobalTimeToLocalTime(time_t& nTime)
{
	struct timeb tstruct;
	ftime(&tstruct);

	return (nTime + (tstruct.timezone * 60 * -1));
}
//-------------------------------------------------------------------------

void			GetCurrentDateTime(INT32 nDateTime, OUT LPTSTR lpBuf)
{
	time_t t;
	struct tm *dt = NULL;
	t = time(NULL);
	dt = localtime(&t);

	if(dt == NULL)
		return;

	if(nDateTime == 0)
		snprintf(lpBuf, MAX_TIME_STR-1, "%.4d_%.2d_%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);
	else if(nDateTime == 2)
		snprintf(lpBuf, MAX_TIME_STR-1, "%.4d%.2d%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);
	else
		snprintf(lpBuf, MAX_TIME_STR-1, "%.4d_%.2d_%.2d %.2d:%.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
}

void GetDateTimeByIndex(INT32 nIndex, OUT LPTSTR lpBuf)
{
	time_t t;
	struct tm *dt = NULL;
	if(nIndex < 1)
		return;

	t = time(NULL)-nIndex*60*60*24;
	dt = localtime(&t);
	if(dt == NULL)
		return;

	snprintf(lpBuf, MAX_TIME_STR-1, "%.4d_%.2d_%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);
}

UINT32	GetCurrentYearMonDayHour(UINT32 nShort)
{
	time_t t;
	struct tm *dt;
	t = time(NULL);
	dt = localtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		if(nShort)
		{
			nRtn = (((dt->tm_year + 1900) % 2000) * 1000000);
		}
		else
		{
			nRtn = (((dt->tm_year + 1900)) * 1000000);
		}
		nRtn += ((dt->tm_mon + 1) * 10000);
		nRtn += dt->tm_mday * 100;
		nRtn += dt->tm_hour;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetCurrentYearMonDayHour(UINT32 nTime, UINT32 nShort)
{
	if(!nTime)	return 0;

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		if(nShort)
		{
			nRtn = (((dt->tm_year + 1900) % 2000) * 1000000);
		}
		else
		{
			nRtn = (((dt->tm_year + 1900)) * 1000000);
		}
		nRtn += ((dt->tm_mon + 1) * 10000);
		nRtn += dt->tm_mday * 100;
		nRtn += dt->tm_hour;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetCurrentYearMonDayHourMin()
{
	time_t t;
	struct tm *dt;
	t = time(NULL);
	dt = localtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = (((dt->tm_year + 1900) % 2000) * 100000000);
		nRtn += ((dt->tm_mon + 1) * 1000000);
		nRtn += dt->tm_mday * 10000;
		nRtn += dt->tm_hour * 100;
		nRtn += dt->tm_min;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetCurrentYearMonDayHourMin(UINT32 nTime)
{
	if(!nTime)	return 0;

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = (((dt->tm_year + 1900) % 2000) * 100000000);
		nRtn += ((dt->tm_mon + 1) * 1000000);
		nRtn += dt->tm_mday * 10000;
		nRtn += dt->tm_hour * 100;
		nRtn += dt->tm_min;
	}
	return nRtn;
}

//-------------------------------------------------------------------------

UINT32	GetCurrentYearMonDay(UINT32 nShort)
{
	time_t t;
	struct tm *dt;
	t = time(NULL);
	dt = localtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		if(nShort)
		{
			nRtn = (((dt->tm_year + 1900) % 2000) * 10000);
		}
		else
		{
			nRtn = (((dt->tm_year + 1900)) * 10000);
		}
		nRtn += ((dt->tm_mon + 1) * 100);
		nRtn += dt->tm_mday;
	}
	return nRtn;
}

UINT32	GetCurrentYearMonDay(UINT32 nTime, UINT32 nShort)
{
	if(!nTime)	return 0;

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		if(nShort)
		{
			nRtn = (((dt->tm_year + 1900) % 2000) * 10000);
		}
		else
		{
			nRtn = (((dt->tm_year + 1900)) * 10000);
		}
		nRtn += ((dt->tm_mon + 1) * 100);
		nRtn += dt->tm_mday;
	}
	return nRtn;
}

//-------------------------------------------------------------------------

UINT32	GetCurrentYearMon()
{
	time_t t;
	struct tm *dt;
	t = time(NULL);
	dt = localtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = (dt->tm_year + 1900) * 100;
		nRtn += (dt->tm_mon + 1);
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetDayOfWeek(UINT32 nTime, UINT32 nType)
{
	struct tm *dt;
	time_t t = (time_t)nTime;
	if(!nType)
		dt = localtime(&t);
	else
		dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = dt->tm_wday;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetDayOfDay(UINT32 nTime, UINT32 nType)
{
	struct tm *dt;
	time_t t = (time_t)nTime;
	if(!nType)
		dt = localtime(&t);
	else
		dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = dt->tm_mday;
	}
	return nRtn;
}
//-------------------------------------------------------------------------

UINT32	GetWeekCntEx(UINT32 nDay, UINT32 nWeek, UINT32 nStartDoW)
{
	UINT32 nCurDay = nDay;
	UINT32 nOneDayWeek = nWeek;
	while(nCurDay != 1)
	{
		nCurDay -= 1;
		if(nOneDayWeek == 1)	nOneDayWeek = 7;
		else					nOneDayWeek -= 1;
	}

	UINT32 nInterval = 0;
	if (nStartDoW > nOneDayWeek)
		nInterval = nStartDoW - nOneDayWeek;
	else if (nStartDoW < nOneDayWeek)
		nInterval = 7 - (nOneDayWeek - nStartDoW);
	else
		nInterval = 0;

	if (nDay <= nInterval) 
		return 1;

	nOneDayWeek = nStartDoW;
	nCurDay += nInterval;

	UINT32 nWeekCnt = 1;
	while(nDay != nCurDay)
	{
		nCurDay += 1;
		if(nOneDayWeek == 7)
		{
			nOneDayWeek = 1;
			nWeekCnt += 1;
		}
		else
		{
			nOneDayWeek += 1;
		}
	}
	return nWeekCnt;
}
//-------------------------------------------------------------------------

UINT32	GetWeekCount(UINT32 nDay, UINT32 nWeek)
{
	UINT32 nCurDay = nDay;
	UINT32 nOneDayWeek = nWeek;
	while(nCurDay != 1)
	{
		nCurDay -= 1;
		if(nOneDayWeek == 1)	nOneDayWeek = 7;
		else					nOneDayWeek -= 1;
	}

	UINT32 nWeekCnt = 1;
	while(nDay != nCurDay)
	{
		nCurDay += 1;
		if(nOneDayWeek == 7)
		{
			nOneDayWeek = 1;
			nWeekCnt += 1;
		}
		else
		{
			nOneDayWeek += 1;
		}
	}
	return nWeekCnt;
}

UINT32	GetWeekCntToday()
{
	time_t t;
	struct tm *dt = NULL;
	t = time(NULL);
	dt = localtime(&t);
	if(dt == NULL)
		return 0;
	return GetWeekCount((UINT32)dt->tm_mday,(UINT32)dt->tm_wday);
}
//-------------------------------------------------------------------------

UINT32	GetWeekCntToday(UINT32 nStartDoW)
{
	time_t t;
	struct tm *dt = NULL;
	t = time(NULL);
	dt = localtime(&t);
	if(dt == NULL)
		return 0;
	return GetWeekCntEx((UINT32)dt->tm_mday,(UINT32)dt->tm_wday, nStartDoW);
}

UINT32	GetWeekCnt(UINT32 nDay, UINT32 nWeek, UINT32 nStartDoW)
{
	UINT32 nCurDay = nDay;
	UINT32 nOneDayWeek = nWeek;
	while(nCurDay != 1)
	{
		nCurDay -= 1;
		if(nOneDayWeek == 0)			nOneDayWeek = 6;
		else							nOneDayWeek -= 1;
	}

	UINT32 nWeekCnt = 0;
	while(nCurDay <= nDay)
	{
		if(nOneDayWeek == nStartDoW)
		{
			nWeekCnt += 1;
		}

		if(nOneDayWeek == 6)	nOneDayWeek = 0;
		else
		{
			nOneDayWeek += 1;
		}

		nCurDay += 1;
	}
	return nWeekCnt;
}
//-------------------------------------------------------------------------

UINT32	GetWeekCntDay(UINT32 nCurDay, UINT32 nCurDow, UINT32 nSdtDoW, UINT32 nWeekCnt, UINT32 nStartDow)
{
	UINT32 nMatchDay = 0;
	UINT32 nChkDay = nCurDay;
	UINT32 nOneDayWeek = nCurDow;
	while(nChkDay != 1)
	{
		nChkDay -= 1;
		if(nOneDayWeek == 0)			nOneDayWeek = 6;
		else							nOneDayWeek -= 1;
	}

	UINT32 nChkWeekCnt = 0;
	while(nChkDay <= nCurDay)
	{
		if(nOneDayWeek == nSdtDoW)
		{
			nChkWeekCnt += 1;
		}

		if(nChkWeekCnt == nWeekCnt)
		{
			if(nStartDow == nOneDayWeek)
			{
				nMatchDay = nChkDay;
				break;
			}
		}

		if(nOneDayWeek == 6)	nOneDayWeek = 0;
		else
		{
			nOneDayWeek += 1;
		}

		nChkDay += 1;
	}
	return nMatchDay;
}


UINT32	GetDayOfMonth(UINT32 nTime)
{
	struct tm *dt;
	time_t t = (time_t)nTime;
	dt = localtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = dt->tm_mday;
	}
	return nRtn;
}

UINT32	GetDayOfMonth(UINT32 nTime, UINT32 nType)
{
	struct tm *dt;
	time_t t = (time_t)nTime;
	if(!nType)
		dt = localtime(&t);
	else
		dt = gmtime(&t);

	UINT32 nRtn = 0;
	if(dt)
	{
		nRtn = dt->tm_mon + 1;
	}
	return nRtn;
}

//-------------------------------------------------------------------------

UINT32			GetCurrentYearMon(UINT32 nTime)
{
	if(!nTime)	return 0;

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	UINT32 nRtn = 0;

	if(dt)
	{
		nRtn = (dt->tm_year + 1900) * 100;
		nRtn += (dt->tm_mon + 1);
	}

	return nRtn;
}
//-------------------------------------------------------------------------
UINT32			GetDayDiffCurrent(UINT32 nTime)
{
	time_t tSetDate = nTime, tCurrent;
	time(&tCurrent);
	return (UINT32)((difftime(tCurrent, tSetDate))/(60*60*24));
}
//-------------------------------------------------------------------------

UINT32	GetCurrentDateTimeInt()
{
	UINT32 dwTime = 0;
	time_t t = 0;
	struct tm lt = {0,};
	time(&t);
	localtime_r(&t, &lt);

	dwTime = UINT32(t + lt.tm_gmtoff);
	return dwTime;
}

UINT32	GetCurrentTimeZone()
{
	time_t t = 0;
	struct tm lt = {0,};
	time(&t);
	localtime_r(&t, &lt);
	return (UINT32)lt.tm_gmtoff;
}


INT32 GetFileTimeInfo(LPCSTR pcPath, UINT32 *pdwCreateTime, UINT32 *pdwModifyTime, UINT32 *pdwAccessTime)
{
	struct timeb tstruct;
	struct stat stStat;
	if(pcPath == NULL || pdwCreateTime == NULL || pdwModifyTime == NULL || pdwAccessTime == NULL)
	{
		return -1;
	}
	if(stat(pcPath, &stStat) < 0)
	{
		return -2;
	}

	if (!S_ISREG(stStat.st_mode))
	{
		return -3;
	}
	ftime(&tstruct);
	*pdwCreateTime = (UINT32)(stStat.st_ctime + (tstruct.timezone * 60 * -1));
	*pdwModifyTime = (UINT32)(stStat.st_mtime + (tstruct.timezone * 60 * -1));
	*pdwAccessTime = (UINT32)(stStat.st_atime + (tstruct.timezone * 60 * -1));
	return 0;
}

//-------------------------------------------------------------------------

LPCTSTR	GetFormatTime(UINT32 nTime, LPTSTR lpBuf, LPTSTR lpFormat)
{
	struct timeb tstruct;
	ftime(&tstruct);

	time_t t = nTime - (tstruct.timezone * 60 * -1);
	struct tm *dt = localtime(&t);

	if(!dt)
	{
		return (LPCTSTR)lpBuf;
	}
	else if(lpFormat)
		sprintf_ext(CHAR_MAX_SIZE-1, lpBuf, lpFormat, dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
	else
		sprintf_ext(CHAR_MAX_SIZE-1, lpBuf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);
	return (LPCTSTR)lpBuf;
}
//-------------------------------------------------------------------------

LPTSTR			GetDateTimeFmt(UINT32 nTime, OUT LPTSTR lpBuf, INT32 nFormat)
{
	if(nTime < 10)	return lpBuf;

	time_t t = (time_t)nTime;
	struct tm *dt;
	dt = gmtime(&t);

	if(!dt)
	{
		return lpBuf;
	}

	switch(nFormat)
	{
		case 0:	sprintf_ext(22, lpBuf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);	break;
		case 1: sprintf_ext(12, lpBuf, "%.4d-%.2d-%.2d",dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);		break;
		case 2: sprintf_ext(10, lpBuf, "%.4d%.2d%.2d",dt->tm_year + 1900,dt->tm_mon + 1,dt->tm_mday);		break;
	}
	return lpBuf;
}


int uptime()
{
	double dStartTime = 0;
	double dIdletTme = 0;
	char acBuf[40] = {0,};
	FILE *fp = NULL;
	if ((fp = fopen("/proc/uptime", "r")) == NULL)
	{
		return -1;
	}

	if(fgets(acBuf, 36, fp) == NULL)
	{
		fclose(fp);
		return -1;
	}

	sscanf(acBuf, "%lf %lf", &dStartTime, &dIdletTme);
	fclose(fp);

	return (int)dStartTime;
}

double diff_time(struct timeval stStartTime)
{
	struct timeval stEndTime;
	double fDiffTime = 0;
	if(stStartTime.tv_sec > 1)
	{
		gettimeofday(&stEndTime, NULL);
		fDiffTime = ( stEndTime.tv_sec - stStartTime.tv_sec )*1000000 + ( stEndTime.tv_usec - stStartTime.tv_usec );
	}
	return fDiffTime;
}

//-------------------------------------------------------------------------

