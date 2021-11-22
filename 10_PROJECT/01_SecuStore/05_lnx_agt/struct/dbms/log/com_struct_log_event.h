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

#ifndef DB_LOG_EVENT_H_348683B5_F316_4a4d_842D_63E68B189A5D
#define DB_LOG_EVENT_H_348683B5_F316_4a4d_842D_63E68B189A5D

typedef struct _db_log_event
{
    _db_log_event()
    {
        _init();        
    }

	void _init()
	{
		nID 				= 0;
		nUsedFlag			= 1;
		nRegDate			= 0;
		nEvtTime 			= 0;
		nEvtErrCode			= ERR_SUCCESS;
		nSkipTarget			= 0;

		nNotifyType 		= 0;
		nNotifyID			= 0;

		nHostID				= 0;
		nUserID				= 0;
		nRegSvrID			= 0;
		nSyncSvrMode		= 0;
		nSyncSvrStep		= 0;

		nSubjectType 		= 0;
		nSubjectID			= 0;
		strSubjectInfo		= "";

		nTargetType 		= 0;
		nTargetID 			= 0;
		strTargetInfo 		= "";
	
		nObjectType			= 0;
		nObjectCode			= 0;
		nObjectID			= 0;
		strObjectInfo		= "";

		nOperationType		= 0;
		strEventDescr		= "";
		tDescIDStrMap.clear();
	}

	void	_set_desc_info(UINT32 nKey, String strDescValue)
	{
		tDescIDStrMap[nKey] = strDescValue;
	}

	void	_set_desc_info(UINT32 nKey, UINT32 nValue)
	{
		String strValue;
		strValue = SPrintf("%u", nValue);

		tDescIDStrMap[nKey] = strValue;
	}

	String	_get_desc_value(UINT32 nKey)
	{
		return tDescIDStrMap[nKey];
	}
  
	UINT32		nID;
	UINT32		nUsedFlag;
	UINT32		nRegDate;
	UINT32		nEvtTime;
	UINT32		nEvtErrCode;
	UINT32		nSkipTarget;

	UINT32		nNotifyType;
	UINT32		nNotifyID;

	UINT32		nHostID;
	UINT32		nUserID;
	UINT32		nRegSvrID;
	UINT32		nSyncSvrMode;
	UINT32		nSyncSvrStep;

	UINT32		nSubjectType;
	UINT32		nSubjectID;
	String		strSubjectInfo;

	UINT32		nTargetType;
	UINT32		nTargetID;
	String		strTargetInfo;

    UINT32		nObjectType;
    UINT32		nObjectCode;
	UINT32		nObjectID;
	String		strObjectInfo;
    
	UINT32		nOperationType;
    String		strEventDescr;    
	TMapIDStr	tDescIDStrMap; 
}DB_LOG_EVENT, *PDB_LOG_EVENT;

typedef list<DB_LOG_EVENT>				TListDBLogEvent;
typedef TListDBLogEvent::iterator		TListDBLogEventItor;

typedef map<UINT32, DB_LOG_EVENT>		TMapDBLogEvent;
typedef TMapDBLogEvent::iterator		TMapDBLogEventItor;

#endif //DB_LOG_EVENT_H_348683B5_F316_4a4d_842D_63E68B189A5D



