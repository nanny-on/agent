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

#ifndef _MANAGE_PO_FA_DEL_FILE_AFTER_BOOT_H__
#define _MANAGE_PO_FA_DEL_FILE_AFTER_BOOT_H__
//---------------------------------------------------------------------------

class CManagePoFaDelFileAfterBoot : public CManageBase<DB_DEL_FILE_AFTER_BOOT>
{
public:
	INT32		LoadDBMS();

public:
	INT32		GetDelFileListCount();
	VOID		DelFileListAfterBoot();
	INT32		InsertFilePathAfterBoot(LPCSTR pcDelFilePath);

private:
	VOID		DeleteFilePathAfterBoot(UINT32 nID, LPCSTR pcDelFilePath);

public:
	CManagePoFaDelFileAfterBoot();
    ~CManagePoFaDelFileAfterBoot();

};

extern CManagePoFaDelFileAfterBoot*	t_ManagePoFaDelFileAfterBoot;

#endif /*_MANAGE_PO_FA_DEL_FILE_AFTER_BOOT_H__*/
