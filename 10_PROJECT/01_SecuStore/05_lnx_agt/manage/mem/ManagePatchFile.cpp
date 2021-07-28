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
#include "ManagePatchFile.h"

//---------------------------------------------------------------------------

CManagePatchFile*	t_ManagePatchFile = NULL;

//---------------------------------------------------------------------------

CManagePatchFile::CManagePatchFile()
{
}
//---------------------------------------------------------------------------

CManagePatchFile::~CManagePatchFile()
{
	
}
//---------------------------------------------------------------------------

INT32		CManagePatchFile::SetDnFileList(UINT32 nID, String strFileList, CHAR szTok)
{
	MEM_PATCH_FILE data;
	data.nID = nID;	
	StrToMapIDStr(strFileList, data.tDnFileMap, szTok);

	return AddItem(data);
}
//---------------------------------------------------------------------------

INT32		CManagePatchFile::SetDnFileList(UINT32 nID, TMapIDStr& tDnMap)
{
	MEM_PATCH_FILE data;
	data.nID = nID;
	data.tDnFileMap = tDnMap;

	return AddItem(data);	
}
//---------------------------------------------------------------------------

INT32			CManagePatchFile::ChgValidPatchFile(UINT32 nPatchID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32			CManagePatchFile::ChgValidPatchFile(TMapIDStr& tFileMap)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32	CManagePatchFile::IsReadyPatchFile(UINT32 nID)
{
	return 0;
}
//---------------------------------------------------------------------------

INT32	CManagePatchFile::IsReadyPatchFile(TMapIDStr& tFileMap)
{
	return 0;
}
//---------------------------------------------------------------------------
