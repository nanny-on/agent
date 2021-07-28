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

//---------------------------------------------------------------------------

#ifndef ManageEvtMonH
#define ManageEvtMonH
//---------------------------------------------------------------------------

class CManageEvtMon : public CManageBase<MEM_EVT_MON>
{
private:
	CMutexExt			m_tMutex;
	UINT32				m_nGID;

public:
	INT32				AddEvtMon(UINT32 nClass, UINT32 nType, String strName, UINT32 nCode = 0);
	INT32				DelEvtMon(String trName);

public:
	INT32				GetOverTimeEvtMon(TListMemEvtMon& tEvtMonList);

public:
	CManageEvtMon();
    ~CManageEvtMon();

};

extern CManageEvtMon	t_ManageEvtMon;

#endif
