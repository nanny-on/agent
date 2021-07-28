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

#ifndef DeployPolicyUtilH
#define DeployPolicyUtilH
//---------------------------------------------------------------------------

class CDeployPolicyUtil
{

public:
	INT32		SetPolicy(DB_POLICY& dp, UINT32 nPkgID, UINT64 nPolicyType);
	UINT32		GetPolicy(DB_POLICY& dp, UINT64 nPolicyType);

public:
	UINT32		GetCurPoID(UINT64 nPolicyType);
	PVOID		GetCurPoPtr(UINT64 nPolicyType);
	PVOID		GetPoPtr(UINT32 nID, UINT64 nPolicyType);


public:
	UINT64		GetPoTypeFromPoIdx(UINT32 nPoIdx);
	UINT32		GetPoIdxFromPoType(UINT64 nPoType);

public:
	INT32		ApplyPolicy(UINT64 nPolicyType, UINT32 nPoID);

public:
	CDeployPolicyUtil();
    ~CDeployPolicyUtil();
};

extern CDeployPolicyUtil*		t_DeployPolicyUtil;


#endif
