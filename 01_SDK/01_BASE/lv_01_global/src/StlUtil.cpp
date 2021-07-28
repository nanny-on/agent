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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "StlUtil.h"

//------------------------------------------------------------------------------

INT32	IsExistsIDFromMap(TMapID& tIDMap, UINT32 nID)
{
	TMapIDItor find = tIDMap.find(nID);
	if(find == tIDMap.end())	return 0;
	return 1;
}
//---------------------------------------------------------------------------

INT32	IsExistsIDFromList(TListID& tIDList, UINT32 nID)
{
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		if(*begin != nID)	continue;

		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------

VOID	ConvertListToMap(TListID& tIDList, TMapID& tIDMap)
{
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		tIDMap[*begin] = 0;
	}
	return;
}
//---------------------------------------------------------------------------

VOID	ConvertMapToList(TMapID& tIDMap, TListID& tIDList)
{
	TMapIDItor begin, end;
	begin = tIDMap.begin();	end = tIDMap.end();
	for(begin; begin != end; begin++)
	{
		tIDList.push_back(begin->first);
	}
	return;
}
//---------------------------------------------------------------------------

VOID	ChangeMapKeyValue(TMapID& tOrIDMap, TMapID& tChgIDMap)
{
	TMapIDItor begin, end;
	begin = tOrIDMap.begin();	end = tOrIDMap.end();
	for(begin; begin != end; begin++)
	{
		tChgIDMap[begin->second] = begin->first;
	}
	return;
}
//---------------------------------------------------------------------------

VOID	AppandMapToMap(TMapID& tFromIDMap, TMapID& tToIDMap)
{
	TMapIDItor begin, end;
	begin = tFromIDMap.begin();	end = tFromIDMap.end();
	for(begin; begin != end; begin++)
	{
		tToIDMap[begin->first] = begin->second;
	}
	return;
}
//---------------------------------------------------------------------------

VOID	AppandListToList(TListID& tFromIDList, TListID& tToIDList)
{
	TListIDItor begin, end;
	begin = tFromIDList.begin();	end = tFromIDList.end();
	for(begin; begin != end; begin++)
	{
		tToIDList.push_back(*begin);
	}
	return;
}
//---------------------------------------------------------------------------

VOID	AppandListToMap(TListID& tFromIDList, TMapID& tToIDMap)
{
	TListIDItor begin, end;
	begin = tFromIDList.begin();	end = tFromIDList.end();
	for(begin; begin != end; begin++)
	{
		tToIDMap[*begin] = 0;
	}
	return;
}
//---------------------------------------------------------------------------

VOID	AppandMapToList(TMapID& tFromIDMap, TListID& tToIDList)
{
	TMapIDItor begin, end;
	begin = tFromIDMap.begin();	end = tFromIDMap.end();
	for(begin; begin != end; begin++)
	{
		tToIDList.push_back(begin->first);
	}
	return;
}
//---------------------------------------------------------------------------

INT32	RemoveListID(TListID& tIDList, UINT32 nID)
{
	TListIDItor begin, end;
	begin = tIDList.begin();	end = tIDList.end();
	for(begin; begin != end; begin++)
	{
		if(*begin != nID)	continue;

		tIDList.erase(begin);
		return tIDList.size();
	}
	return 0;
}
//------------------------------------------------------------------------------

INT32	CompareMapID(TMapID& tAMap, TMapID& tBMap)
{
	TMapIDItor begin_a, end_a, begin_b, end_b;

	begin_a = tAMap.begin();	end_a = tAMap.end();
	begin_b = tBMap.begin();	end_b = tBMap.end();

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;

	for(; begin_a != end_a && begin_b != end_b; begin_a++, begin_b++)
	{
		if(begin_a->first > begin_b->first)		return -1;
		if(begin_a->first < begin_b->first)		return 1;

		if(begin_a->second > begin_b->second)	return -1;
		if(begin_a->second < begin_b->second)	return 1;		
	}

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;
	return 0;
}

INT32	CompareMapIDStr(TMapIDStr& tAMap, TMapIDStr& tBMap)
{
	TMapIDStrItor begin_a, end_a, begin_b, end_b;

	begin_a = tAMap.begin();	end_a = tAMap.end();
	begin_b = tBMap.begin();	end_b = tBMap.end();

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;

	for(; begin_a != end_a && begin_b != end_b; begin_a++, begin_b++)
	{
		if(begin_a->first > begin_b->first)		return -1;
		if(begin_a->first < begin_b->first)		return 1;

		if(begin_a->second > begin_b->second)	return -1;
		if(begin_a->second < begin_b->second)	return 1;		
	}

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;
	return 0;
}
//------------------------------------------------------------------------------

INT32	CompareMapID64(TMapID64& tAMap, TMapID64& tBMap)
{
	TMapID64Itor begin_a, end_a, begin_b, end_b;

	begin_a = tAMap.begin();	end_a = tAMap.end();
	begin_b = tBMap.begin();	end_b = tBMap.end();

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;

	for(; begin_a != end_a && begin_b != end_b; begin_a++, begin_b++)
	{
		if(begin_a->first > begin_b->first)		return -1;
		if(begin_a->first < begin_b->first)		return 1;

		if(begin_a->second > begin_b->second)	return -1;
		if(begin_a->second < begin_b->second)	return 1;		
	}

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;
	return 0;
}
//------------------------------------------------------------------------------

INT32	CompareList(TListID& tAList, TListID& tBList)
{
	TListIDItor begin_a, end_a, begin_b, end_b;
	begin_a = tAList.begin();	end_a = tAList.end();
	begin_b = tBList.begin();	end_b = tBList.end();

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;

	for(; begin_a != end_a && begin_b != end_b; begin_a++, begin_b++)
	{
		if(*begin_a > *begin_b)		return -1;
		if(*begin_a < *begin_b)		return 1;		
	}

	if(begin_a != end_a && begin_b == end_b)	return -1;
	if(begin_a == end_a && begin_b != end_b)	return 1;

	return 0;
}
//------------------------------------------------------------------------------

INT32	CompareListChgMap(TListID& tAList, TListID& tBList)
{
	TMapID tAMap, tBMap;
	ConvertListToMap(tAList, tAMap);
	ConvertListToMap(tBList, tBMap);

	return CompareMapID(tAMap, tBMap);
}
//------------------------------------------------------------------------------

