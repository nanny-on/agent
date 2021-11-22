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
//---------------------------------------------------------------------------

#ifndef ManageSiteFileAliasH
#define ManageSiteFileAliasH
//---------------------------------------------------------------------------

class CManageSiteFileAlias : public CManageBase<DB_SITE_FILE_ALIAS>
{

public:
	INT32			LoadDBMS();

public:
	INT32						AddSiteFileAlias(DB_SITE_FILE_ALIAS& data);
	INT32						EditSiteFileAlias(DB_SITE_FILE_ALIAS& data);
    INT32						DelSiteFileAlias(UINT32 nID);
	PDB_SITE_FILE_ALIAS			FindSiteFileAliasKey(DB_SITE_FILE_ALIAS& data);
	INT32						ApplySiteFileAlias(DB_SITE_FILE_ALIAS& data);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_SITE_FILE_ALIAS& data);
	INT32			GetPktSync(MemToken& RecvToken, DB_SITE_FILE_ALIAS& data);

	INT32			SetPkt(MemToken& SendToken);
	INT32			SetPkt(TListID& tIDList, MemToken& SendToken);
	INT32			SetPkt(UINT32 nID, MemToken& SendToken);
    INT32			SetPkt(PDB_SITE_FILE_ALIAS pdata, MemToken& SendToken);

	INT32			SetPktSync(MemToken& SendToken);
	INT32			SetPktSync(TListPVOID& tIDList);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageSiteFileAlias();
    ~CManageSiteFileAlias();

};

extern CManageSiteFileAlias*	t_ManageSiteFileAlias;

#endif
