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

#ifndef ManagePoHostWebSockH
#define ManagePoHostWebSockH
//---------------------------------------------------------------------------

class CManagePoHostWebSock : public CManagePoBase<DB_PO_HOST_WEB_SOCK>
{
public:
	TMapID					m_tConWebMap;

public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data);
    INT32					EditPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data);
	INT32					DelPoHostWebSock(UINT32 nID);
	INT32					ApplyPoHostWebSock(DB_PO_HOST_WEB_SOCK&	data);

public:
	String					GetName(UINT32 nID);   
	UINT32					SetPolicyType(TMapID64& tTypeMap, UINT64 nPolicy);
	UINT32					DelPolicyType(TMapID64& tTypeMap, UINT64 nPolicy);
	UINT32					GetPolicyType(TMapID64& tTypeMap, UINT64 nPolicy);

	UINT64					GetPolicyOption(TMapID64& tOptMap, UINT64 nPolicy);
	UINT64					SetPolicyOption(TMapID64& tOptMap, UINT64 nPolicy, UINT64 nOpt);

public:
	INT32					AddConWeb(UINT32 nCliPort);
	INT32					DelConWeb(UINT32 nCliPort);
	INT32					CntConWeb();

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_HOST_WEB_SOCK pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_HOST_WEB_SOCK& data);

public:
	CManagePoHostWebSock();
    ~CManagePoHostWebSock();

};

extern CManagePoHostWebSock*	t_ManagePoHostWebSock;

#endif
