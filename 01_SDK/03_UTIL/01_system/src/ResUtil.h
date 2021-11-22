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

// ResUtil.h: interface for the CResUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESUTIL_H__8FE18D99_FD75_4EAE_A3AC_42ACB6837F14__INCLUDED_)
#define AFX_RESUTIL_H__8FE18D99_FD75_4EAE_A3AC_42ACB6837F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResUtil  
{
public:
	INT32 SaveRes(LPCTSTR strResType, LPCTSTR strResName, LPCTSTR strSaveFileName);
public:
	CResUtil();
	virtual ~CResUtil();

};

#endif // !defined(AFX_RESUTIL_H__8FE18D99_FD75_4EAE_A3AC_42ACB6837F14__INCLUDED_)
