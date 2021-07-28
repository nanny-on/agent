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

#include "stdafx.h"
#include "com_struct.h"
#include "DocFileFmt.h"
#include "DocFileFmtUtil.h"


//---------------------------------------------------------------------------------

INT32 ASIDF_SetDFFmtInit(PASI_DFILE_FMT_INIT pADFFI)
{
	return 0;
}
//---------------------------------------------------------------------------------

INT32 ASIDF_GetDFFmtInfo(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg)
{	
	INT32 nRetVal = 0;
	ASI_DFILE_FMT_INFO tADFFI;
	CDocFileFmtUtil tDFFUtil;
	if(pADFFI == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "invalid input data");
		return -11;
	}

	memcpy(&tADFFI, pADFFI, sizeof(tADFFI));

	nRetVal = tDFFUtil.ProcAnalysis(&tADFFI, acLogMsg);
	if(nRetVal == 0)
		memcpy(pADFFI, &tADFFI, sizeof(tADFFI));
	return nRetVal;
}
//---------------------------------------------------------------------------------
