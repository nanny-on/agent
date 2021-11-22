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

#ifndef _ELF_FILE_H__

#define _ELF_FILE_H__

#include <elf.h>
#include "WhiteListDef.h"

#define RETRYDELAY  150

///////////////////////////////////////////////////////////////////////////////////////////////
class CElfFileInfo
{
private:
	BOOL IsELF(Elf32_Ehdr stElfHdr);
	BOOL Is64Bit(Elf32_Ehdr stElfHdr);

	INT32 IsScriptsFile(INT32 fd, BOOL &bIsScripts, char *acLogMsg = NULL);
	INT32 ReadElfHeader(INT32 fd, Elf32_Ehdr *pElfHdr, char *acLogMsg = NULL);
	INT32 ReadSection32(INT32 fd, Elf32_Shdr stShTable, char **ppShStr, char *acLogMsg = NULL);
	INT32 ReadTextSection32(INT32 fd, Elf32_Ehdr stElfHdr, Elf32_Shdr* pShTable, PELF_DATA pElfData, char *acLogMsg = NULL);
	INT32 ReadSectionHeaderTable32(INT32 fd, Elf32_Ehdr stElfHdr, Elf32_Shdr* pShTable, char *acLogMsg = NULL);
	INT32 AnalysisElf32(INT32 fd, Elf32_Ehdr stElfHdr, PELF_DATA pElfData, char *acLogMsg = NULL);

	INT32 ReadElfHeader64(INT32 fd, Elf64_Ehdr *pElfHdr64, char *acLogMsg = NULL);
	INT32 ReadSection64(INT32 fd, Elf64_Shdr stShTable, char **ppShStr, char *acLogMsg = NULL);
	INT32 ReadTextSection64(INT32 fd, Elf64_Ehdr stElfHdr64, Elf64_Shdr* pShTable, PELF_DATA pElfData, char *acLogMsg = NULL);
	INT32 ReadSectionHeaderTable64(INT32 fd, Elf64_Ehdr stElfHdr64, Elf64_Shdr* pShTable, char *acLogMsg = NULL);
	INT32 AnalysisElf64(INT32 fd, PELF_DATA pElfData, char *acLogMsg = NULL);

public:
	CElfFileInfo();
	virtual ~CElfFileInfo();
	INT32 AnalysisElfFile(CHAR *pPath, PELF_DATA pElfData, char *acLogMsg = NULL);

};

#endif /*_ELF_FILE_H__*/