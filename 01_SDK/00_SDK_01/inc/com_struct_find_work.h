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



#ifndef COM_STRUCT_FIND_WORK_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_STRUCT_FIND_WORK_H_938EF200_F60B_452A_BD30_A10E8507EDCC

//------------------------------------------------------------------------------

#define		FIND_FILE_SEARCH_STEP_INIT			0
#define		FIND_FILE_SEARCH_STEP_PRE_DIR		1
#define		FIND_FILE_SEARCH_STEP_SET_TDIR		2
//------------------------------------------------------------------------------

typedef struct _find_dir_item
{
	_find_dir_item()
	{
		nOrderID	= 0;
		nSubSearch	= 0;
	}
	UINT32		nOrderID;
	String		strSearchDir;
	StringW		strSearchDirW;
	UINT32		nSubSearch;
}FIND_DIR_ITEM, *PFIND_DIR_ITEM;

typedef list<FIND_DIR_ITEM>				TListFindDirItem;
typedef TListFindDirItem::iterator		TListFindDirItemItor;

//------------------------------------------------------------------------------

typedef struct _find_file_item
{
	_find_file_item()
	{
		nFileSize	= 0;
		nFindType	= 0;
	}
	String		strFilePath;
	String		strFileName;
	StringW		strFilePathW;
	StringW		strFileNameW;
	UINT64		nFileSize;
	UINT32		nFindType;
}FIND_FILE_ITEM, *PFIND_FILE_ITEM;

typedef list<FIND_FILE_ITEM>			TListFindFileItem;
typedef TListFindFileItem::iterator		TListFindFileItemItor;

//------------------------------------------------------------------------------

typedef struct _find_file_work
{
	_find_file_work()
	{
		nOrderID			= 0;
		nSearchStep			= 0;
		nSearchPathNum		= 0;
		nSearchedPathNum	= 0;
		nDirSubSearchNum	= 0;
		nDirSubSearchedNum	= 0;
		nDirOldSearchedNum	= 0;
		nSameSearcheCnt		= 0;
		nDirTotalNum		= 0;
		nDirSearchedNum		= 0;
		nFileTotalNum		= 0;
		nFileWorkedNum		= 0;

		tMutexExt			= NULL;
		nContinue			= 0;
	}
	UINT32		nOrderID;
	UINT32		nSearchStep;
	UINT32		nSearchPathNum;
	UINT32		nSearchedPathNum;
	UINT32		nDirSubSearchNum;
	UINT32		nDirOldSearchedNum;
	UINT32		nSameSearcheCnt;
	UINT32		nDirSubSearchedNum;
	UINT32		nDirTotalNum;
	UINT32		nDirSearchedNum;
	UINT32		nFileTotalNum;
	UINT32		nFileWorkedNum;
	PVOID		tMutexExt;

	TListFindFileItem	tFFIList;
	UINT32		nContinue;
}FIND_FILE_WORK, *PFIND_FILE_WORK;

typedef map<UINT32, FIND_FILE_WORK>		TMapFindFileWork;
typedef TMapFindFileWork::iterator		TMapFindFileWorkItor;
//------------------------------------------------------------------------------


#endif      //COM_STRUCT_FIND_WORK_H_938EF200_F60B_452A_BD30_A10E8507EDCC
