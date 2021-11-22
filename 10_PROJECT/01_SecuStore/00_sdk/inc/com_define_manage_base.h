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

#ifndef _COM_DEFINE_MANAGE_BASE_H__
#define _COM_DEFINE_MANAGE_BASE_H__

typedef struct _mgr_base_gkey
{
	_mgr_base_gkey()
	{
		nKey		= 0;
		nInitMode	= 0;
	}

	UINT64 nKey;
	UINT32 nInitMode;

	TListIDItor	tItorBegin;
	TListIDItor	tItorEnd;	
}MGR_BASE_GKEY, *PMGR_BASE_GKEY;

typedef map<UINT64, MGR_BASE_GKEY>		TMapMgrBaseGKey;
typedef TMapMgrBaseGKey::iterator		TMapMgrBaseGKeyItor;

#endif