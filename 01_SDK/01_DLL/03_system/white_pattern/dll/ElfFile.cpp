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
#include "ElfFile.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//						CPE_DosHeader
////////////////////////////////////////////////////////////////////////////////////////////////
CElfFileInfo::CElfFileInfo()
{

}

CElfFileInfo::~CElfFileInfo()
{

}

BOOL CElfFileInfo::IsELF(Elf32_Ehdr stElfHdr)
{
	/* ELF magic : 0x7f,'E','L','F' */
	if(!strncmp((char*)stElfHdr.e_ident, "\177ELF", 4))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CElfFileInfo::Is64Bit(Elf32_Ehdr stElfHdr)
{
	if (stElfHdr.e_ident[EI_CLASS] == ELFCLASS64)
	{
		return TRUE;
	}
	return FALSE;
}


INT32 CElfFileInfo::IsScriptsFile(INT32 fd, BOOL &bIsScripts, char *acLogMsg)
{
	off_t dwOffset = 0;
	char acData[MAX_QBUFF] = {0,};
	INT32 i = 0, nIndex = 0;
	if(fd < 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[IsScriptsFile] invalid input data");
		return -1;
	}
	if(lseek(fd, dwOffset, SEEK_SET) == -1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[IsScriptsFile] fail to seek %ld %d", dwOffset, errno);
		return -2;
	}
	if(read(fd, acData, MAX_QBUFF-1) == -1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[IsScriptsFile] fail to read %d", errno);
		return -3;
	}
	acData[MAX_QBUFF-1] = 0;

	if(acData[0] == 0)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[IsScriptsFile] invalid read data");
		return -4;
	}

	for(i=0; i<20; i++)
	{
		if ((acData[i] == '\n') || (acData[i-1] == '\r'))
			continue;
		nIndex = i;
		break;
	}

	if(_strnicmp(&acData[nIndex], "#!", 2))
	{
		bIsScripts = FALSE;
		return 0;
	}

	for(i=nIndex+2; i<nIndex+20; i++)
	{
		if (acData[i] != '/')
			continue;
		if(!_strnicmp(&acData[i], "/bin", 4))
		{
			bIsScripts = TRUE;
			return 0;
		}
	}

	bIsScripts = FALSE;
	return 0;
}


INT32 CElfFileInfo::ReadElfHeader(INT32 fd, Elf32_Ehdr *pElfHdr, char *acLogMsg)
{
	off_t dwOffset = 0;
	size_t dwSize = (size_t)sizeof(Elf32_Ehdr);
	if(fd < 0 || pElfHdr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader] invalid input data");
		return -1;
	}
	if(lseek(fd, dwOffset, SEEK_SET) == -1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader] fail to seek %ld %d", dwOffset, errno);
		return -2;
	}
	if(read(fd, (char *)pElfHdr, dwSize) == -1)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader] fail to read rz:%ld hz:%ld %d", dwSize, sizeof(Elf32_Ehdr), errno);
		return -3;
	}
	return 0;
}

INT32 CElfFileInfo::ReadElfHeader64(INT32 fd, Elf64_Ehdr *pElfHdr64, char *acLogMsg)
{
	off_t dwOffset = 0;
	size_t dwSize = sizeof(Elf64_Ehdr);
	if(fd < 0 || pElfHdr64 == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader64] invalid input data");
		return -1;
	}
	if(lseek(fd, dwOffset, SEEK_SET) != dwOffset)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader64] fail to seek %ld %d", dwOffset, errno);
		return -2;
	}
	if(read(fd, (void *)pElfHdr64, dwSize) != dwSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadElfHeader64] fail to read %ld %d", dwSize, errno);
		return -3;
	}
	return 0;
}


INT32 CElfFileInfo::ReadSectionHeaderTable64(INT32 fd, Elf64_Ehdr stElfHdr64, Elf64_Shdr* pShTable, char *acLogMsg)
{
	UINT32 i = 0;
	if(fd < 0 || pShTable == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable64] invalid input data");
		return -1;
	}
	if(lseek(fd, (off_t)stElfHdr64.e_shoff, SEEK_SET) != (off_t)stElfHdr64.e_shoff)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable64] fail to seek off:%d %d", stElfHdr64.e_shoff, errno);
		return -2;
	}

	for(i=0; i<stElfHdr64.e_shnum; i++)
	{
		if(read(fd, (void *)&pShTable[i], stElfHdr64.e_shentsize) != stElfHdr64.e_shentsize)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable64] fail to read idx:%lu/%lu sz:%d %d", i, stElfHdr64.e_shnum, stElfHdr64.e_shentsize, errno);
			return -3;
		}
	}
	return 0;
}

