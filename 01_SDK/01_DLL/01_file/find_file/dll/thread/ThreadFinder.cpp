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

// ThreadFinder.cpp : implementation file
//

#include "stdafx.h"
#include "com_struct.h"
#include "com_struct_find_work.h"
#include "FindFileUtil.h"
#include "ThreadFinder.h"

/////////////////////////////////////////////////////////////////////////////
// CThreadFinder

CThreadFinder::CThreadFinder()
{
}

CThreadFinder::~CThreadFinder()
{
}

BOOL CThreadFinder::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CThreadFinder::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CThreadBase::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CThreadFinder message handlers

int CThreadFinder::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_nRunFlag = 1;
	INT32 nMaxFindDirNum = 0;
	while(GetContinue())
	{
		if(!m_nPause)
		{
			if(ChkWorkFindDir() == 0 || nMaxFindDirNum == 1)
			{
				if(ChkWorkFindFile())
				{
					nMaxFindDirNum = 0;
				}
				else
				{
					Sleep(10);
				}
			}			
			else
			{
				nMaxFindDirNum++;
			}
		}		
		else
		{
			Sleep(10);
		}		
	}
	m_nRunFlag = 0;	
	return 0;
}
//---------------------------------------------------------------------------

INT32		CThreadFinder::ChkWorkFindDir()
{
	FIND_DIR_ITEM tFDI;
	TListStr tNameList;
	CFindFileUtil* tFFU = (CFindFileUtil*)m_pOwnerClass;
	INT32 nRtn = 0;
	UINT32 nWorkNum = 0;

	if(tFFU == NULL)
		return 0;

	if(tFFU->GetFindSubDirItem(tFDI))
	{
		tFFU->Recursive_SearchDir(tFDI.nOrderID, (char *)tFDI.strSearchDir.c_str(), "", (tFDI.nSubSearch ? -1 : 0), tNameList, NULL, NULL, &m_nContinue);
		if(m_nContinue && tNameList.size())
		{
			tFFU->SetFindFileWork_TotalDir(tFDI.nOrderID, tNameList.size(), 1);
			tFFU->AddFindFileDirItem(tFDI.nOrderID, tNameList, &nWorkNum);
		}
		nRtn = 1;
	}
	return nRtn;
}
//---------------------------------------------------------------------------

INT32		CThreadFinder::ChkWorkFindFile()
{
	CFindFileUtil* tFFU = (CFindFileUtil*)m_pOwnerClass;
	INT32 nRtn = 0;
	if(tFFU == NULL)
		return 0;

	UINT32 nOrderID = 0;
	TListFindDirItem tSFDIList;
	if(tFFU->GetFindFileDirItem(tSFDIList, nOrderID))
	{
		TListFindFileItem tAFFIList;
		TListFindDirItemItor begin, end;
		begin = tSFDIList.begin();	end = tSFDIList.end();
		for(begin; begin != end && m_nContinue; begin++)
		{
			tFFU->Recursive_SearchFile(begin->nOrderID, begin->strSearchDir, tAFFIList, &m_nContinue);
		}				
				
		if(m_nContinue)
		{
			tFFU->AddFindFileItemList(nOrderID, (UINT32)tSFDIList.size(), tAFFIList);			
		}
		nRtn = 1;
	}
	return nRtn;
}
//---------------------------------------------------------------------------
