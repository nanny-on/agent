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

#ifndef MEM_DV_DISABLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467
#define MEM_DV_DISABLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467

typedef struct _mem_dv_disable
{
	_mem_dv_disable()
	{
		nID					= 0;
		strDvPaName			= "";
		strDvName			= "";
		strDvInstanceID		= "";
		strDvPaInstanceID	= "";
	}

	UINT32				nID;	
	String				strDvPaName;
	String				strDvName;
	String				strDvInstanceID;
	String				strDvPaInstanceID;

}MEM_DV_DISABLE, *PMEM_DV_DISABLE;

typedef list<MEM_DV_DISABLE>				TListMemDvDisable;
typedef TListMemDvDisable::iterator		TListMemDvDisableItor;

typedef map<UINT32, MEM_DV_DISABLE>		TMapMemDvDisable;
typedef TMapMemDvDisable::iterator			TMapMemDvDisableItor;
typedef TMapMemDvDisable::reverse_iterator	TMapMemDvDisableRItor;

#endif //MEM_DV_DISABLE_H_4D39CDDB_PO_E289_4d56_9F98_FE42776F4467



