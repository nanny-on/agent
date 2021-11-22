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

// RemoveLogDLLUtil.h: interface for the CRemoveLogDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REMOVELOGDLLUTIL_H__3A5F83D0_C90F_4B73_83D7_9499C82D5921__INCLUDED_)
#define AFX_REMOVELOGDLLUTIL_H__3A5F83D0_C90F_4B73_83D7_9499C82D5921__INCLUDED_

class CRemoveLogDLLUtil  
{

private:
	HINSTANCE m_hDLL;

private:
	RemoveFileByDTType			f_RemoveFileByDT;
    RemoveFileByLastDayType     f_RemoveFileByLastDay;
	
public:
	INT32		LoadLibraryExt(LPCTSTR lpDLLPath);

public:
	INT32		RemoveFileByDT(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay);
    void		RemoveFileByLastDay(LPCTSTR lpFindPath, UINT32 nChkType, UINT32 nDay);

public:
	CRemoveLogDLLUtil();
	virtual ~CRemoveLogDLLUtil();

};

extern CRemoveLogDLLUtil*	t_RemoveLogDLLUtil;

#endif // !defined(AFX_REMOVELOGDLLUTIL_H__3A5F83D0_C90F_4B73_83D7_9499C82D5921__INCLUDED_)
