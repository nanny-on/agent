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

// SGCDLLUtil.cpp: implementation of the CASICOMPDLLUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "CompDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASICOMPDLLUtil* t_ASICOMPDLLUtil = NULL;

CASICOMPDLLUtil::CASICOMPDLLUtil()
{
	m_hDLL	= NULL;
}
//--------------------------------------------------------------------

CASICOMPDLLUtil::~CASICOMPDLLUtil()
{
	if(!m_hDLL)		return;

	FreeLibraryExt();
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::LoadLibraryExt(LPCTSTR lpDLLName)
{
	if(m_hDLL)
	{
		FreeLibraryExt();
	}

	m_hDLL = LoadLibrary(lpDLLName);
	if(m_hDLL == NULL)	return -1;

	return 0;
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
	}
	return 0;
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompDir(PASI_COMP_INFO paci)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_CompDir = (fn_ASICOMP_CompDirType)GetProcAddress(m_hDLL, "ASICOMP_CompDir")) == NULL)		return -1;
	
	return fn_ASICOMP_CompDir(paci);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_DecompDir(PASI_DECOMP_INFO padi)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_DecompDir = (fn_ASICOMP_DecompDirType)GetProcAddress(m_hDLL, "ASICOMP_DecompDir")) == NULL)		return -1;
	
	return fn_ASICOMP_DecompDir(padi);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_SetNotifyState(PVOID lParam, PVOID pNotifyState)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_SetNotifyState = (fn_ASICOMP_SetNotifyStateType)GetProcAddress(m_hDLL, "ASICOMP_SetNotifyState")) == NULL)		return -1;
	
	return fn_ASICOMP_SetNotifyState(lParam, pNotifyState);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_CompFile = (fn_ASICOMP_CompFileType)GetProcAddress(m_hDLL, "ASICOMP_CompFile")) == NULL)		return -1;
	
	return fn_ASICOMP_CompFile(lpFileName, lpSaveDir, lpSaveFile);
}
//--------------------------------------------------------------------


INT32	CASICOMPDLLUtil::ASICOMP_DecompFile(LPCTSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSavedFile)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_DecompFile = (fn_ASICOMP_DecompFileType)GetProcAddress(m_hDLL, "ASICOMP_DecompFile")) == NULL)		return -1;
	
	return fn_ASICOMP_DecompFile(lpFileName, lpSaveDir, lpSavedFile);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompFileW(LPCWSTR lpFileName, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	if(!m_hDLL)	return -1000;	

	if((fn_ASICOMP_CompFileW = (fn_ASICOMP_CompFileWType)GetProcAddress(m_hDLL, "ASICOMP_CompFileW")) == NULL)		return -1;

	return fn_ASICOMP_CompFileW(lpFileName, lpSaveDir, lpSaveFile);
}
//--------------------------------------------------------------------


