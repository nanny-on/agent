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

// PowerUtil.h: interface for the CPowerUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERUTIL_H__06625C07_394E_4FDB_8F0B_C18A792E1D4A__INCLUDED_)
#define AFX_POWERUTIL_H__06625C07_394E_4FDB_8F0B_C18A792E1D4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPowerUtil  
{
private:
	BOOL	GrabPrivilege (LPCTSTR lpctPrivName);

public:	
	BOOL	SystemShutDown();	
	BOOL	RestartSystem();
	BOOL	SetSystemPower(UINT32 nFlag, DWORD dwReason);
	void	ChangeDisplayInfo(INT32 nX, INT32 nY, INT32 nQuality);
	void	MonitorPowerOff(HWND hWnd = NULL);
	void	MonitorPowerOn(HWND hWnd = NULL);
public:
	CPowerUtil();
	virtual ~CPowerUtil();

};

extern CPowerUtil*	t_PowerUtil;

#endif // !defined(AFX_POWERUTIL_H__06625C07_394E_4FDB_8F0B_C18A792E1D4A__INCLUDED_)
