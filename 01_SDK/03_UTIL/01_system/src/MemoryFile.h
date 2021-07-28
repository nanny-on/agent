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

//-----------------------------------------------------------
//
// Project: Filseclab Personal Firewall
// Author:	Bright Chu
// Website: http://www.filseclab.com
// E-mail:	software@filseclab.com
//
// Copyright (c) 2002-2004 Filseclab.
//
// Warnning:
//
// If you did not pay the license fee for this source code to
// Filseclab, please you close this file and destory all of the 
// source code and documents immediately. Otherwise, you will
// be tortious. If you want to get license, please you visit
// website at http://www.filseclab.com to purchase it. 
//
// You must be read the license before you may use the source 
// code and documents of Filseclab Personal Firewall. If you 
// have not did it, please open the license.htm file and read 
// it carefully at first.
//
// Unauthorized reproduction or distribution of this source 
// code, or any portion of it, may result in severe civil and 
// criminal penalties, and will be prosecuted to the maximum 
// extent possible under the law.
//
//
//

#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H

class CMemoryFile
{
public:
	CMemoryFile();
	virtual ~CMemoryFile();

private:
	HANDLE	m_hMemFile;
	PVOID	m_pMapFile;
	PVOID	m_ST;

private:
	UINT32		m_nUsedMutex;
	CMutexExt	m_tMutex;

private:
	PVOID	GetSecurityAttributes();

public:
	INT32	CreateMemFile(String strFileName, UINT32 nFileSize);
	INT32	OpenMemFile(String strFileName);
	INT32	CloseMemFile();

	PVOID	MapViewFile(UINT32 nAccess = 0);	
	INT32	UnmapViewFile();

public:
	INT32	SetUsedMutex(UINT32 nMode = 1);
};

#endif //MOMORY_FILE_H
