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

#ifndef ManageLinkLicenseH
#define ManageLinkLicenseH
//---------------------------------------------------------------------------

class CManageLinkLicense : public CManagePoBase<DB_LINK_LICENSE>
{
public:
	UINT32					m_nSID;
	TMapID					m_tSIDMap;

public:
	INT32					LoadDBMS();

public:
	INT32					InitHash();
	String					GetHash();

public:
	INT32					AddLinkLicense(DB_LINK_LICENSE&	dl);
    INT32					EditLinkLicense(DB_LINK_LICENSE&	dl);
    INT32					DelLinkLicense(UINT32 nID);
	
public:
	String					GetName(UINT32 nID);    
	PDB_LINK_LICENSE		FindLicense(String strLicense);
	UINT32					GetNextSID();
	UINT32					DelSID(UINT32 nSID);
	UINT32					GetSIDList(TListID& tIDList);
	UINT32					GetSIDCount();
	
public:
	INT32					SetPkt(MemToken& SendToken);
	INT32					SetPkt(UINT32 nID, MemToken& SendToken);
	INT32					SetPkt(PDB_LINK_LICENSE pdl, MemToken& SendToken);
	INT32					GetPkt(MemToken& RecvToken, DB_LINK_LICENSE& dl);

public:
	CManageLinkLicense();
    ~CManageLinkLicense();

};

extern CManageLinkLicense*	t_ManageLinkLicense;

#endif
