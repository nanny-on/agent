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

#ifndef ManageDeviceMediaH
#define ManageDeviceMediaH

//---------------------------------------------------------------------------

class CManageDeviceMedia : public CManageBase<MEM_LOGIC_TIMER>
{
public:
	CManageDeviceMedia();
	~CManageDeviceMedia();

public:
	BOOL		Init(HWND hNotifyWnd, UINT nNotifyMessage);
	INT32		SetPkt(MemToken& SendToken);
	VOID		OnMediaChange(WPARAM wParam, LPARAM lParam);

private:
	INT32		Release();

	INT32		AttachInstance(LPCTSTR lpszDrv);
	INT32		DetachInstance(LPCTSTR lpszDrv);

private:
	HWND		m_hNotifyWnd;
	UINT		m_uNotifyMessage;
	ULONG		m_uRegisterNotifyMessage;
};

extern CManageDeviceMedia*	t_ManageDeviceMedia;

#endif
