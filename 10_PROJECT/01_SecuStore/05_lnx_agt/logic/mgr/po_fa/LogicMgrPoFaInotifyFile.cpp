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
#include "LogicMgrPoFaInotifyFile.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

CLogicMgrPoFaInotifyFile*		t_LogicMgrPoFaInotifyFile = NULL;

CLogicMgrPoFaInotifyFile::CLogicMgrPoFaInotifyFile()
{
	t_ManagePoFaInotifyFile	= new CManagePoFaInotifyFile();
	if(t_ManagePoFaInotifyFile)
		t_ManagePoFaInotifyFile->LoadDBMS();
	m_strLogicName		= "mgr po fa inotify file";
}
//---------------------------------------------------------------------------

CLogicMgrPoFaInotifyFile::~CLogicMgrPoFaInotifyFile()
{
	SAFE_DELETE(t_ManagePoFaInotifyFile);
}

INT32		CLogicMgrPoFaInotifyFile::GetAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nCount)
{
	INT32 nRetVal = 0;
	if(t_ManagePoFaInotifyFile == NULL || pNotifyPath == NULL || nCount < 1)
	{
		WriteLogE("[%s] invalid input data", m_strLogicName.c_str());
		return -1;
	}
	nRetVal = t_ManagePoFaInotifyFile->GetAllInotifyPath(pNotifyPath, nCount);
	if(nRetVal != 0)
	{
		WriteLogE("[%s] fail to get all inotify file path (%d)", m_strLogicName.c_str(), nRetVal);
		nRetVal -= 100;
	}
	return nRetVal;
}

INT32		CLogicMgrPoFaInotifyFile::InsertInotifyPath(PNOTIFY_PATH pNotifyPath)
{
	INT32 nRetVal = 0;
	if(t_ManagePoFaInotifyFile == NULL || pNotifyPath == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strLogicName.c_str());
		return -1;
	}

	nRetVal = t_ManagePoFaInotifyFile->InsertInotifyPath(pNotifyPath);
	if(nRetVal != 0)
	{
		WriteLogE("[%s] fail to insert inotify file path (%d)", m_strLogicName.c_str(), nRetVal);
		nRetVal -= 100;
	}
	return nRetVal;
}

INT32		CLogicMgrPoFaInotifyFile::DeleteInotifyPath(PNOTIFY_PATH pNotifyPath)
{
	INT32 nRetVal = 0;
	if(t_ManagePoFaInotifyFile == NULL || pNotifyPath == NULL)
	{
		WriteLogE("[%s] invalid input data", m_strLogicName.c_str());
		return -1;
	}

	nRetVal = t_ManagePoFaInotifyFile->DeleteInotifyPath(pNotifyPath);
	if(nRetVal != 0)
	{
		nRetVal -= 100;
	}
	return nRetVal;
}

INT32		CLogicMgrPoFaInotifyFile::GetInotifyPathCount()
{
	if(t_ManagePoFaInotifyFile == NULL)
	{
		return 0;
	}

	return t_ManagePoFaInotifyFile->GetInotifyPathCount();
}


VOID		CLogicMgrPoFaInotifyFile::DelAllInotifyPath(PNOTIFY_PATH pNotifyPath, INT32 nCount)
{
	if(t_ManagePoFaInotifyFile != NULL)
	{
		t_ManagePoFaInotifyFile->DelAllInotifyPath(pNotifyPath, nCount);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
