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

#ifndef ManagePtnVulnMPH
#define ManagePtnVulnMPH
//---------------------------------------------------------------------------

class CManagePtnVulnMP : public CManageBase<DB_PTN_VULN_MP>
{
private:
	CDBMgrPtnVulnMP*	m_tDBMgrPtnVulnMP;

public:
	INT32			LoadDBMS();

public:
	INT32			AddPtnVulnMP(DB_PTN_VULN_MP& data);
    INT32			EditPtnVulnMP(DB_PTN_VULN_MP& data);
    INT32			DelPtnVulnMP(UINT32 nID);

public:
	INT32			GetPkt(MemToken& RecvToken, DB_PTN_VULN_MP& data);
	INT32			SetPkt(MemToken& SendToken);
    INT32			SetPkt(PDB_PTN_VULN_MP pdata, MemToken& SendToken);

public:
	void 			RemainDBG_ItemCnt(UINT32 nIdx);

public:
	CManagePtnVulnMP();
    ~CManagePtnVulnMP();

};

#endif
