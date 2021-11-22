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

//
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "com_struct.h"
#include "FileInfoDLLUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CASIFIDLLUtil*	t_ASIFIDLLUtil = NULL;

CASIFIDLLUtil::CASIFIDLLUtil()
{
	m_hDLL = NULL;
}
//--------------------------------------------------------------------

CASIFIDLLUtil::~CASIFIDLLUtil()
{
	FreeLibraryExt();
}
//--------------------------------------------------------------------

INT32 CASIFIDLLUtil::LoadLibraryExt(char* pcDLLName)
{
	if(m_hDLL)
		return 0;

	if(pcDLLName == NULL)
	{
		return -1;
	}

	m_hDLL = LoadLibrary(pcDLLName);
	if( m_hDLL == NULL)
	{	
		return -2;
	}
	return 0;
}
//--------------------------------------------------------------------

VOID	CASIFIDLLUtil::FreeLibraryExt()
{
	if(m_hDLL)
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
}
//--------------------------------------------------------------------

char* CASIFIDLLUtil::ASIFI_GetFileDescription(char* pcFileName, char* pcFileDescr)
{
	ASIFI_GetFileDescriptionType fASIFI_GetFileDescription = NULL;
	if(m_hDLL == NULL || pcFileName == NULL || pcFileDescr == NULL)
		return NULL;
	
	fASIFI_GetFileDescription = (ASIFI_GetFileDescriptionType)GetProcAddress(m_hDLL, "ASIFI_GetFileDescription");
	if(fASIFI_GetFileDescription == NULL)
	{
		return NULL;
	}

	memset(pcFileDescr, 0, CHAR_MAX_SIZE);
	
	if(fASIFI_GetFileDescription(pcFileName, pcFileDescr, CHAR_MAX_SIZE) != 0)
	{
		return NULL;
	}
	return pcFileDescr;
}
//--------------------------------------------------------------------

char*	CASIFIDLLUtil::ASIFI_GetFileVersion(char* pcFileName, char* pcFileVersion)
{
	ASIFI_GetFileVersionType fASIFI_GetFileVersion = NULL;
	if(m_hDLL == NULL || pcFileName == NULL || pcFileVersion == NULL)
		return NULL;

	fASIFI_GetFileVersion = (ASIFI_GetFileVersionType)GetProcAddress(m_hDLL, "ASIFI_GetFileVersion");
	if(fASIFI_GetFileVersion == NULL)
	{
		return NULL;
	}

	memset(pcFileVersion, 0, CHAR_MAX_SIZE);

	if(fASIFI_GetFileVersion(pcFileName, pcFileVersion, CHAR_MAX_SIZE) != 0)
	{
		return NULL;
	}
	return pcFileVersion;
}
//--------------------------------------------------------------------

char*	CASIFIDLLUtil::ASIFI_GetFileSpecialValue(char* pcFileName, char* pcFileKey, char* pcFileValue)
{
	ASIFI_GetFileSpecialValueType fASIFI_GetFileSpecialValue = NULL;
	if(m_hDLL == NULL || pcFileName == NULL || pcFileKey == NULL || pcFileValue == NULL)
		return NULL;

	fASIFI_GetFileSpecialValue = (ASIFI_GetFileSpecialValueType)GetProcAddress(m_hDLL, "ASIFI_GetFileSpecialValue");
	if(fASIFI_GetFileSpecialValue == NULL)
	{
		return NULL;
	}

	memset(pcFileValue, 0, CHAR_MAX_SIZE);

	if(fASIFI_GetFileSpecialValue(pcFileName, pcFileKey, pcFileValue, CHAR_MAX_SIZE) != 0)
	{
		return NULL;
	}	

	return pcFileValue;
}
//--------------------------------------------------------------------

INT32	CASIFIDLLUtil::ASIFI_GetFileLanguage(char* pcFileName)
{
	INT32 nType = 0;
	INT32 nLanguageType = LOCATION_NATIONAL_TYPE_KOR;
	ASIFI_GetFileLanguageType	fASIFI_GetFileLanguage = NULL;

	if(m_hDLL == NULL || pcFileName == NULL)
		return nLanguageType;

	fASIFI_GetFileLanguage = (ASIFI_GetFileLanguageType)GetProcAddress(m_hDLL, "ASIFI_GetFileLanguage");
	if(fASIFI_GetFileLanguage == NULL)
	{
		return nLanguageType;
	}

	if(fASIFI_GetFileLanguage(pcFileName, nType) != 0)
	{
		return nLanguageType;
	}

	switch(nType)
	{
		case ASI_LOCALE_TYPE_ENGLISH:				nLanguageType = LOCATION_NATIONAL_TYPE_US; break;
		case ASI_LOCALE_TYPE_KOREA:					nLanguageType = LOCATION_NATIONAL_TYPE_KOR; break;
		case ASI_LOCALE_TYPE_JAPAN:					nLanguageType = LOCATION_NATIONAL_TYPE_JAPAN; break;
		case ASI_LOCALE_TYPE_CHINA_SIMPLIFIED:		nLanguageType = LOCATION_NATIONAL_TYPE_CHINA_SIMPLIFIED; break;
		case ASI_LOCALE_TYPE_CHINA_TRADITIONAL:		nLanguageType = LOCATION_NATIONAL_TYPE_CHINA_TRADITIONAL; break;
		default: nLanguageType = LOCATION_NATIONAL_TYPE_KOR; break;
	}
	return nLanguageType;
}
//--------------------------------------------------------------------

INT32		CASIFIDLLUtil::ASIFI_GetFileElfMagic(char* pcFileName)
{
	INT32 nElfMagic = ASI_FI_ELF_MACHINE_NONE;
	ASIFI_GetFileElfMagicType fASIFI_GetFileElfMagic = NULL;
	if(m_hDLL == NULL || pcFileName == NULL)
		return ASI_FI_ELF_MACHINE_NONE;
	
	fASIFI_GetFileElfMagic = (ASIFI_GetFileElfMagicType)GetProcAddress(m_hDLL, "ASIFI_GetFileElfMagic");
	if(fASIFI_GetFileElfMagic == NULL)
	{
		return ASI_FI_ELF_MACHINE_NONE;
	}

	if(fASIFI_GetFileElfMagic(pcFileName, nElfMagic) != 0)
	{
		return ASI_FI_ELF_MACHINE_NONE;
	}

	return nElfMagic;
}
//--------------------------------------------------------------------

INT32		CASIFIDLLUtil::ASIFI_GetFileElfType(char *pcFilePath, char *pcFileName)
{
	INT32 nElfType = ASI_FI_ELF_TYPE_NONE;
	ASIFI_GetFileElfTypeType	fASIFI_GetFileElfType = NULL;
	
	if(m_hDLL == NULL || pcFilePath == NULL)
		return ASI_FI_ELF_TYPE_NONE;
	
	fASIFI_GetFileElfType = (ASIFI_GetFileElfTypeType)GetProcAddress(m_hDLL, "ASIFI_GetFileElfType");
	if(fASIFI_GetFileElfType == NULL)
	{
		return ASI_FI_ELF_TYPE_NONE;
	}

	if(fASIFI_GetFileElfType(pcFilePath, nElfType, pcFileName) != 0)
	{
		return ASI_FI_ELF_TYPE_NONE;
	}
	return nElfType;
}

