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

// PowerUtil.cpp: implementation of the CPowerUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "PowerUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------

CPowerUtil*	t_PowerUtil = NULL;

//--------------------------------------------------------------------
CPowerUtil::CPowerUtil()
{

}
//--------------------------------------------------------------------

CPowerUtil::~CPowerUtil()
{

}
//--------------------------------------------------------------------

BOOL CPowerUtil::GrabPrivilege (LPCTSTR lpctPrivName) 
{ 
	BOOL bRetVal = FALSE; 
	return bRetVal; 
}
//--------------------------------------------------------------------

BOOL CPowerUtil::SystemShutDown()
{
	return FALSE;
}
//--------------------------------------------------------------------

BOOL CPowerUtil::RestartSystem()
{
	return FALSE;	
}
//---------------------------------------------------------------------------

BOOL	CPowerUtil::SetSystemPower(UINT32 nFlag, DWORD dwReason)
{
	return FALSE;
}
//---------------------------------------------------------------------------

void	CPowerUtil::ChangeDisplayInfo(INT32 nX, INT32 nY, INT32 nQuality)
{
}
//---------------------------------------------------------------------------

void	CPowerUtil::MonitorPowerOff(HWND hWnd)
{
	return;
}
//---------------------------------------------------------------------------

void	CPowerUtil::MonitorPowerOn(HWND hWnd)
{
	return;
}
//---------------------------------------------------------------------------


