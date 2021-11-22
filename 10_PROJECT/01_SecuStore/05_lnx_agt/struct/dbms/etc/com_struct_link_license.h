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

#ifndef DB_LINK_LICENSE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_LINK_LICENSE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_link_license
{
	_db_link_license()
	{		
	}

	DB_PO_HEADER		tDPH;

	String				strLicenseKey;

}DB_LINK_LICENSE, *PDB_LINK_LICENSE;

typedef list<DB_LINK_LICENSE>				TListDBLinkLicense;
typedef TListDBLinkLicense::iterator		TListDBLinkLicenseItor;

typedef map<UINT32, DB_LINK_LICENSE>		TMapDBLinkLicense;
typedef TMapDBLinkLicense::iterator			TMapDBLinkLicenseItor;

#endif //DB_LINK_LICENSE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



