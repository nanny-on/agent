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

#ifndef ManagePoInVulnScanH
#define ManagePoInVulnScanH
//---------------------------------------------------------------------------

class CManagePoInVulnScan : public CManagePoBase<DB_PO_IN_VULN_SCAN>
{
public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	INT32					InitHash(UINT32 nID);

public:
	INT32					AddPoInVulnScan(DB_PO_IN_VULN_SCAN&	data);
    INT32					EditPoInVulnScan(DB_PO_IN_VULN_SCAN&	data);
	INT32					DelPoInVulnScan(UINT32 nID);
	INT32					ApplyPoInVulnScan(DB_PO_IN_VULN_SCAN&	data);

public:
	String					GetName(UINT32 nID);    

public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPktHost(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_PO_IN_VULN_SCAN pdata, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_PO_IN_VULN_SCAN& data);

public:
	CManagePoInVulnScan();
    ~CManagePoInVulnScan();

};

extern CManagePoInVulnScan*	t_ManagePoInVulnScan;

#endif
