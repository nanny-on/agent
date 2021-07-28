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
#include "LogicCtrlVuln.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::ScanWindowsPatch()
{
	return SS_PTN_VULN_SCAN_RESULT_SCANNING;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::CheckWindowsPatch(UINT32 nWorkID)
{
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::MakeSkipWindowsPatch(TMapIDMap& tSkipIDMap)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::MakeSkipWindowsPatchUnit(TMapIDMap& tSkipIDMap, String strInfo)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicCtrlVuln::IsSkipWindowsPatch(TMapIDMap& tSkipIDMap, PMEM_VULN_RST pdata)
{

	return 0;
}
//---------------------------------------------------------------------------


