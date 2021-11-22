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
 * * 
 */

// ValidTimeUtil.h: interface for the CValidTimeUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALIDTIMEUTIL_H__7CA430D1_302A_4318_9017_7F8963AA8BBD__INCLUDED_)
#define AFX_VALIDTIMEUTIL_H__7CA430D1_302A_4318_9017_7F8963AA8BBD__INCLUDED_

class CValidTimeUtil  
{
private:
	DWORD		m_nValidTickCnt;
	UINT32		m_nValidTime;

public:
	void		InitValidTime(UINT32 nCurTime);
	UINT32		GetValidTime();

public:
	CValidTimeUtil();
	virtual ~CValidTimeUtil();

};

extern CValidTimeUtil* t_ValidTimeUtil;

#endif // !defined(AFX_VALIDTIMEUTIL_H__7CA430D1_302A_4318_9017_7F8963AA8BBD__INCLUDED_)