INT32 CElfFileInfo::ReadSection64(INT32 fd, Elf64_Shdr stShTable, char **ppShStr, char *acLogMsg)
{
	char* pBuff = NULL;
	off_t dwOffset = 0;
	size_t dwSize = 0;

	dwOffset = (off_t)stShTable.sh_offset;
	dwSize = (off_t)stShTable.sh_size;

	if(fd < 0 || dwOffset < 1 || dwSize < 1 || ppShStr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection64] invalid input data off:%ld sz:%ld", dwOffset, dwSize);
		return -1;
	}

	pBuff = (char*)malloc(dwSize+1);
	if(pBuff == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection64] fail to allocate memory sz:%d %d", dwSize, errno);
		return -2;
	}
	memset(pBuff, 0, dwSize+1);

	if(lseek(fd, dwOffset, SEEK_SET) != dwOffset)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection64] fail to seek off:%d %d", dwOffset, errno);
		safe_free(pBuff);
		return -3;
	}

	if(read(fd, (void *)pBuff, dwSize) != dwSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection64] fail to read sz:%d %d", dwSize, errno);
		safe_free(pBuff);
		return -4;
	}
	*ppShStr = pBuff;
	return 0;
}


INT32 CElfFileInfo::ReadTextSection64(INT32 fd, Elf64_Ehdr stElfHdr64, Elf64_Shdr* pShTable, PELF_DATA pElfData, char *acLogMsg)
{
	UINT32 i = 0;;
	char* pShStr = NULL;
	off_t dwOffset = 0;
	size_t dwSize = 0;
	INT32 nRetVal = 0;
	char acSection[MAX_TYPE_LEN] = {0,};

	if(fd < 0 || pShTable == NULL || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection64] invalid input data");
		return -1;
	}

	/* Read section-header string-table */
	nRetVal = ReadSection64(fd, pShTable[stElfHdr64.e_shstrndx], &pShStr, acLogMsg);
	if(nRetVal < 0 || pShStr == NULL)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	for(i=0; i<stElfHdr64.e_shnum; i++)
	{
		strncpy(acSection, (char *)(pShStr + pShTable[i].sh_name), MAX_TYPE_LEN-1);
		acSection[MAX_TYPE_LEN-1] = 0;
		if(!strcmp(".text", acSection))
		{
			pElfData->stTextSecion.dwOffset = (DWORD)pShTable[i].sh_offset;
			pElfData->stTextSecion.dwSize = (DWORD)pShTable[i].sh_size;
			pElfData->stTextSecion.dwAddr = (DWORD)pShTable[i].sh_addr;
			pElfData->stTextSecion.dwType = (DWORD)pShTable[i].sh_type;

			dwOffset = (off_t)pShTable[i].sh_offset;
			dwSize = (size_t)pShTable[i].sh_size;
			if(dwSize > SEC_DATA_LENGTH)
				dwSize = SEC_DATA_LENGTH;
			break;
		}
	}

	if(dwOffset == 0 || dwSize < 1 || dwSize > SEC_DATA_LENGTH)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection64] fail to find text section %ld %ld", dwOffset, dwSize);
		safe_free(pShStr);
		return -2;
	}

	if(lseek(fd, dwOffset, SEEK_SET) != dwOffset)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection64] fail to seek off:ld %d", dwOffset, errno);
		safe_free(pShStr);
		return -3;
	}
	

	if(read(fd, (void *)pElfData->stTextSecion.acData, dwSize) != dwSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection64] fail to read text section sz:%ld %d", dwSize, errno);
		safe_free(pShStr);
		return -4;
	}

	safe_free(pShStr);
	return 0;
}


INT32 CElfFileInfo::AnalysisElf64(INT32 fd, PELF_DATA pElfData, char *acLogMsg)
{
	INT32 nRetVal = 0;
	Elf64_Ehdr stElfHdr64;
	Elf64_Shdr* pShTable = NULL;
	if(fd < 0 || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf64] invalid input data");
		return -1;
	}
	do{
		memset(&stElfHdr64, 0, sizeof(Elf64_Ehdr));
		nRetVal = ReadElfHeader64(fd, &stElfHdr64, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 10;
			break;
		}

		if(stElfHdr64.e_shentsize < 1 || stElfHdr64.e_shnum < 1)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf64] invalid hdr value sz:%d num:%d", stElfHdr64.e_shentsize, stElfHdr64.e_shnum);
			nRetVal = -2;
			break;
		}
		pShTable = (Elf64_Shdr*)malloc(stElfHdr64.e_shentsize * stElfHdr64.e_shnum);
		if(pShTable == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf64] fail to allocate memory sz:%d num:%d %d", stElfHdr64.e_shentsize, stElfHdr64.e_shnum, errno);
			nRetVal = -3;
			break;
		}
		memset(pShTable, 0, stElfHdr64.e_shentsize * stElfHdr64.e_shnum);
		nRetVal = ReadSectionHeaderTable64(fd, stElfHdr64, pShTable, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}
		pElfData->stElfHeader.dwMachine = (DWORD)stElfHdr64.e_machine;
		pElfData->stElfHeader.dwEpAddr = (DWORD)stElfHdr64.e_entry;
		pElfData->stElfHeader.dwPhOffset = (DWORD)stElfHdr64.e_phoff;
		pElfData->stElfHeader.dwPhNum = (DWORD)stElfHdr64.e_phnum;
		pElfData->stElfHeader.dwShOffset = (DWORD)stElfHdr64.e_shoff;
		pElfData->stElfHeader.dwShNum = (DWORD)stElfHdr64.e_shnum;
		nRetVal = ReadTextSection64(fd, stElfHdr64, pShTable, pElfData, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 30;
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	safe_free(pShTable);

	return nRetVal;
}

