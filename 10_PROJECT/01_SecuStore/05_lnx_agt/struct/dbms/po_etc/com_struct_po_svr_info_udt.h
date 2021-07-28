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

#ifndef DB_PO_SVR_INFO_UDT_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_PO_SVR_INFO_UDT_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_svr_info_udt
{
	_db_po_svr_info_udt()
	{
	}

	DB_PO_HEADER		tDPH;

	String				strChkSvrInfoList;
	String				strDataSvrInfoList;

}DB_PO_SVR_INFO_UDT, *PDB_PO_SVR_INFO_UDT;

typedef list<DB_PO_SVR_INFO_UDT>				TListDBPoSvrInfoUdt;
typedef TListDBPoSvrInfoUdt::iterator			TListDBPoSvrInfoUdtItor;

typedef map<UINT32, DB_PO_SVR_INFO_UDT>			TMapDBPoSvrInfoUdt;
typedef TMapDBPoSvrInfoUdt::iterator			TMapDBPoSvrInfoUdtItor;

#endif //DB_PO_SVR_INFO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



