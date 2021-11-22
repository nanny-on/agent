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

#ifndef _WHITE_LIST_DEF__
#define _WHITE_LIST_DEF__

#define WDB_FILE_NAME "as_patten"

#define WILDCARD_LENGTH		32
#define PATH_LENGTH			512
#define HASH_PADDING_LEN	8
#define SEC_DATA_LENGTH		4096

//db name | category | create time | write time | major version | minor version
#define FMT_FILE_HEADER "%s%c%d%c%d%c%d%c%d%c%d\n"

//hash | path | tag | product | company | desc | sign company | category | size | magic | response | role | create time | access time | write time | version high | version low
#define FMT_WHITE_LIST		"%s%c%s%c%s%c%s%c%s%c%s%c%s%c%u%c%u%c%u%c%u%c%u%c%u%c%u%c%u%c%u%c%u\n"
#define FMT_WHITE_LIST_HDR	"%u%c%s"

typedef struct _file_info
{
	DWORD dwFileSize;
	DWORD dwCreateTime;
	DWORD dwAccessTime;
	DWORD dwWriteTime;
	char acFileName[MAX_QQBUFF];
}FILE_INFO, *PFILE_INFO;

typedef struct _elf_header
{
	DWORD dwMachine;
	DWORD dwEpAddr;
	DWORD dwPhOffset;
	DWORD dwPhNum;
	DWORD dwShOffset;
	DWORD dwShNum;
}ELF_HEADER, *PELF_HEADER;

typedef struct _text_section
{
	DWORD dwOffset;
	DWORD dwAddr;
	DWORD dwSize;
	DWORD dwType;
	char acData[SEC_DATA_LENGTH];
}TEXT_SECTION, *PTEXT_SECTION;

typedef struct _elf_data
{
	ELF_HEADER stElfHeader;
	TEXT_SECTION stTextSecion;
}ELF_DATA, *PELF_DATA;

#endif /* _WHITE_LIST_DEF__ */