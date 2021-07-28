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

#ifndef DB_ENV_NOTIFY_INFO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define DB_ENV_NOTIFY_INFO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _db_env_notify_info
{
	_db_env_notify_info()
	{
		nPolicyType		= 0;
		nPolicyCode		= 0;
		nPosType		= 0;
		nShowTime		= 0;
		nShowSize		= 0;
		nShowPos		= 0;
		nMsgFmtType		= 0;
		nNotifyBeforeDay	= 0;
		nNotifySkipDay		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT64				nPolicyType;
	UINT64				nPolicyCode;
	UINT32				nPosType;
	UINT32				nShowSize;
	UINT32				nShowPos;
	UINT32				nShowTime;
	UINT32				nMsgFmtType;
	String				strMsgInfo;

	UINT32				nNotifyBeforeDay;

	String				strMatchInfo;
	UINT32				nNotifySkipDay;

}DB_ENV_NOTIFY_INFO, *PDB_ENV_NOTIFY_INFO;

typedef list<DB_ENV_NOTIFY_INFO>				TListDBEnvNotifyInfo;
typedef TListDBEnvNotifyInfo::iterator			TListDBEnvNotifyInfoItor;

typedef map<UINT32, DB_ENV_NOTIFY_INFO>			TMapDBEnvNotifyInfo;
typedef TMapDBEnvNotifyInfo::iterator			TMapDBEnvNotifyInfoItor;

typedef struct mem_env_notify_info_sch
{
	mem_env_notify_info_sch()
	{
		nNotifyType		= 0;
		nPolType		= 0;
		nOpType			= 0;
		nSchType		= 0;
		nNotifyInfoID	= 0;	
		
		nSchTime		= 0;
	}

	DB_PO_HEADER		tDPH;

	UINT32				nNotifyType;
	UINT32				nPolType;
	UINT32				nOpType;
	UINT32				nSchType;
	UINT32				nNotifyInfoID;

	UINT32				nSchTime;

}MEM_ENV_NOTIFY_INFO_SCH, *PMEM_ENV_NOTIFY_INFO_SCH;

typedef list<MEM_ENV_NOTIFY_INFO_SCH>			TListMEMEnvNotifyInfoSch;
typedef TListMEMEnvNotifyInfoSch::iterator		TListMEMEnvNotifyInfoSchItor;

typedef map<UINT32, MEM_ENV_NOTIFY_INFO_SCH>	TMapMEMEnvNotifyInfoSch;
typedef TMapMEMEnvNotifyInfoSch::iterator		TMapMEMEnvNotifyInfoSchItor;

#endif //DB_ENV_NOTIFY_INFO_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



