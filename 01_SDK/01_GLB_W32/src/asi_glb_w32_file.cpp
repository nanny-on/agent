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
 */

#include "stdafx.h"
#include "com_struct.h"
#include "asi_lnx_define_w32.h"
#include "asi_glb_w32_file.h"
#include "asi_glb_w32_handle.h"

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, PVOID lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	INT32 nFd = 0;
	INT32 nOpenFlag = O_RDWR;

	switch(dwCreationDisposition)
	{
		case CREATE_NEW:		nOpenFlag += O_CREAT;				break;
		case CREATE_ALWAYS:		nOpenFlag += (O_CREAT | O_TRUNC);	break;
		case OPEN_EXISTING:		nOpenFlag += 0;						break;
		case OPEN_ALWAYS:		nOpenFlag += O_CREAT;				break;
	}

	nFd = open(lpFileName, nOpenFlag);

	if(nFd != (INT32)(LONG)INVALID_HANDLE_VALUE)
		SetHandleInformation((HANDLE)(LONG)nFd, 0, HANDLE_TYPE_FILE);

	return (HANDLE)(nFd);
}
//------------------------------------------------------------------------------------------

BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, PVOID lpOverlapped)
{
	INT32 nFn = (INT32)(LONG)hFile;
	if(nFn == -1 || lpBuffer == NULL || lpNumberOfBytesRead == NULL)
	{
		return FALSE;
	}

	*lpNumberOfBytesRead = read(nFn, lpBuffer, nNumberOfBytesToRead);
	if(*lpNumberOfBytesRead == (DWORD)-1)
		return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------------------------

BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, PVOID lpOverlapped)
{
	INT32 nFn = (INT32)(LONG)hFile;

	*lpNumberOfBytesWritten = write(nFn, lpBuffer, nNumberOfBytesToWrite);
	if(*lpNumberOfBytesWritten == -1)	return FALSE;

	return TRUE;
}
//------------------------------------------------------------------------------------------

int GetFileSize(HANDLE hFile, UINT32 *pdwFileLen)
{
	INT32 nFn = (INT32)(LONG)hFile;
	return GetFileSizeWithFd(nFn, pdwFileLen);
}

//------------------------------------------------------------------------------------------

DWORD SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	INT32 nFd = (INT32)(LONG)hFile;
	return SetFilePointer(nFd, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
}
//------------------------------------------------------------------------------------------

DWORD SetFilePointer(int nFd, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
	off_t nMove = lDistanceToMove;
	if(lpDistanceToMoveHigh)
		nMove = (off_t)MAKEUINT64(lDistanceToMove, *lpDistanceToMoveHigh);

	return lseek64(nFd, nMove, dwMoveMethod);
}
//------------------------------------------------------------------------------------------


