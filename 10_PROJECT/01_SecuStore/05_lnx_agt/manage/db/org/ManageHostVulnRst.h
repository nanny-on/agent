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

#ifndef ManageHostVulnRstH
#define ManageHostVulnRstH
//---------------------------------------------------------------------------

class CManageHostVulnRst : public CManageBase<DB_HOST_VULN_RST>
{

public:
	INT32				LoadDBMS();

public:
	INT32				AddHostVulnRst(DB_HOST_VULN_RST& data);
    INT32				EditHostVulnRst(DB_HOST_VULN_RST& data);
    INT32				DelHostVulnRst(UINT32 nID);

	void				SetRstMapToStr(PDB_HOST_VULN_RST pdata);

public:
	INT32				SetPktSync(MemToken& SendToken);

	INT32				GetPkt(MemToken& RecvToken, DB_HOST_VULN_RST& data);
	INT32				SetPkt(MemToken& SendToken);
	INT32				SetPkt(TListDBHostVulnRst& tDBHostVulnRstList, MemToken& SendToken);
	INT32				SetPkt(PDB_HOST_VULN_RST pdata, MemToken& SendToken);	

public:
	INT32				GetPkt_Link(MemToken& RecvToken, DB_HOST_VULN_RST& data);
	INT32				SetPkt_Link(MemToken& SendToken);
	INT32				SetPkt_Link(PDB_HOST_VULN_RST pdata, MemToken& SendToken);	

	INT32				SetPkt_Link(UINT32 nID, MemToken& SendToken);	

public:
	void 				RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageHostVulnRst();
    ~CManageHostVulnRst();

};

extern CManageHostVulnRst*	t_ManageHostVulnRst;

#endif