INT32 CElfFileInfo::ReadSectionHeaderTable32(INT32 fd, Elf32_Ehdr stElfHdr, Elf32_Shdr* pShTable, char *acLogMsg)
{
	UINT32 i = 0;
	if(fd < 0 || pShTable == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable32] invalid input data");
		return -1;
	}

	if(lseek(fd, (off_t)stElfHdr.e_shoff, SEEK_SET) != (off_t)stElfHdr.e_shoff)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable32] fail to seek off:%d %d", stElfHdr.e_shoff, errno);
		return -2;
	}

	for(i=0; i<stElfHdr.e_shnum; i++)
	{
		if(read(fd, (void *)&pShTable[i], stElfHdr.e_shentsize) != stElfHdr.e_shentsize)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[ReadSectionHeaderTable32] fail to read idx:%lu/%lu sz:%d %d", i, stElfHdr.e_shnum, stElfHdr.e_shentsize, errno);
			return -3;
		}
	}
	return 0;
}

INT32 CElfFileInfo::ReadSection32(INT32 fd, Elf32_Shdr stShTable, char **ppShStr, char *acLogMsg)
{
	char* pBuff = NULL;
	off_t dwOffset = 0;
	size_t dwSize = 0;

	dwOffset = (off_t)stShTable.sh_offset;
	dwSize = (off_t)stShTable.sh_size;

	if(fd < 0 || dwOffset < 1 || dwSize < 1 || ppShStr == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection32] invalid input data off:%ld sz:%ld", dwOffset, dwSize);
		return -1;
	}

	pBuff = (char*)malloc(dwSize+1);
	if(pBuff == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection32] fail to allocate memory sz:%d %d", dwSize, errno);
		return -2;
	}
	memset(pBuff, 0, dwSize+1);

	if(lseek(fd, dwOffset, SEEK_SET) != dwOffset)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection32] fail to seek off:%d %d", dwOffset, errno);
		safe_free(pBuff);
		return -3;
	}

	if(read(fd, (void *)pBuff, dwSize) == dwSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadSection32] fail to read sz:%d %d", dwSize, errno);
		safe_free(pBuff);
		return -4;
	}
	*ppShStr = pBuff;
	return 0;
}


INT32 CElfFileInfo::ReadTextSection32(INT32 fd, Elf32_Ehdr stElfHdr, Elf32_Shdr* pShTable, PELF_DATA pElfData, char *acLogMsg)
{
	UINT32 i = 0;;
	char* pShStr = NULL;
	off_t dwOffset = 0;
	size_t dwSize = 0;
	INT32 nRetVal = 0;
	char acSection[MAX_TYPE_LEN] = {0,};

	if(fd < 0 || pShTable == NULL || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection32] invalid input data");
		return -1;
	}

	/* Read section-header string-table */
	nRetVal = ReadSection32(fd, pShTable[stElfHdr.e_shstrndx], &pShStr, acLogMsg);
	if(nRetVal < 0 || pShStr == NULL)
	{
		nRetVal -= 10;
		return nRetVal;
	}

	for(i=0; i<stElfHdr.e_shnum; i++)
	{
		strncpy(acSection, (char *)(pShStr + pShTable[i].sh_name), MAX_TYPE_LEN-1);
		acSection[MAX_TYPE_LEN-1] = 0;
		if(!strcmp(".text", acSection))
		{
			pElfData->stTextSecion.dwOffset = (DWORD)pShTable[i].sh_offset;
			pElfData->stTextSecion.dwSize = (DWORD)pShTable[i].sh_size;
			pElfData->stTextSecion.dwAddr = (DWORD)pShTable[i].sh_addr;
			pElfData->stTextSecion.dwType = (DWORD)pShTable[i].sh_type;

			dwOffset = (off_t)pShTable[i].sh_offset;
			dwSize = (size_t)pShTable[i].sh_size;
			if(dwSize > SEC_DATA_LENGTH)
				dwSize = SEC_DATA_LENGTH;
			break;
		}
	}

	if(dwOffset == 0 || dwSize < 1 || dwSize > SEC_DATA_LENGTH)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection32] fail to find text section %ld %ld", dwOffset, dwSize);
		safe_free(pShStr);
		return -2;
	}

	if(lseek(fd, dwOffset, SEEK_SET) != dwOffset)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection32] fail to seek off:ld %d", dwOffset, errno);
		safe_free(pShStr);
		return -3;
	}


	if(read(fd, (void *)pElfData->stTextSecion.acData, dwSize) != dwSize)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[ReadTextSection32] fail to read text section sz:%ld %d", dwSize, errno);
		safe_free(pShStr);
		return -4;
	}

	safe_free(pShStr);

	return 0;
}




