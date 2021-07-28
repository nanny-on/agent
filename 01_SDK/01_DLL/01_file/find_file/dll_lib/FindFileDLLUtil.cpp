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

//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "FindFileDLLUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIFFDLLUtil*	t_ASIFFDLLUtil = NULL;

CASIFFDLLUtil::CASIFFDLLUtil()
{
	m_hDLL		= NULL;
	m_lpObject	= NULL;
}
//--------------------------------------------------------------------------

CASIFFDLLUtil::~CASIFFDLLUtil()
{
	if(m_hDLL)
	{
		FreeLibraryExt();
	}
	return;
}
//--------------------------------------------------------------------------

INT32			CASIFFDLLUtil::LoadLibraryExt(LPCTSTR lpDLLPath)
{
	if(m_hDLL)		return 0;

	if( (m_hDLL = LoadLibrary(lpDLLPath)) == NULL)		return -1;

	return 0;
}
//--------------------------------------------------------------------------

INT32			CASIFFDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		if(m_lpObject)
		{
			ASIFF_Free();
		}
		
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
	return 0;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


INT32	CASIFFDLLUtil::ASIFF_Init(PASI_FF_INIT_INFO pafii)
{
	if(!m_hDLL)		return -100;

	fn_ASIFF_InitType fnASIFF_Init = (fn_ASIFF_InitType)GetProcAddress(m_hDLL, "ASIFF_Init");
	if(!fnASIFF_Init)	return -101;

	return fnASIFF_Init(&m_lpObject, pafii, sizeof(ASI_FF_INIT_INFO));
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_Free()
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_FreeType fnASIFF_Free = (fn_ASIFF_FreeType)GetProcAddress(m_hDLL, "ASIFF_Free");
	if(!fnASIFF_Free)	return -101;

	INT32 nRtn = fnASIFF_Free(m_lpObject);
	m_lpObject = NULL;

	return nRtn;
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_DeleteFindFileWork(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_DeleteFindFileWorkType fnASIFF_DeleteFindFileWork = (fn_ASIFF_DeleteFindFileWorkType)GetProcAddress(m_hDLL, "ASIFF_DeleteFindFileWork");
	if(!fnASIFF_DeleteFindFileWork)	return -101;

	return fnASIFF_DeleteFindFileWork(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_AddSearchDirPath(UINT32 nOrderID, LPCTSTR lpSearchPath)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_AddSearchDirPathType fnASIFF_AddSearchDirPath = (fn_ASIFF_AddSearchDirPathType)GetProcAddress(m_hDLL, "ASIFF_AddSearchDirPath");
	if(!fnASIFF_AddSearchDirPath)	return -101;

	return fnASIFF_AddSearchDirPath(m_lpObject, nOrderID, lpSearchPath);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_ClearSearchDirPath(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_ClearSearchDirPathType fnASIFF_ClearSearchDirPath = (fn_ASIFF_ClearSearchDirPathType)GetProcAddress(m_hDLL, "ASIFF_ClearSearchDirPath");
	if(!fnASIFF_ClearSearchDirPath)	return -101;

	return fnASIFF_ClearSearchDirPath(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------


INT32	CASIFFDLLUtil::ASIFF_AddFileMask(UINT32 nOrderID, LPCTSTR lpFileMask)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_AddFileMaskType fnASIFF_AddFileMask = (fn_ASIFF_AddFileMaskType)GetProcAddress(m_hDLL, "ASIFF_AddFileMask");
	if(!fnASIFF_AddFileMask)	return -101;

	return fnASIFF_AddFileMask(m_lpObject, nOrderID, lpFileMask);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_DelFileMask(UINT32 nOrderID, LPCTSTR lpFileMask)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_DelFileMaskType fnASIFF_DelFileMask = (fn_ASIFF_DelFileMaskType)GetProcAddress(m_hDLL, "ASIFF_DelFileMask");
	if(!fnASIFF_DelFileMask)	return -101;

	return fnASIFF_DelFileMask(m_lpObject, nOrderID, lpFileMask);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_ClearFileMask(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_ClearFileMaskType fnASIFF_ClearFileMask = (fn_ASIFF_ClearFileMaskType)GetProcAddress(m_hDLL, "ASIFF_ClearFileMask");
	if(!fnASIFF_ClearFileMask)	return -101;

	return fnASIFF_ClearFileMask(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------


INT32	CASIFFDLLUtil::ASIFF_AddExceptPath(UINT32 nOrderID, LPCTSTR lpExceptPath)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_AddExceptPathType fnASIFF_AddExceptPath = (fn_ASIFF_AddExceptPathType)GetProcAddress(m_hDLL, "ASIFF_AddExceptPath");
	if(!fnASIFF_AddExceptPath)	return -101;

	return fnASIFF_AddExceptPath(m_lpObject, nOrderID, lpExceptPath);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_DelExceptPath(UINT32 nOrderID, LPCTSTR lpExceptPath)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_DelExceptPathType fnASIFF_DelExceptPath = (fn_ASIFF_DelExceptPathType)GetProcAddress(m_hDLL, "ASIFF_DelExceptPath");
	if(!fnASIFF_DelExceptPath)	return -101;

	return fnASIFF_DelExceptPath(m_lpObject, nOrderID, lpExceptPath);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_ClearExceptPath(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_ClearExceptPathType fnASIFF_ClearExceptPath = (fn_ASIFF_ClearExceptPathType)GetProcAddress(m_hDLL, "ASIFF_ClearExceptPath");
	if(!fnASIFF_ClearExceptPath)	return -101;

	return fnASIFF_ClearExceptPath(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_AddFileDateTime(UINT32 nOrderID, UINT32 nType, UINT32 nChkTime)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_AddFileDateTimeType fnASIFF_AddFileDateTime = (fn_ASIFF_AddFileDateTimeType)GetProcAddress(m_hDLL, "ASIFF_AddFileDateTime");
	if(!fnASIFF_AddFileDateTime)	return -101;

	return fnASIFF_AddFileDateTime(m_lpObject, nOrderID, nType, nChkTime);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_DelFileDateTime(UINT32 nOrderID, UINT32 nType)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_DelFileDateTimeType fnASIFF_DelFileDateTime = (fn_ASIFF_DelFileDateTimeType)GetProcAddress(m_hDLL, "ASIFF_DelFileDateTime");
	if(!fnASIFF_DelFileDateTime)	return -101;

	return fnASIFF_DelFileDateTime(m_lpObject, nOrderID, nType);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_ClearFileDateTime(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_ClearFileDateTimeType fnASIFF_ClearFileDateTime = (fn_ASIFF_ClearFileDateTimeType)GetProcAddress(m_hDLL, "ASIFF_ClearFileDateTime");
	if(!fnASIFF_ClearFileDateTime)	return -101;

	return fnASIFF_ClearFileDateTime(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------


INT32	CASIFFDLLUtil::ASIFF_AddFileFindOption(UINT32 nOrderID, UINT32 nFindOption)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_AddFileFindOptionType fnASIFF_AddFileFindOption = (fn_ASIFF_AddFileFindOptionType)GetProcAddress(m_hDLL, "ASIFF_AddFileFindOption");
	if(!fnASIFF_AddFileFindOption)	return -101;

	return fnASIFF_AddFileFindOption(m_lpObject, nOrderID, nFindOption);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_ClearFileFindOption(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_ClearFileFindOptionType fnASIFF_ClearFileFindOption = (fn_ASIFF_ClearFileFindOptionType)GetProcAddress(m_hDLL, "ASIFF_ClearFileFindOption");
	if(!fnASIFF_ClearFileFindOption)	return -101;

	return fnASIFF_ClearFileFindOption(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------


INT32	CASIFFDLLUtil::ASIFF_SearchDirFile(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_SearchDirFileType fnASIFF_SearchDirFile = (fn_ASIFF_SearchDirFileType)GetProcAddress(m_hDLL, "ASIFF_SearchDirFile");
	if(!fnASIFF_SearchDirFile)	return -101;

	return fnASIFF_SearchDirFile(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_SearchDirFileThread(UINT32 nOrderID)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_SearchDirFileThreadType fnASIFF_SearchDirFileThread = (fn_ASIFF_SearchDirFileThreadType)GetProcAddress(m_hDLL, "ASIFF_SearchDirFileThread");
	if(!fnASIFF_SearchDirFileThread)	return -101;

	return fnASIFF_SearchDirFileThread(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_GetFindFileItem(UINT32 nOrderID, PASI_FF_FILE_ITEM pAFFIList, PUINT32 nListSize, PASI_FF_FILE_RESULT pAFFR)
{	
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_GetFindFileItemType fnASIFF_GetFindFileItem = (fn_ASIFF_GetFindFileItemType)GetProcAddress(m_hDLL, "ASIFF_GetFindFileItem");
	if(!fnASIFF_GetFindFileItem)	return -101;

	return fnASIFF_GetFindFileItem(m_lpObject, nOrderID, pAFFIList, nListSize, pAFFR);
}

INT32	CASIFFDLLUtil::ASIFF_IsDocFileFormat(LPCTSTR pFilePath, INT32 *pnFileType)
{
	if(!m_hDLL)
		return -100;
	if(!m_lpObject)
		return -101;

	fn_ASIFF_IsDocFileFormatType fnASIFF_IsDocFileFormat = (fn_ASIFF_IsDocFileFormatType)GetProcAddress(m_hDLL, "ASIFF_IsDocFileFormat");
	if(!fnASIFF_IsDocFileFormat)
		return -102;

	return fnASIFF_IsDocFileFormat(m_lpObject, pFilePath, pnFileType);
}

INT32	CASIFFDLLUtil::ASIFF_StopFindFileWork(UINT32 nOrderID)
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_StopFindFileWorkType fnASIFF_StopFindFileWork = (fn_ASIFF_StopFindFileWorkType)GetProcAddress(m_hDLL, "ASIFF_StopFindFileWork");
	if(!fnASIFF_StopFindFileWork)	return -101;

	return fnASIFF_StopFindFileWork(m_lpObject, nOrderID);
}
//--------------------------------------------------------------------------

INT32	CASIFFDLLUtil::ASIFF_StopFindFileWorkAll()
{
	if(!m_hDLL)		return -100;
	if(!m_lpObject)	return -101;

	fn_ASIFF_StopFindFileWorkAllType fnASIFF_StopFindFileWorkAll = (fn_ASIFF_StopFindFileWorkAllType)GetProcAddress(m_hDLL, "ASIFF_StopFindFileWorkAll");
	if(!fnASIFF_StopFindFileWorkAll)	return -101;

	return fnASIFF_StopFindFileWorkAll(m_lpObject);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

