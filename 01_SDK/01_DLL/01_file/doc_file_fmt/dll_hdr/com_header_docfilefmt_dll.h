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

#ifndef COM_HEADER_DFILE_FMT_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C
#define COM_HEADER_DFILE_FMT_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C

//-------------------------------------------------------------------------------------

#define ASIDFF_FILE_FMT_TYPE_UNKNOWN	0x00000000
#define	ASIDFF_FILE_FMT_TYPE_PPT		0x00000001
#define ASIDFF_FILE_FMT_TYPE_PPTX		0x00000002
#define	ASIDFF_FILE_FMT_TYPE_PPS		0x00000003
#define	ASIDFF_FILE_FMT_TYPE_PPSX		0x00000004
#define	ASIDFF_FILE_FMT_TYPE_XLS		0x00000005
#define	ASIDFF_FILE_FMT_TYPE_XLSX		0x00000006
#define	ASIDFF_FILE_FMT_TYPE_DOC		0x00000007
#define	ASIDFF_FILE_FMT_TYPE_DOCX		0x00000008

#define	ASIDFF_FILE_FMT_TYPE_ZIP		0x00000050

#define	ASIDFF_FILE_FMT_MORE_CHECK		0x00000070

#define	ASIDFF_FILE_FMT_TYPE_PDF		0x00000100
#define	ASIDFF_FILE_FMT_TYPE_HWP		0x00000200
#define	ASIDFF_FILE_FMT_TYPE_HPT		0x00000201
#define	ASIDFF_FILE_FMT_TYPE_HWPX		0x00000202

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

#define ASIDFF_LOCAL_HEADER_MAGIC		0x04034b50

//-------------------------------------------------------------------------------------

typedef struct _asi_dfile_fmt_init
{
	_asi_dfile_fmt_init()
	{
		ZeroMemoryExt(szLogPath);
		ZeroMemoryExt(szLogFile);
		nRemainLog				= 1;
		nFileLogRetention		= 5;
	}
	CHAR				szLogPath[CHAR_MAX_SIZE];
	CHAR				szLogFile[CHAR_MAX_SIZE];
	UINT32				nRemainLog;
	UINT32				nFileLogRetention;
}ASI_DFILE_FMT_INIT, *PASI_DFILE_FMT_INIT;

//-------------------------------------------------------------------------------------

typedef struct _asi_dfile_fmt_info
{
	_asi_dfile_fmt_info()
	{
		nFmtType		= 0;
		memset(szFileName, 0, CHAR_MAX_SIZE);
		memset(szFmtType, 0, MAX_TYPE_LEN);
	}
	INT32			nFmtType;
	CHAR 			szFileName[CHAR_MAX_SIZE];
	CHAR			szFmtType[MAX_TYPE_LEN];

}ASI_DFILE_FMT_INFO, *PASI_DFILE_FMT_INFO;


#pragma pack(2)

typedef struct _zip_date
{
	UINT16 wMday:5;
	UINT16 wMon:4;
	UINT16 wYear:7;
}ZIP_DATE, *PZIP_DATE;

typedef struct _zip_time
{
	UINT16 wSec:5;
	UINT16 wMin:6;
	UINT16 wHour:5;
}ZIP_TIME, *PZIP_TIME;

typedef struct _zip_file_header
{
	UINT16 wYersionNeeded;
	UINT16 wFlag;
	UINT16 wCompressionMethod;
	ZIP_TIME stLastModTime;
	ZIP_DATE stLastModDate;
	UINT32 dwCRC;
	UINT32 dwCompressedSize;
	UINT32 dwUncompressedSize;
	UINT16 wNameSize;
	UINT16 wExtraSize;
}ZIP_FILE_HEADER, *PZIP_FILE_HEADER;

#pragma pack()

//-------------------------------------------------------------------------------------

typedef	INT32	(*ASIDFF_SetDFFmtInitType)(PASI_DFILE_FMT_INIT pADFFI);
typedef	INT32	(*ASIDFF_GetDFFmtInfoType)(PASI_DFILE_FMT_INFO pADFFI, char *acLogMsg);

//-------------------------------------------------------------------------------------

#endif //COM_HEADER_DOCUFILEINFO_DLL_H_152BCED9_0D66_4F66_8A87_BF47B4A5386C