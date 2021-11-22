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

#ifndef DB_ENV_LOG_UNIT_H_348683B5_F316_4a4d_842D_63E68B189A5D
#define DB_ENV_LOG_UNIT_H_348683B5_F316_4a4d_842D_63E68B189A5D

typedef struct _db_env_log_event_unit
{
	_db_env_log_event_unit()
	{
		nEvtType				= 0;
		nEvtCode				= 0;
		nEvtErrCode				= 0;
		nSubjectType 			= 0;
		nTargetType 			= 0;	
		nObjectType				= 0;
		nObjectCode				= 0;
		nOperationType			= 0;

		nRecordMode				= 0;

		tIDMapStrExtInfo.clear();
	}

	DB_PO_HEADER		tDPH;

	UINT32				nEvtType;
	UINT32				nEvtCode;
	UINT32				nEvtErrCode;

	UINT32				nSubjectType;
	String				strSubjectInfo;

	UINT32				nTargetType;
	String				strTargetInfo;

	UINT32				nObjectType;
	UINT32				nObjectCode;
	String				strObjectInfo;

	UINT32				nOperationType;
	String				strEvtDescr;

	UINT32				nRecordMode;
	String				strSMSValue;

	TMapIDStr			tIDMapStrExtInfo;

}DB_ENV_LOG_UNIT, *PDB_ENV_LOG_UNIT;

typedef list<DB_ENV_LOG_UNIT>				TListDBEnvLogUnit;
typedef TListDBEnvLogUnit::iterator		TListDBEnvLogUnitItor;

typedef map<UINT32, DB_ENV_LOG_UNIT>		TMapDBEnvLogUnit;
typedef TMapDBEnvLogUnit::iterator			TMapDBEnvLogUnitItor;

#endif //DB_ENV_LOG_UNIT_H_348683B5_F316_4a4d_842D_63E68B189A5D



