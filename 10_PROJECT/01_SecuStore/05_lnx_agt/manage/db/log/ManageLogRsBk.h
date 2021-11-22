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

#ifndef ManageLogRsBkH
#define ManageLogRsBkH
//---------------------------------------------------------------------------

class CManageLogRsBk : public CManageBase<DB_LOG_RS_BK>
{

private:
	UINT64					m_nTBkSize;

public:
	INT32					LoadDBMS();

public:
	INT32					AddLogRsBk(DB_LOG_RS_BK&	data);
    INT32					EditLogRsBk(DB_LOG_RS_BK&	data);
    INT32					DelLogRsBk(UINT32 nID);
	INT32					ApplyLogRsBk(DB_LOG_RS_BK&	data);

public:
	INT32					GetItemIDListCnt(TListID& tIDList, UINT32 nCnt, UINT32 nDirection = 0);
	INT32					GetKeyListDay(TListID& tIDList, UINT32 nDay, UINT32 nDirection = 0);
	INT32					GetItemIDMapDay(TMapID& tIDMap, UINT32 nDay, UINT32 nDirection = 0);

public:
	INT32					GetTodayLogRsBkID(DB_LOG_RS_BK&	data);
	PDB_LOG_RS_BK			GetTodayLogRsBk(DB_LOG_RS_BK&	data);
	INT32					CompareLogRsBkLast(DB_LOG_RS_BK& data);

	INT32					GetOldRsBk(PDB_LOG_RS_BK pdata, TListID& tIDList, UINT32 nRemainNum = 5);
	INT32					GetOldRsBk(String strKey, TListID& tIDList, UINT32 nRemainNum = 5);
	INT32					GetFirstRsBk(String strKey, TListID& tIDList, UINT32 nRemainNum = 1);
	UINT64					GetTBkSize()	{	return m_nTBkSize;	};

public:
	INT32					GetInitPktList(UINT64 nKey, TListDBLogRsBk& tInitPktList, UINT32 nOnceNum);
	INT32					SetPkt(MemToken& SendToken, UINT64 nKey, UINT32 nOnceNum);
	
public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(PDB_LOG_RS_BK pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LOG_RS_BK& data);

public:
	INT32					SetPkt_Link(MemToken& SendToken, INT32 nMaxDay = 6);

public:
	CManageLogRsBk();
    ~CManageLogRsBk();

};

extern CManageLogRsBk*	t_ManageLogRsBk;

#endif
