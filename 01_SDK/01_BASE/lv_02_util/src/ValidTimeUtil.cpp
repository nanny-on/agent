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

// ValidTimeUtil.cpp: implementation of the CValidTimeUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "ValidTimeUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CValidTimeUtil* t_ValidTimeUtil;

CValidTimeUtil::CValidTimeUtil()
{
	m_nValidTime = 0;
	m_nValidTickCnt = 0;
}
//---------------------------------------------------------------------

CValidTimeUtil::~CValidTimeUtil()
{
	m_nValidTime = 0;
	m_nValidTickCnt = 0;
}
//---------------------------------------------------------------------

void		CValidTimeUtil::InitValidTime(UINT32 nCurTime)
{
	m_nValidTime = nCurTime;
	m_nValidTickCnt = GetTickCount();
	return;
}
//---------------------------------------------------------------------

UINT32		CValidTimeUtil::GetValidTime()
{
	UINT32 nRtnTime = 0;

	if(m_nValidTime)
	{
		nRtnTime = m_nValidTime + ((GetTickCount() - m_nValidTickCnt) / 1000);
	}
	else
	{
		nRtnTime = GetCurrentDateTimeInt();
	}
	return nRtnTime;
}
//---------------------------------------------------------------------
