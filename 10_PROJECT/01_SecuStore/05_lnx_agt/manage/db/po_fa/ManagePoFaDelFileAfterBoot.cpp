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
#include "ManagePoFaDelFileAfterBoot.h"

//---------------------------------------------------------------------------

CManagePoFaDelFileAfterBoot*	t_ManagePoFaDelFileAfterBoot = NULL;


//---------------------------------------------------------------------------

CManagePoFaDelFileAfterBoot::CManagePoFaDelFileAfterBoot()
{
	t_DBMgrPoFaDelFileAfterBoot	= new CDBMgrPoFaDelFileAfterBoot();    
}
//---------------------------------------------------------------------------

CManagePoFaDelFileAfterBoot::~CManagePoFaDelFileAfterBoot()
{
	if(t_DBMgrPoFaDelFileAfterBoot)
	{
		delete t_DBMgrPoFaDelFileAfterBoot;
		t_DBMgrPoFaDelFileAfterBoot = NULL;
	}
}
//---------------------------------------------------------------------------

INT32		CManagePoFaDelFileAfterBoot::LoadDBMS()
{
	TListDelFileAfterBoot tDBDelFileAfterBootList;
    TListDelFileAfterBootItor begin, end;

	if(t_DBMgrPoFaDelFileAfterBoot == NULL)
	{
		return -1;
	}
	if(SetER(t_DBMgrPoFaDelFileAfterBoot->LoadExecute(&tDBDelFileAfterBootList)))
    {
    	return g_nErrRtn;
    }

    begin = tDBDelFileAfterBootList.begin();	end = tDBDelFileAfterBootList.end();
    for(begin; begin != end; begin++)
    {
    	AddItem(begin->nID, *begin);
    }
    return 0;
}
//---------------------------------------------------------------------------

INT32	CManagePoFaDelFileAfterBoot::GetDelFileListCount()
{
	return (INT32)m_tMap.size();
}


VOID	CManagePoFaDelFileAfterBoot::DelFileListAfterBoot()
{
	if(m_tMap.size() < 1)
		return;

	TMapDelFileAfterBootItor begin, end;
    begin = m_tMap.begin();	end = m_tMap.end();
    for(begin; begin != end; begin++)
    {
    	DeleteFilePathAfterBoot(begin->second.nID, begin->second.strDelFilePath.c_str());
    }
}
//---------------------------------------------------------------------------

VOID	CManagePoFaDelFileAfterBoot::DeleteFilePathAfterBoot(UINT32 nID, LPCSTR pcDelFilePath)
{
	if(pcDelFilePath == NULL || pcDelFilePath[0] == 0)
		return;

	if(unlink(pcDelFilePath) == -1)
	{
		WriteLogE("fail to delete %s on boot time (%u) (%d)", pcDelFilePath, nID, errno);
	}
	else
	{
		WriteLogN("success to delete %s on boot time (%u)", pcDelFilePath, nID);
		if(t_DBMgrPoFaDelFileAfterBoot)
			t_DBMgrPoFaDelFileAfterBoot->DeleteExecute(nID);
	}
}

INT32	CManagePoFaDelFileAfterBoot::InsertFilePathAfterBoot(LPCSTR pcDelFilePath)
{
	INT32 nRetVal = 0;
	if(t_DBMgrPoFaDelFileAfterBoot == NULL || pcDelFilePath == NULL || pcDelFilePath[0] == 0)
		return -1;

	nRetVal = t_DBMgrPoFaDelFileAfterBoot->InsertDelFileInfo(pcDelFilePath);
	if(nRetVal != 0)
	{
		WriteLogE("fail to insert del file info (%d)", pcDelFilePath, nRetVal);
		nRetVal -= 10;
		return nRetVal;
	}
	return 0;
}


//---------------------------------------------------------------------------


