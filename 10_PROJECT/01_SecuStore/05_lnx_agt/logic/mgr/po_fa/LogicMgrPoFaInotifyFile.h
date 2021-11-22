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

//---------------------------------------------------------------------------

#ifndef _LOGIC_MGR_PO_FA_INOTIFY_FILE_H__
#define _LOGIC_MGR_PO_FA_INOTIFY_FILE_H__
//---------------------------------------------------------------------------

class CLogicMgrPoFaInotifyFile	: public CLogicBase
{
public:
	INT32		InsertInotifyPath(PNOTIFY_PATH pNotifyPath);
	INT32		DeleteInotifyPath(PNOTIFY_PATH pNotifyPath);
	INT32		GetInotifyPathCount();
	INT32		GetAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nCount);
	VOID		DelAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nCount);
public:
	CLogicMgrPoFaInotifyFile();
    ~CLogicMgrPoFaInotifyFile();
};

extern CLogicMgrPoFaInotifyFile*		t_LogicMgrPoFaInotifyFile;

#endif /*_LOGIC_MGR_PO_FA_INOTIFY_FILE_H__*/
