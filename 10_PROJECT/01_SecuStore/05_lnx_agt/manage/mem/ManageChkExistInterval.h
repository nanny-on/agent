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

#ifndef ManageChkExistIntervalH
#define ManageChkExistIntervalH
//---------------------------------------------------------------------------

class CManageChkExistInterval : public CManageBase<MEM_CHK_EXIST_INTERVAL>
{
private:
	CMutexExt			m_tMutex;
	UINT32				m_nGlobalID;

public:
	INT32				InitChkExistInterval();
	INT32				AddChkExistInterval(MEM_CHK_EXIST_INTERVAL tMCEI);
	INT32				SetChkExistStatus(String strChkName, UINT32 nStatus);

public:
	INT32				IsChkExistStatus(String strChkName);

public:
	CManageChkExistInterval();
    ~CManageChkExistInterval();

};

extern CManageChkExistInterval	t_ManageChkExistInterval;

#endif
