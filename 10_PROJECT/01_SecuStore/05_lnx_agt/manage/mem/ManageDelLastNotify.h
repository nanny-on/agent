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

#ifndef ManageDelLastNotifyH
#define ManageDelLastNotifyH
//---------------------------------------------------------------------------

class CManageDelLastNotify : public CManageBase<MEM_FIND_ORDER_INFO>
{
private:
	CMutexExt			m_tMutex;
	UINT32				m_nGID;

public:
	INT32				AddDelLastNotify(MEM_FIND_ORDER_INFO& tMFOI);
	INT32				AddDelLastNotify(UINT32 nPolicyType, UINT32 nOpType, UINT32 nTotalDelCnt);
	INT32				DelDelLastNotify(UINT32 nID);

public:
	INT32				IsRemainNotify();

public:
	INT32				GetRemainNotifyList(TListMemFindOrderInfo& tMemFindOrderInfoList);

public:
	CManageDelLastNotify();
    ~CManageDelLastNotify();

};

extern CManageDelLastNotify*	t_ManageDelLastNotify;

#endif
