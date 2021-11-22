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


#ifndef _AS_TIME_H__
#define	_AS_TIME_H__

typedef struct _TIMSTM
{
	char        chYear[4];
	char        ch4;
	char        chMonth[2];
	char        ch7;
	char        chDay[2];
	char        ch10;
	char        chHour[2];
	char        ch13;
	char        chMinute[2];
	char        ch16;
	char        chSecond[2];
	char        ch19;
	char        chMicroSecond[6];
	char        chNull;
}TIMSTM, *PTIMSTM;

#define Int32x32To64(a, b)	(((int64_t)((long)(a))) * ((int64_t)((long)(b))))
#define UInt32x32To64(a, b) (((uint64_t)((unsigned int)(a))) * ((uint64_t)((unsigned int)(b))))

static const LONGLONG SECS_BETWEEN_EPOCHS = 11644473600;
static const LONGLONG SECS_TO_100NS = 10000000; /* 10^7 */

#define		ONE_HOUR		3600

extern void		UnixTimeToFileTime(time_t* pt, LPFILETIME pft);
extern void		FileTimeToUnixTime(LPFILETIME pft, time_t* pt);
extern void		UnixTimeToSystemTime(time_t* pt, LPSYSTEMTIME pst);
extern void		SystemTimeToUnixTime(LPSYSTEMTIME pst, time_t* pt);
extern void		FileTimeToDBTime( LPFILETIME pft, PTIMSTM pts );
extern void		DBTimeToFileTime( PTIMSTM pts, LPFILETIME pft );
extern void		DBTimeToSystemTime(PTIMSTM pt, LPSYSTEMTIME pst);
extern void		SystemTimeToDBTime(LPSYSTEMTIME pst, PTIMSTM pt);
extern void		UnixTimeToDBTime(time_t* pt, PTIMSTM pst);
extern void		DBTimeToUnixTime(PTIMSTM pst, time_t* pt);
extern time_t	LocalTimeToGlobalTime(time_t& nTime);
extern time_t	GlobalTimeToLocalTime(time_t& nTime);
extern void		GetDateTimeByIndex(INT32 nIndex, OUT LPTSTR lpBuf);
extern void		GetCurrentDateTime(INT32 nDateTime, OUT LPTSTR lpBuf);
extern UINT32	GetCurrentDateTimeInt();
extern UINT32	GetCurrentTimeZone();
extern INT32	GetFileTimeInfo(LPCSTR pcPath, UINT32 *pdwCreateTime, UINT32 *pdwModifyTime, UINT32 *pdwAccessTime);
extern UINT32	GetCurrentYearMon();
extern UINT32	GetCurrentYearMon(UINT32 nTime);
extern UINT32	GetCurrentYearMonDay(UINT32 nShort = 0);
extern UINT32	GetCurrentYearMonDay(UINT32 nTime, UINT32 nShort);
extern UINT32	GetCurrentYearMonDayHour(UINT32 nShort = 0);
extern UINT32	GetCurrentYearMonDayHour(UINT32 nTime, UINT32 nShort);
extern UINT32	GetCurrentYearMonDayHourMin();
extern UINT32	GetCurrentYearMonDayHourMin(UINT32 nTime);
extern UINT32	GetDayOfWeek(UINT32 nTime, UINT32 nType);
extern UINT32	GetDayOfDay(UINT32 nTime, UINT32 nType);
extern UINT32	GetDayOfMonth(UINT32 nTime);
extern UINT32	GetDayOfMonth(UINT32 nTime, UINT32 nType);
extern UINT32	GetDayDiffCurrent(UINT32 nTime);
extern LPCTSTR	GetFormatTime(UINT32 nTime, LPTSTR lpBuf, LPTSTR lpFormat = 0);
extern LPTSTR	GetDateTimeFmt(UINT32 nTime, OUT LPTSTR lpBuf, INT32 nFormat = 0);
extern UINT32	GetWeekCntToday();
extern UINT32	GetWeekCntToday(UINT32 nStartDoW);
extern UINT32	GetWeekCnt(UINT32 nDay, UINT32 nWeek, UINT32 nStartDoW = 0/*Sunday*/);
extern UINT32	GetWeekCntDay(UINT32 nCurDay, UINT32 nCurDow, UINT32 nSdtDoW, UINT32 nWeekCnt, UINT32 nStartDow);
extern int		uptime();
extern double	diff_time(struct timeval stStartTime);

#endif /*_AS_TIME_H__*/


