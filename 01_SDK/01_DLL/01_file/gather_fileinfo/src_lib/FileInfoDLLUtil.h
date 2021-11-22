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


#ifndef _FILE_INFO_DLL_UTIL_H__
#define _FILE_INFO_DLL_UTIL_H__


//------------------------------------------------------
#include "com_header_file_info_dll.h"
//------------------------------------------------------


class CASIFIDLLUtil  
{
private:
	HINSTANCE m_hDLL;

public:
	INT32		LoadLibraryExt(char *pcDLLName);
	VOID		FreeLibraryExt();

public:
	char*		ASIFI_GetFileDescription(char *pcFileName, char *pcFileDescr);
	char*		ASIFI_GetFileVersion(char *pcFileName, char *pcFileVersion);
	char*		ASIFI_GetFileSpecialValue(char *pcFileName, char *pcFileKey, char *pcFileValue);
	INT32		ASIFI_GetFileLanguage(char *pcFileName);
	INT32		ASIFI_GetFileElfMagic(char *pcFileName);
	INT32		ASIFI_GetFileElfType(char *pcFilePath, char *pcFileName = NULL);

public:
	CASIFIDLLUtil();
	virtual ~CASIFIDLLUtil();

};

extern CASIFIDLLUtil*	t_ASIFIDLLUtil;

#endif /*_FILE_INFO_DLL_UTIL_H__*/