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

#ifndef DB_PO_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define DB_PO_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _db_po_header
{
	_db_po_header()
    {
		nID					= 0;
		nUsedFlag			= USED_FLAG_TYPE_USED;
		nRegDate			= 0;
		nSeqNo				= 0;
		nAdminID			= 0;
		nSubAdminLock		= 0;
		nTargetLock			= 0;
		nPolicyID			= 0;
		nUnitID				= 0;
		nTargetOS			= 0;
		nExtOption			= 0;
		nUsedMode			= 0;

		nEditID				= 0;
    }

	INT32	_exists_item(TMapIDItor _find)
	{
		TMapIDItor find = tSubIDMap.find(_find->first);
		if(find == tSubIDMap.end())		return 0;

		return 1;
	}

	INT32	_exists_id(UINT32 nID)
	{
		TMapIDItor find = tSubIDMap.find(nID);
		if(find == tSubIDMap.end())		return 0;

		return 1;
	}

	INT32	_exists_value(UINT32 nValue)
	{
		TMapIDItor begin, end;
		begin = tSubIDMap.begin();	end = tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			if(begin->second == nValue)		return 1;
		}
		return 0;
	}

	INT32	_add_id(UINT32 nID)
	{
		tSubIDMap[nID] = 0;
		return 0;
	}

	INT32	_add_id_value(UINT32 nID, UINT32 nValue)
	{
		tSubIDMap[nID] = nValue;
		return 0;
	}

	INT32	_exists_id(UINT8 nKey, UINT32 nID)
	{
		UINT32 nEditID = ((UINT32)nKey) << 24;
		nEditID += nID;

		return _exists_id(nEditID);
	}

	INT32	_add_key_id_value(UINT8 nKey, UINT32 nID, UINT32 nValue)
	{
		UINT32 nEditID = ((UINT32)nKey) << 24;
		nEditID += nID;

		return _add_id_value(nEditID, nValue);
	}

	void	_get_id_to_keysubid(UINT32 nID, UINT8& nKey, UINT32& nSubID)
	{
		nKey = (nID >> 24);
		nSubID = ((nID) << 8) >> 8;
		return;
	}

	UINT8	_get_id_to_key(UINT32 nID)
	{
		return UINT8(nID >> 24);
	}

	UINT32	_get_id_to_subid(UINT32 nID)
	{
		return UINT32(((nID) << 8) >> 8);
	}

	INT32	_is_equal_key(UINT32 nID, UINT8 nKey)
	{
		if(nKey == (nID >> 24))	return 1;
		return 0;
	}

	INT32	_get_id_list_by_key(UINT8 nKey, TListID& tIDList)
	{
		TMapIDItor begin, end;
		begin = tSubIDMap.begin();	end = tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			if(_get_id_to_key(begin->first) != nKey)	continue;

			tIDList.push_back(begin->first);
		}
		return 0;
	}

	INT32	_get_id_valid_map_by_key(UINT8 nKey, TMapID& tIDMap)
	{
		TMapIDItor begin, end;
		begin = tSubIDMap.begin();	end = tSubIDMap.end();
		for(begin; begin != end; begin++)
		{
			if(_get_id_to_key(begin->first) != nKey)	continue;

			tIDMap[begin->first] = begin->second;
		}
		return 0;
	}

	INT32	_clear()
	{
		tSubIDMap.clear();
		return 0;
	}

	UINT32				nID ;
	UINT32				nUsedFlag;
	UINT32				nRegDate;
	UINT32				nSeqNo;

    String				strName;
    String				strDescr;
	UINT32				nAdminID;
	UINT32				nSubAdminLock;
	UINT32				nTargetLock;
	UINT32				nPolicyID;
	UINT32				nUnitID;
	UINT32				nTargetOS;
	UINT32				nExtOption;
	UINT32				nUsedMode;
	
	UINT32				nEditID;
	String				strHash;
	TMapID				tSubIDMap;
    
}DB_PO_HEADER, *PDB_PO_HEADER;

typedef list<DB_PO_HEADER>				TListDBPoHeader;
typedef TListDBPoHeader::iterator		TListDBPoHeaderItor;

typedef map<UINT32, DB_PO_HEADER>		TMapDBPoHeader;
typedef TMapDBPoHeader::iterator		TMapDBPoHeaderItor;
//------------------------------------------------------------------------------

#endif //DB_PO_HEADER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



