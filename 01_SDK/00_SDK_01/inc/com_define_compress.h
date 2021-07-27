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

#ifndef COM_DEFINE_COMPRESS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467
#define COM_DEFINE_COMPRESS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467

#pragma pack(1)
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


#define		ASI_COMP_VER_MAJOR		0x0001
#define		ASI_COMP_VER_MINOR		0x0001

typedef struct _asi_comp_file_ver
{
	UINT16	nMajorVer;
	UINT16	nMinorVer;
	UINT32	nCreateDateTime;
	UINT32	nFileNum;
}ASI_COMP_FILE_VER, *PASI_COMP_FILE_VER;
//-----------------------------------------------------------------------------------

typedef struct _asi_comp_file_hdr
{
	_asi_comp_file_hdr()
	{
		nOrFileSize		= 0;
		nComFileSize	= 0;
	}
	String		strSubPath;
	String		strFileName;
	StringW		strSubPathW;
	StringW		strFileNameW;
	UINT32	nOrFileSize;
	UINT32	nComFileSize;
}ASI_COMP_FILE_HDR, *PASI_COMP_FILE_HDR;
//-----------------------------------------------------------------------------------

typedef struct _asi_comp_file_body
{
	_asi_comp_file_body()
	{
		data = 0;
	}
	~_asi_comp_file_body()
	{
		if(data)
		{
			free(data);
			data = 0;
		}
	}
	void _set_data(PVOID inData, INT32 nLen)
	{
		if(data) free(data);	

		data = malloc(nLen + 1);
		if(data)
		{
			memset(data, 0, nLen+1);
			memcpy(data, inData, nLen);
		}
		return;
	}
	UINT32	nFilePos;
	PVOID	data;
}ASI_COMP_FILE_BODY, *PASI_COMP_FILE_BODY;
//-----------------------------------------------------------------------------------

typedef struct _asi_comp_file_info
{
	ASI_COMP_FILE_HDR		header;
	ASI_COMP_FILE_BODY		body;
}ASI_COMP_FILE_INFO, *PASI_COMP_FILE_INFO;

typedef list<PASI_COMP_FILE_INFO>			TListASICompFileInfo;
typedef TListASICompFileInfo::iterator		TListASICompFileInfoItor;
//-----------------------------------------------------------------------------------

typedef struct _asi_comp_token_hdr
{
	_asi_comp_token_hdr()
	{
		memset(szRelativePath, 0, MAX_PATH);
		memset(szFileName, 0, MAX_PATH);
		nSrcFileSize		= 0;
		nTokenSize			= 0;
		nLoofCnt			= 0;
	}
	CHAR	szRelativePath[MAX_PATH];
	CHAR	szFileName[MAX_PATH];
	UINT64	nSrcFileSize;
	UINT32	nTokenSize;
	UINT64	nLoofCnt;
}ASI_COMP_TOKEN_HDR, *PASI_COMP_TOKEN_HDR;
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

#define		ASI_COMP_SECU_TOKEN_SIZE		(1024*1024*10)

typedef struct _asi_comp_secu_file_hdr
{
	_asi_comp_secu_file_hdr()
	{
		nCompVer		= 0;
		nEncType		= 0;
		nFileSize		= 0;
		nFileMode		= 0;
		nTokenSize		= 0;
	}
	UINT32		nCompVer;
	UINT32		nEncType;
	UINT32		nFileMode;
	UINT32		nTokenSize;
	UINT64		nFileSize;
	char		acEncHash[MAX_QBUFF];
	char		acRelativePath[MAX_QBUFF];
	char		acFileName[MAX_QBUFF];
}ASI_COMP_SECU_FILE_HDR, *PASI_COMP_SECU_FILE_HDR;
//-----------------------------------------------------------------------------------

typedef struct _asi_comp_secu_loop_hdr
{
	_asi_comp_secu_loop_hdr()
	{
		nSrcSize		= 0;
		nTarSize		= 0;
	}
	UINT32		nSrcSize;	
	UINT32		nTarSize;
}ASI_COMP_SECU_LOOP_HDR, *PASI_COMP_SECU_LOOP_HDR;
//-----------------------------------------------------------------------------------

#pragma pack()

#endif //COM_DEFINE_COMPRESS_H_4D39CDDB_E289_4d56_9F98_FE42776F4467



