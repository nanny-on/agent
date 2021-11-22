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


#ifndef ManageVulnRegProtectH
#define ManageVulnRegProtectH
//---------------------------------------------------------------------------

class CManageVulnRegProtect : public CManageBase<DB_VULN_REG_PROTECT>
{

public:
	INT32			LoadDBMS();

public:
	INT32			AddVulnRegProtect(DB_VULN_REG_PROTECT& data);
    INT32			EditVulnRegProtect(DB_VULN_REG_PROTECT& data);
    INT32			DelVulnRegProtect(UINT32 nID);
	INT32			ApplyVulnRegProtect(DB_VULN_REG_PROTECT&	data);

public:
	String			GetName(UINT32 nID);
	INT32			MakeObject(POBJECT_ENTRY* pObjList, UINT32& nObjRulNum, UINT32 nType);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_VULN_REG_PROTECT& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_VULN_REG_PROTECT pdata, MemToken& SendToken);
	INT32			SetPktHost(UINT32 nID, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageVulnRegProtect();
    ~CManageVulnRegProtect();

};

extern CManageVulnRegProtect*	t_ManageVulnRegProtect;

#endif
