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

#ifndef MEM_FIND_ORDER_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_FIND_ORDER_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_find_order_info
{
	_mem_find_order_info()
	{
		nID					= 0;
		nEvtTime			= 0;
		nPoType				= 0;
		nPoID				= 0;
		nOpType				= 0;
		nNextOp				= 0;
		nNetDrv				= 0;

		nNotiPg				= 0;
		nNotiPgSend			= 0;
		nNotiPgTotal		= 0;
		nNotiPgEnd			= 0;

		nNotiTotalFind		= 0;
	}
	_mem_find_order_info(UINT32 _nID, UINT32 _nPoID, UINT32 _nOpType, UINT32 _nNextOp)
	{
		nID			= _nID;
		nPoID		= _nPoID;
		nOpType		= _nOpType;
		nNextOp		= _nNextOp;
	}

	void _set_opt(UINT32 nKey, UINT32 nValue)
	{
		tOptMap[nKey] = nValue;
		return;
	}

	UINT32 _get_opt(UINT32 nKey)
	{
		TMapIDItor find = tOptMap.find(nKey);
		if(find == tOptMap.end())	return 0;

		return find->second;
	}

	void _set_str_opt(UINT32 nKey, String strValue)
	{
		tOptStrMap[nKey] = strValue;
		return;
	}

	String _get_str_opt(UINT32 nKey)
	{
		TMapIDStrItor find = tOptStrMap.find(nKey);
		if(find == tOptStrMap.end())	return "";

		return find->second;
	}

	UINT32			nID;	
	UINT32			nEvtTime;
	UINT64			nPoType;
	UINT32			nPoID;
	UINT32			nOpType;
	UINT32			nNextOp;
	UINT32			nNetDrv;

	TMapID			tOptMap;
	TMapIDStr		tOptStrMap;

	UINT32			nNotiPg;
	UINT32			nNotiPgSend;
	UINT32			nNotiPgTotal;
	UINT32			nNotiPgEnd;

	UINT32			nNotiTotalFind;

}MEM_FIND_ORDER_INFO, *PMEM_FIND_ORDER_INFO;

typedef list<MEM_FIND_ORDER_INFO>				TListMemFindOrderInfo;
typedef TListMemFindOrderInfo::iterator			TListMemFindOrderInfoItor;

typedef map<UINT32, MEM_FIND_ORDER_INFO>		TMapMemFindOrderInfo;
typedef TMapMemFindOrderInfo::iterator			TMapMemFindOrderInfoItor;

#endif //MEM_FIND_ORDER_INFO_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



