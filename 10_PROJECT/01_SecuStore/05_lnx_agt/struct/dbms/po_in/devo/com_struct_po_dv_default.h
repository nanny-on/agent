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

#ifndef DB_PO_DV_DEFAULT_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_DV_DEFAULT_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_dv_default
{
	_db_po_dv_default()
	{
		nPoPerm				= 0;
		strDvPaName			= "";
		strDvName			= "";
		strDvInstanceID		= "";
		strDvPaInstanceID	= "";
		strClassName		= "";		
	}
	DB_PO_HEADER		tDPH;

	UINT32				nPoPerm;
	String				strDvPaName;
	String				strDvName;
	String				strDvInstanceID;
	String				strDvPaInstanceID;
	String				strClassName;

}DB_PO_DV_DEFAULT, *PDB_PO_DV_DEFAULT;

typedef list<DB_PO_DV_DEFAULT>				TListDBPoDvDefault;
typedef TListDBPoDvDefault::iterator		TListDBPoDvDefaultItor;

typedef map<UINT32, DB_PO_DV_DEFAULT>		TMapDBPoDvDefault;
typedef TMapDBPoDvDefault::iterator			TMapDBPoDvDefaultItor;

#endif //DB_PO_DV_DEFAULT_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



