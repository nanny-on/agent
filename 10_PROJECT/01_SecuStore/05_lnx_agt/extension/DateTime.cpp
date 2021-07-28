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


#include "stdafx.h"
#include "com_struct.h"
#include "DateTime.h"


CTime::CTime(void)
{
	m_dt = time(NULL);
}


CTime::CTime(time_t tTime)
{
	m_dt = tTime;
}


CTime::~CTime(void)
{
}


int CTime::GetYear(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_year + 1900;
}


int CTime::GetMonth(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_mon+1;
}


int CTime::GetDay(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_mday;
}


int CTime::GetHour(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_hour;
}


int CTime::GetMinute(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_min;
}


int CTime::GetSecond(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_sec;
}


int CTime::GetDayOfWeek(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_wday;
}


int CTime::GetDayOfYear(void)
{
	struct tm tm_time;
	localtime_r(&m_dt, &tm_time);

	return tm_time.tm_yday;
}


