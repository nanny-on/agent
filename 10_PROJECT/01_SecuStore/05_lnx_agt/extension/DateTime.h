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

#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <time.h>

class CTime
{
private:
	enum TimeStatus
	{
		error = -1,
		valid = 0,
		invalid = 1,    // Invalid date (out of range, etc.)
		null = 2,       // Literally has no value
	};

	time_t m_dt;
	TimeStatus m_status;

public:
	CTime(void);
	CTime(time_t tTime);
	~CTime(void);

	int GetYear(void);
	int GetMonth(void);
	int GetDay(void);
	int GetHour(void);
	int GetMinute(void);
	int GetSecond(void);
	int GetDayOfWeek(void);
	int GetDayOfYear(void);
};


#endif


