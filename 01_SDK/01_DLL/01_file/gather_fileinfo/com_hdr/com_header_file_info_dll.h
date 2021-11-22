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


#ifndef COM_HEADER_FILE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_FILE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C


//////////////////////////////////////////////////////////////////////////

#define ASI_LOCALE_TYPE_ENGLISH				1
#define ASI_LOCALE_TYPE_KOREA				2
#define	ASI_LOCALE_TYPE_JAPAN				3
#define ASI_LOCALE_TYPE_CHINA_SIMPLIFIED	4
#define ASI_LOCALE_TYPE_CHINA_TRADITIONAL	5
#define ASI_LOCALE_TYPE_ETC					100
//--------------------------------------------------------------------------------------

#define	ASI_FI_ELF_TYPE_NONE				0x00000000
#define ASI_FI_ELF_TYPE_EXE					0x00000001
#define ASI_FI_ELF_TYPE_SO					0x00000002
#define ASI_FI_ELF_TYPE_DRV					0x00000004

#define	ASI_FI_ELF_MACHINE_NONE				0x00000000
#define ASI_FI_ELF_MACHINE_386				0x00000001
#define ASI_FI_ELF_MACHINE_X86_64			0x00000002
#define ASI_FI_ELF_MACHINE_AARCH64			0x00000004

#define	ASI_FI_ELF_CLASS_NONE				0x00000000
#define ASI_FI_ELF_CLASS_32BIT				0x00000001
#define ASI_FI_ELF_CLASS_64BIT				0x00000002

//--------------------------------------------------------------------------------------

typedef INT32 (*ASIFI_GetFileDescriptionType)(char *pcFileName, char *pcFileDescr, INT32 nLength);
typedef INT32 (*ASIFI_GetFileVersionType)(char *pcFileName, char *pcFileVersion, INT32 nLength);
typedef INT32 (*ASIFI_GetFileSpecialValueType)(char *pcFileName, char *pcFileKey, char *pcFileValue, INT32 nLength);
typedef INT32 (*ASIFI_GetFileLanguageType)(char *pcFileName, INT32& nLanguageType);
typedef INT32 (*ASIFI_GetFileElfMagicType)(char *pcFileName, INT32& nPEMagic);
typedef INT32 (*ASIFI_GetFileElfTypeType)(char *pcFilePath, INT32& nPEType, char *pcFileName = NULL);

//--------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////

#endif //COM_HEADER_FILE_INFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
