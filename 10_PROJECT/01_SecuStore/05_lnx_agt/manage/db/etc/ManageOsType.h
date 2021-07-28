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

//---------------------------------------------------------------------------

#ifndef ManageOsTypeH
#define ManageOsTypeH
//---------------------------------------------------------------------------

class CManageOsType : public CManageBase<DB_OS_TYPE>
{
public:
	INT32				LoadDBMS();

public:
    INT32				IsServerType(UINT32 nID);
    String				GetOsName(UINT32 nID);
    String				GetOsNameSP(UINT32 nID, UINT32 nSP);

public:
	INT32				SetPkt(MemToken& SendToken);
    INT32				SetPkt(PDB_OS_TYPE pdot, MemToken& SendToken);
    
public:
	CManageOsType();
    ~CManageOsType();

};

extern CManageOsType*	t_ManageOsType;

#endif
