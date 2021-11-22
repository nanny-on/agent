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

#ifndef _FILE_DELETE_UTIL_H__

#define _FILE_DELETE_UTIL_H__


#include "SecureDeleteUtil.h"

#define SECURE_DEL_BUF_SIZE (1024)

class CFileDeleteUtil : public CUtilBase
{
public:
	CFileDeleteUtil(void);
	~CFileDeleteUtil(void);

public:
	void		SetDeleteMethod(UINT32 nMethod=0);
	INT32 		SecureDeleteFile(LPCSTR szPath, UINT32 nDelMethod, DWORD dwOverwriteCount=3, BOOL bReadOnlyDelete=TRUE, BOOL bWait=TRUE, UINT32 nLimitMByteByte=0, UINT32 nLimitDelCnt=0);

private:
	INT32 		ZeroFile(LPCSTR szPath, DWORD dwOverwriteCount);

	UINT32		m_nDelMethod;
	CSecureDeleteUtil*		t_WipeUtil;
};

extern CFileDeleteUtil*		t_FileDeleteUtil;

#endif /*_FILE_DELETE_UTIL_H__*/