INT32 CElfFileInfo::AnalysisElf32(INT32 fd, Elf32_Ehdr stElfHdr, PELF_DATA pElfData, char *acLogMsg)
{
	INT32 nRetVal = 0;
	Elf32_Shdr* pShTable = NULL;
	if(fd < 0 || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf32] invalid input data");
		return -1;
	}
	do{
		if(stElfHdr.e_shentsize < 1 || stElfHdr.e_shnum < 1)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf32] invalid hdr value sz:%d num:%d", stElfHdr.e_shentsize, stElfHdr.e_shnum);
			nRetVal = -2;
			break;
		}
		pShTable = (Elf32_Shdr*)malloc(stElfHdr.e_shentsize * stElfHdr.e_shnum);
		if(pShTable == NULL)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElf32] fail to allocate memory sz:%d num:%d %d", stElfHdr.e_shentsize, stElfHdr.e_shnum, errno);
			nRetVal = -3;
			break;
		}
		memset(pShTable, 0, stElfHdr.e_shentsize * stElfHdr.e_shnum);
		nRetVal = ReadSectionHeaderTable32(fd, stElfHdr, pShTable, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 20;
			break;
		}

		pElfData->stElfHeader.dwMachine = (DWORD)stElfHdr.e_machine;
		pElfData->stElfHeader.dwEpAddr = (DWORD)stElfHdr.e_entry;
		pElfData->stElfHeader.dwPhOffset = (DWORD)stElfHdr.e_phoff;
		pElfData->stElfHeader.dwPhNum = (DWORD)stElfHdr.e_phnum;
		pElfData->stElfHeader.dwShOffset = (DWORD)stElfHdr.e_shoff;
		pElfData->stElfHeader.dwShNum = (DWORD)stElfHdr.e_shnum;

		nRetVal = ReadTextSection32(fd, stElfHdr, pShTable, pElfData, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 30;
			break;
		}
		nRetVal = 0;
	}while(FALSE);

	safe_free(pShTable);

	return nRetVal;
}


INT32 CElfFileInfo::AnalysisElfFile(CHAR *pPath, PELF_DATA pElfData, char *acLogMsg)
{
	INT32 nRetVal = 0;
	INT32 fd = -1;
	Elf32_Ehdr stElfHdr;
	BOOL bIsScriptsFile = FALSE;
	if(pPath == NULL || pPath[0] == 0 || pElfData == NULL)
	{
		if(acLogMsg)
			snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElfFile] invalid input data");
		return -1;
	}
	do{
		memset(pElfData, 0, sizeof(ELF_DATA));
		fd = open(pPath, O_RDONLY|O_SYNC);
		if(fd<0)
		{
			Sleep(RETRYDELAY);
			fd = open(pPath, O_RDONLY|O_SYNC);
			if(fd<0)
			{
				if(acLogMsg)
					snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElfFile] fail to open %s %d", pPath, errno);
				nRetVal = -2;
				break;
			}
		}

		nRetVal = IsScriptsFile(fd, bIsScriptsFile, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 100;
			break;
		}

		if(bIsScriptsFile == TRUE)
		{
			nRetVal = 1;
			break;
		}

		memset(&stElfHdr, 0, sizeof(Elf32_Ehdr));
		nRetVal = ReadElfHeader(fd, &stElfHdr, acLogMsg);
		if(nRetVal < 0)
		{
			nRetVal -= 200;
			break;
		}

		if(IsELF(stElfHdr) == FALSE)
		{
			if(acLogMsg)
				snprintf(acLogMsg, MAX_LOGMSG, "[AnalysisElfFile] is not elf file");
			nRetVal = -3;
			break;
		}

		if(Is64Bit(stElfHdr) == TRUE)
		{
			nRetVal = AnalysisElf64(fd, pElfData, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 300;
				break;
			}
		}
		else
		{
			nRetVal = AnalysisElf32(fd, stElfHdr, pElfData, acLogMsg);
			if(nRetVal < 0)
			{
				nRetVal -= 400;
				break;
			}
		}
		nRetVal = 0;
	}while(FALSE);

	if(fd >= 0)
		close(fd);

	return nRetVal;
}

