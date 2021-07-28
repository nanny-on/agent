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


#ifndef DB_ENV_INFO_PROC_REPUTATION_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_ENV_INFO_PROC_REPUTATION_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_info_proc_reputation
{
	_db_info_proc_reputation()
	{	
		nID				= 0;
		nEditDate		= 0;

		nSiteFileID		= 0;
		nReputation		= 0;
		nNumOfUser		= 0;

		nSendPktRef		= 0;
	}

	UINT32				nID;
	UINT32				nEditDate;

	UINT32				nSiteFileID;
	UINT32				nReputation;
	UINT32				nNumOfUser;

	String				strFileKey;
	UINT32				nSendPktRef;

}DB_INFO_PROC_REPUTATION, *PDB_INFO_PROC_REPUTATION;

typedef list<DB_INFO_PROC_REPUTATION>			TListDBInfoProcReputation;
typedef TListDBInfoProcReputation::iterator		TListDBInfoProcReputationItor;

typedef map<UINT32, DB_INFO_PROC_REPUTATION>	TMapDBInfoProcReputation;
typedef TMapDBInfoProcReputation::iterator		TMapDBInfoProcReputationItor;

#endif //DB_ENV_INFO_PROC_REPUTATION_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