INT32	CASICOMPDLLUtil::ASICOMP_DecompFileW(LPCTSTR lpFileName, LPCWSTR lpSaveDir, LPCWSTR lpSavedFile)
{
	if(!m_hDLL)	return -1000;	

	if((fn_ASICOMP_DecompFileW = (fn_ASICOMP_DecompFileWType)GetProcAddress(m_hDLL, "ASICOMP_DecompFileW")) == NULL)		return -1;

	return fn_ASICOMP_DecompFileW(lpFileName, lpSaveDir, lpSavedFile);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompBufferAndSaveFile(LPCTSTR lpFilePath, LPCTSTR lpFileName, BYTE* lpSrcBuffer, INT32 nBufferLen, LPCTSTR lpSaveDir, LPCTSTR lpSaveFile)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_CompBufferAndSaveFile = (fn_ASICOMP_CompBufferAndSaveFileType)GetProcAddress(m_hDLL, "ASICOMP_CompBufferAndSaveFile")) == NULL)		return -1;
	
	return fn_ASICOMP_CompBufferAndSaveFile(lpFilePath, lpFileName, lpSrcBuffer, nBufferLen, lpSaveDir, lpSaveFile);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_DecompFileAndBuffer(LPCTSTR lpFileName, BYTE** lpOutBuffer, INT32& nOutBuffeLen)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_DecompFileAndBuffer = (fn_ASICOMP_DecompFileAndBufferType)GetProcAddress(m_hDLL, "ASICOMP_DecompFileAndBuffer")) == NULL)		return -1;
	
	return fn_ASICOMP_DecompFileAndBuffer(lpFileName, lpOutBuffer, nOutBuffeLen);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompBufferAndBuffer(BYTE* lpInBuffer, INT32 nLen, LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_CompBufferAndBuffer = (fn_ASICOMP_CompBufferAndBufferType)GetProcAddress(m_hDLL, "ASICOMP_CompBufferAndBuffer")) == NULL)		return -1;
	
	return fn_ASICOMP_CompBufferAndBuffer(lpInBuffer, nLen, lpRelativeFilePath, lpAbsoluteFileName, lpOutBuffer, nOutBufferLen, nUsedComp);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_CompFileAndBuffer(LPCTSTR lpRelativeFilePath, LPCTSTR lpAbsoluteFileName, BYTE** lpOutBuffer, INT32& nOutBufferLen, INT32 nUsedComp)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_CompFileAndBuffer = (fn_ASICOMP_CompFileAndBufferType)GetProcAddress(m_hDLL, "ASICOMP_CompFileAndBuffer")) == NULL)		return -1;
	
	return fn_ASICOMP_CompFileAndBuffer(lpRelativeFilePath, lpAbsoluteFileName, lpOutBuffer, nOutBufferLen, nUsedComp);
}
//--------------------------------------------------------------------


INT32	CASICOMPDLLUtil::ASICOMP_DecompBufferAndSave(BYTE* lpSrcBuffer, INT32 nSrcBuffLen, LPCTSTR lpSaveDir, LPTSTR lpSaveFile, LPCTSTR lpSaveFileAddOn, UINT32 nUsedComp)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_DecompBufferAndSave = (fn_ASICOMP_DecompBufferAndSaveType)GetProcAddress(m_hDLL, "ASICOMP_DecompBufferAndSave")) == NULL)		return -1;
	
	return fn_ASICOMP_DecompBufferAndSave(lpSrcBuffer, nSrcBuffLen, lpSaveDir, lpSaveFile, lpSaveFileAddOn, nUsedComp);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_FreeMemory(BYTE* lpOutBuffer)
{
	if(!m_hDLL)	return -1000;	
	
	if((fn_ASICOMP_FreeMemory = (fn_ASICOMP_FreeMemoryType)GetProcAddress(m_hDLL, "ASICOMP_FreeMemory")) == NULL)		return -1;
	
	return fn_ASICOMP_FreeMemory(lpOutBuffer);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_S_CompFile(PASI_COMP_SECU pACS, INT32 nLen)
{
	if(!m_hDLL)	return -1000;	

	ASICOMP_S_CompFileType fnASICOMP_S_CompFile = (ASICOMP_S_CompFileType)GetProcAddress(m_hDLL, "ASICOMP_S_CompFile"); 

	if(fnASICOMP_S_CompFile == NULL)		return -1;
	return fnASICOMP_S_CompFile(pACS, nLen);
}
//--------------------------------------------------------------------

INT32	CASICOMPDLLUtil::ASICOMP_S_DecompFile(PASI_COMP_SECU pACS, INT32 nLen, char *acLogMsg)
{
	if(!m_hDLL)	return -1000;	

	ASICOMP_S_DecompFileType fnASICOMP_S_CompFile = (ASICOMP_S_DecompFileType)GetProcAddress(m_hDLL, "ASICOMP_S_DecompFile"); 

	if(fnASICOMP_S_CompFile == NULL)		return -1;
	return fnASICOMP_S_CompFile(pACS, nLen, acLogMsg);
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

