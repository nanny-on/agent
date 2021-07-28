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


#ifndef ManageVulnFileProtectH
#define ManageVulnFileProtectH
//---------------------------------------------------------------------------

class CManageVulnFileProtect : public CManageBase<DB_VULN_FILE_PROTECT>
{

public:
	INT32			LoadDBMS();

public:
	INT32			AddVulnFileProtect(DB_VULN_FILE_PROTECT& data);
    INT32			EditVulnFileProtect(DB_VULN_FILE_PROTECT& data);
    INT32			DelVulnFileProtect(UINT32 nID);
	INT32			ApplyVulnFileProtect(DB_VULN_FILE_PROTECT&	data);

public:
	String			GetName(UINT32 nID);
	INT32			MakeObject(UINT32 nID, POBJECT_ENTRY* pObjList, UINT32& nObjRulNum, UINT32 nType);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_VULN_FILE_PROTECT& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_VULN_FILE_PROTECT pdata, MemToken& SendToken);
	INT32			SetPktHost(UINT32 nID, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManageVulnFileProtect();
    ~CManageVulnFileProtect();

};

extern CManageVulnFileProtect*	t_ManageVulnFileProtect;

#endif
