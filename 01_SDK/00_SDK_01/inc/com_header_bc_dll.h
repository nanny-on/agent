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
 */


#ifndef     COM_HEADER_ASIBC_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define     COM_HEADER_ASIBC_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//------------------------------------------------------------------------------

typedef struct _asi_bc_info
{
	_asi_bc_info()
	{
		memset(szHomePath, 0, MAX_PATH);
		memset(szSaveSubPath, 0, MAX_PATH);
		memset(szSaveFileName, 0, MAX_PATH);
		lpSkipList = NULL;

		memset(szSHADLL, 0, MAX_PATH);
		memset(szSeedDLL, 0, MAX_PATH);

		nScanRst = 0;
		lpScanRst = NULL;
		nScanRstLen = 0;
	}

	TCHAR	szHomePath[MAX_PATH];
	TCHAR	szSaveSubPath[MAX_PATH];
	TCHAR	szSaveFileName[MAX_PATH];
	LPCTSTR lpSkipList;
	
	TCHAR	szSHADLL[MAX_PATH];	
	TCHAR	szSeedDLL[MAX_PATH];	

	INT32	nScanRst;
	LPCTSTR	lpScanRst;
	INT32	nScanRstLen;
	
}ASIBC_INFO, *PASIBC_INFO;
//------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef INT32 (*ASIBC_MakeFileType)(PASIBC_INFO pBinCaptureInfo, UINT32 nSize);
typedef INT32 (*ASIBC_ChkFileType)(PASIBC_INFO pBinCaptureInfo, UINT32 nSize);

//--------------------------------------------------------------------------

#endif      //COM_HEADER_ASIBC_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
