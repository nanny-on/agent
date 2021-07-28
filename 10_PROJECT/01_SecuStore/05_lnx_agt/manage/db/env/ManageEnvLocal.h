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

#ifndef ManageEnvLocalH
#define ManageEnvLocalH
//---------------------------------------------------------------------------

class CManageEnvLocal : public CManageBase<DB_ENV_LOCAL>
{
public:
	INT32		LoadDBMS();

public:
	INT32					AddEnvLocal(DB_ENV_LOCAL&	data);
    INT32					EditEnvLocal(DB_ENV_LOCAL&	data);
    INT32					DelEnvLocal(UINT32 nID);

public:
	PDB_ENV_LOCAL			GetEnvLocal(UINT32 nChkType, UINT32 nChkCode);

public:
	void					SetEnvLocalNumber(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey, UINT32 nValue);
	UINT32					GetEnvLocalNumber(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey);
	void					SetEnvLocalString(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey, String strValue);
	String					GetEnvLocalString(UINT32 nChkType, UINT32 nChkCode, UINT32 nKey);
	INT32					IsExistLargeThenValue(UINT32 nChkType, UINT32 nChkCode, UINT32 nValue);

public:
	CManageEnvLocal();
    ~CManageEnvLocal();

};

extern CManageEnvLocal*	t_ManageEnvLocal;

#endif
