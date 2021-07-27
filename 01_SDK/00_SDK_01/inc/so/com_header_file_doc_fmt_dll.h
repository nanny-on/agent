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

#ifndef COM_HEADER_DFILE_FMT_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_DFILE_FMT_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//-------------------------------------------------------------------------------------

#define ASIDFF_FILE_FMT_TYPE_UNKNOW		0x00000000
#define	ASIDFF_FILE_FMT_TYPE_PPT		0x00000001
#define ASIDFF_FILE_FMT_TYPE_PPTX		0x00000002
#define	ASIDFF_FILE_FMT_TYPE_PPS		0x00000003
#define	ASIDFF_FILE_FMT_TYPE_PPSX		0x00000004
#define	ASIDFF_FILE_FMT_TYPE_XLS		0x00000005
#define	ASIDFF_FILE_FMT_TYPE_XLSX		0x00000006
#define	ASIDFF_FILE_FMT_TYPE_DOC		0x00000007
#define	ASIDFF_FILE_FMT_TYPE_DOCX		0x00000008

#define	ASIDFF_FILE_FMT_TYPE_PDF		0x00000100
#define	ASIDFF_FILE_FMT_TYPE_HWP		0x00000200
#define	ASIDFF_FILE_FMT_TYPE_HPT		0x00000201

#define	ASIDFF_FILE_FMT_TYPE_HML		0x00000300

#define	ASIDFF_FILE_FMT_TYPE_ODT		0x00000400
#define	ASIDFF_FILE_FMT_TYPE_ODP		0x00000401
#define	ASIDFF_FILE_FMT_TYPE_ODS		0x00000402
#define	ASIDFF_FILE_FMT_TYPE_ODB		0x00000403
#define	ASIDFF_FILE_FMT_TYPE_ODG 		0x00000404

#define	ASIDFF_FILE_FMT_TYPE_SHOW		0x00000500
#define	ASIDFF_FILE_FMT_TYPE_CELL		0x00000501
#define	ASIDFF_FILE_FMT_TYPE_NXL		0x00000502
#define	ASIDFF_FILE_FMT_TYPE_NXT		0x00000503

//-------------------------------------------------------------------------------------

typedef struct _asi_dfile_fmt_init
{
	_asi_dfile_fmt_init()
	{
		ZeroMemoryExt(szLogPath);
		ZeroMemoryExt(szLogFile);
		nRemainLog				= 1;
	}
	CHAR				szLogPath[CHAR_MAX_SIZE];
	CHAR				szLogFile[CHAR_MAX_SIZE];
	UINT32				nRemainLog;
}ASI_DFILE_FMT_INIT, *PASI_DFILE_FMT_INIT;

//-------------------------------------------------------------------------------------

typedef struct _asi_dfile_fmt_info
{
	_asi_dfile_fmt_info()
	{
		nFmtType		= 0;
		ZeroMemoryExt(szFileName);
		ZeroMemoryExt(szFmtType);
		ZeroMemory(wszFileName, sizeof(WCHAR) * CHAR_MAX_SIZE);
	}
	CHAR 			szFileName[CHAR_MAX_SIZE];
	WCHAR			wszFileName[CHAR_MAX_SIZE];
	INT32			nFmtType;
	CHAR			szFmtType[CHAR_MAX_SIZE];

}ASI_DFILE_FMT_INFO, *PASI_DFILE_FMT_INFO;
//-------------------------------------------------------------------------------------

typedef	INT32	(*ASIDFF_SetDFFmtInitType)(PASI_DFILE_FMT_INIT pADFFI);
typedef	INT32	(*ASIDFF_GetDFFmtInfoType)(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);

//-------------------------------------------------------------------------------------

#endif //COM_HEADER_DOCUFILEINFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C