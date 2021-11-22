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
 */


#ifndef _AS_STL_H__
#define	_AS_STL_H__

extern INT32	IsExistsIDFromMap(TMapID& tIDMap, UINT32 nID);
extern INT32	IsExistsIDFromList(TListID& tIDList, UINT32 nID);

extern VOID	ConvertListToMap(TListID& tIDList, TMapID& tIDMap);
extern VOID	ConvertMapToList(TMapID& tIDMap, TListID& tIDList);
extern VOID	ChangeMapKeyValue(TMapID& tOrIDMap, TMapID& tChgIDMap);
extern VOID	AppandMapToMap(TMapID& tFromIDMap, TMapID& tToIDMap);
extern VOID	AppandListToList(TListID& tFromIDList, TListID& tToIDList);
extern VOID	AppandListToMap(TListID& tFromIDList, TMapID& tToIDMap);
extern VOID	AppandMapToList(TMapID& tFromIDMap, TListID& tToIDList);
extern INT32	RemoveListID(TListID& tIDList, UINT32 nID);
extern INT32	CompareMapID(TMapID& tAMap, TMapID& tBMap);
extern INT32	CompareMapIDStr(TMapIDStr& tAMap, TMapIDStr& tBMap);
extern INT32	CompareMapID64(TMapID64& tAMap, TMapID64& tBMap);
extern INT32	CompareList(TListID& tAList, TListID& tBList);
extern INT32	CompareListChgMap(TListID& tAList, TListID& tBList);



#endif //_AS_STL_H__