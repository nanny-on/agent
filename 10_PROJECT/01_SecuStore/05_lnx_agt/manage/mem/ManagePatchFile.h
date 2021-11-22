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


#ifndef ManagePatchFileH
#define ManagePatchFileH
//---------------------------------------------------------------------------

class CManagePatchFile : public CManageBase<MEM_PATCH_FILE>
{


public:
	INT32		SetDnFileList(UINT32 nID, String strFileList, CHAR szTok);
	INT32		SetDnFileList(UINT32 nID, TMapIDStr& tDnMap);

public:
	INT32		ChgValidPatchFile(UINT32 nPatchID);
	INT32		ChgValidPatchFile(TMapIDStr& tFileMap);
	INT32		IsReadyPatchFile(UINT32 nID);
	INT32		IsReadyPatchFile(TMapIDStr& tFileMap);

public:
	CManagePatchFile();
    ~CManagePatchFile();

};

extern CManagePatchFile*	t_ManagePatchFile;

#endif
