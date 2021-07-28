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

#ifndef ManageEnvNotifyInfoH
#define ManageEnvNotifyInfoH
//---------------------------------------------------------------------------

class CManageEnvNotifyInfo : public CManagePoBase<DB_ENV_NOTIFY_INFO>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();
	INT32					GetHash(UINT32 nID, String& strOrgValue);

public:
	INT32					AddEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni);
    INT32					EditEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni);
	INT32					DelEnvNotifyInfo(UINT32 nID);
	INT32					ApplyEnvNotifyInfo(DB_ENV_NOTIFY_INFO&	deni);

public:
	void					AddEnvNotifyInfoNotiTime(UINT32 nPos, UINT32 nID, UINT32 nNotifyID, UINT32 nTimeStamp);
	INT32					GetEnvNotifyInfoNotiTime(UINT32 nPos, UINT32 nID, UINT32 nNotifyID);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_ENV_NOTIFY_INFO pdeni, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_ENV_NOTIFY_INFO& deni);

	INT32					SetPkt_Link(MemToken& SendToken);
	INT32					SetPkt_Link(PDB_ENV_NOTIFY_INFO pdeni, MemToken& SendToken);


public:
	INT32					IsExcludeNotifySchdule(UINT32 nPolType, UINT32 nPoUnitID = 0);
	INT32					GetIDLIstByNotifyPolType(UINT32 nNotifyPolType, TListID& tIDList);

public:
	CManageEnvNotifyInfo();
    ~CManageEnvNotifyInfo();

};

extern CManageEnvNotifyInfo*	t_ManageEnvNotifyInfo;

#endif
