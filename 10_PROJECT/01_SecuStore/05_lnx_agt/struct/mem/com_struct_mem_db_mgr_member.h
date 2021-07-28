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


#ifndef MEM_DB_MGR_MEMBER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define MEM_DB_MGR_MEMBER_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

typedef struct _mem_db_mgr_member
{
	_mem_db_mgr_member()
	{
		_init();
    }

	void _init()
	{
		strQuery = "";
		strQueryTemp = "";
		nValue = 0;
		strValue = "";

		return;
	}

	CString			strQuery;
	CString			strQueryTemp;
	UINT32			nValue;
	CString			strValue;

}MEM_DB_MGR_MEMBER, *PMEM_DB_MGR_MEMBER;

typedef list<MEM_DB_MGR_MEMBER>					TListMemDBMgrMember;
typedef TListMemDBMgrMember::iterator			TListMemDBMgrMemberItor;

typedef map<UINT32, MEM_DB_MGR_MEMBER>			TMapMemDBMgrMember;
typedef TMapMemDBMgrMember::iterator			TMapMemDBMgrMemberItor;

#endif //DB_CHANGE_INFO_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



