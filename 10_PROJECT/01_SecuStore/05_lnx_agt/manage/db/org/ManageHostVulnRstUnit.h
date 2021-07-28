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

#ifndef ManageHostVulnRstUnitH
#define ManageHostVulnRstUnitH
//---------------------------------------------------------------------------

class CManageHostVulnRstUnit : public CManageBase<DB_HOST_VULN_RST_UNIT>
{

public:
	INT32				LoadDBMS();

public:
	INT32				AddHostVulnRstUnit(DB_HOST_VULN_RST_UNIT& data);
    INT32				EditHostVulnRstUnit(DB_HOST_VULN_RST_UNIT& data);
	INT32				DelHostVulnRstUnit(UINT32 nID);
	INT32				ClearHostVulnRstUnit(UINT32 nVulnRstID);

public:
	INT32				GetPkt_Link(MemToken& RecvToken, DB_HOST_VULN_RST_UNIT& data);
	INT32				SetPkt_Link(MemToken& SendToken);
	INT32				SetPkt_Link(TListID& tIDList, MemToken& SendToken);
	INT32				SetPkt_Link(PDB_HOST_VULN_RST_UNIT pdata, MemToken& SendToken);	

public:
	void 				RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageHostVulnRstUnit();
    ~CManageHostVulnRstUnit();

};

extern CManageHostVulnRstUnit*	t_ManageHostVulnRstUnit;

#endif
