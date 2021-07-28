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

#ifndef DB_HOST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_HOST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_host
{
	_db_host()
    {
		nID						= 0;
		nUsedFlag				= USED_FLAG_TYPE_USED;

		nConDate				= 0;
		nDConDate				= 0;

		nUserID					= 0;
        nConnected				= CLIENT_CON_STATUS_DISCONNECTED;

		for(INT32 nIdx = 0; nIdx < SS_POLICY_INDEX_TOTAL_NUMBER; nIdx++)
		{
			tPoSeqNoMap[nIdx] = 0;
			tPoTimeMap[nIdx] = 0;
		}		
    }  

	String _get_name()
	{
		if(strNameAlias.empty())	return strName;
		return strNameAlias;
	}

	void _set_policy_info(UINT32 nIdx, UINT32 nID, UINT32 nSeqNo, UINT32 nTime)
	{
		TMapIDItor find;
		{
			find = tPoIDMap.find(nIdx);
			if(find != tPoIDMap.end())
			{
				find->second = nID;
			}
		}
		{
			find = tPoSeqNoMap.find(nIdx);
			if(find != tPoSeqNoMap.end())
			{
				find->second = nSeqNo;
			}
		}
		{
			find = tPoTimeMap.find(nIdx);
			if(find != tPoTimeMap.end())
			{
				find->second = nTime;
			}
		}
		return;
	}

	UINT32	_get_policy_id(UINT32 nIdx)
	{
		UINT32 nID = 0;
		{
			TMapIDItor find = tPoIDMap.find(nIdx);
			if(find != tPoIDMap.end())
			{
				nID = find->second;
			}
		}
		return nID;
	}

	UINT32	_get_policy_seqno(UINT32 nIdx)
	{
		UINT32 nSeqNo = 0;
		{
			TMapIDItor find = tPoSeqNoMap.find(nIdx);
			if(find != tPoSeqNoMap.end())
			{
				nSeqNo = find->second;
			}
		}
		return nSeqNo;
	}

	UINT32	_get_policy_time(UINT32 nIdx)
	{
		UINT32 nTime = 0;
		{
			TMapIDItor find = tPoTimeMap.find(nIdx);
			if(find != tPoTimeMap.end())
			{
				nTime = find->second;
			}
		}
		return nTime;
	}

    UINT32				nID ;
	UINT32				nUsedFlag;
	String				strName ;
	String				strNameAlias ;
	String				strWorkGroup;
    UINT32				nConDate;
	UINT32				nDConDate;
	String				strPriIP;
	String				strNatIP;
	String				strMac;
	String				strGUID;
	TMapID				tPoIDMap;
	TMapID				tPoSeqNoMap;
	TMapID				tPoTimeMap;
	UINT32				nUserID;
	String				strUserUnicode;    
	String				strWorkGroupAlias;
    UINT32				nConnected;
	TMapID64			tProtectLogIDMap;
	TMapID64			tDocDelLogIDMap;
	TMapID64			tPolApplyLogIDMap;	
}DB_HOST, *PDB_HOST;

typedef list<DB_HOST>				TListDBHost;
typedef TListDBHost::iterator		TListDBHostItor;

typedef map<UINT32, DB_HOST>		TMapDBHost;
typedef TMapDBHost::iterator		TMapDBHostItor;

#endif //DB_HOST_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



