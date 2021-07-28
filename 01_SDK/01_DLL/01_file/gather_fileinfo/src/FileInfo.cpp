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

// FileInfo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "com_struct.h"
#include "ElfInfo.h"
#include "FileInfo.h"

//File Type String
INT32 ASIFI_GetFileDescription(char *pcFileName, char *pcFileDescr, INT32 nLength)
{
	CELFInfo t_ElfInfo;
	INT32 nRetVal = 0;
	if(pcFileName == NULL || pcFileDescr == NULL || nLength < 1)
		return -1;
	nRetVal = t_ElfInfo.Open(pcFileName);
	if(nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}

	t_ElfInfo.GetFileElfTypeString(pcFileDescr, nLength);
	t_ElfInfo.Close();

	return 0;
}
//-----------------------------------------------------------------------------------------

//Entry Point
INT32 ASIFI_GetFileVersion(char *pcFileName, char *pcFileVersion, INT32 nLength)
{
	CELFInfo t_ElfInfo;
	INT32 nRetVal = 0;
	if(pcFileName == NULL || pcFileVersion == NULL || nLength < 1)
		return -1;
	nRetVal = t_ElfInfo.Open(pcFileName);
	if(nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	t_ElfInfo.GetFileElfEntryPointString(pcFileVersion, nLength);
	t_ElfInfo.Close();

	return 0;
}
//-----------------------------------------------------------------------------------------
//OS ABI Value
INT32 ASIFI_GetFileSpecialValue(char *pcFileName, char *pcFileKey, char *pcFileValue, INT32 nLength)
{
	CELFInfo t_ElfInfo;
	INT32 nRetVal = 0;

	if(pcFileName == NULL)
		return -1;

	nRetVal = t_ElfInfo.Open(pcFileName);
	if(nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}
	t_ElfInfo.GetFileElfAbiString(pcFileValue, nLength);
	t_ElfInfo.Close();
	
	return 0;
}
//-----------------------------------------------------------------------------------------

INT32 ASIFI_GetFileLanguage(char *pcFileName, INT32& nLanguageType)
{
	nLanguageType = ASI_LOCALE_TYPE_KOREA;
	return 0;
}
//-----------------------------------------------------------------------------------------

INT32	ASIFI_GetFileElfMagic(char *pcFileName, INT32& nElfMagic)
{
	CELFInfo t_ElfInfo;
	INT32 nRetVal = 0;
	nElfMagic = 0;
	if(pcFileName == NULL)
		return -1;
	nRetVal = t_ElfInfo.Open(pcFileName);
	if(nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}

	if(t_ElfInfo.Is64Bit())
		nElfMagic = ASI_FI_ELF_CLASS_64BIT;
	else
		nElfMagic = ASI_FI_ELF_CLASS_32BIT;

	t_ElfInfo.Close();
	return 0;
}
//-----------------------------------------------------------------------------------------

INT32	ASIFI_GetFileElfType(char *pcFilePath, INT32& nElfType, char *pcFileName)
{
	CELFInfo t_ElfInfo;
	INT32 nRetVal = 0;
	nElfType = 0;	
	if(pcFilePath == NULL)
		return -1;
	nRetVal = t_ElfInfo.Open(pcFilePath, pcFileName);
	if(nRetVal != 0)
	{
		nRetVal -= 1000;
		return nRetVal;
	}

	nElfType = t_ElfInfo.GetFileElfType();
	t_ElfInfo.Close();

	return 0;
}
//-----------------------------------------------------------------------------------------
