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

#include "stdafx.h"
#include "com_struct.h"
#include "ManageLinkEnv.h"

//---------------------------------------------------------------------------

CManageLinkEnv*	t_ManageLinkEnv;

//---------------------------------------------------------------------------

CManageLinkEnv::CManageLinkEnv()
{
}
//---------------------------------------------------------------------------

CManageLinkEnv::~CManageLinkEnv()
{
	
}
//---------------------------------------------------------------------------

INT32	CManageLinkEnv::IsExistInfo(UINT32 nKey, UINT32 nID)
{
	return IsExistKeyIDMap(nKey, nID);
}
//---------------------------------------------------------------------------
