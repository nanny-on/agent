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

//---------------------------------------------------------------------------

#ifndef _FILE_TOKEN_H__

#define _FILE_TOKEN_H__
//---------------------------------------------------------------------------

class FileToken
{
private :
    INT32	m_nPos;
    INT32	m_nFileLen;
	INT32	m_nFd;
	INT32	m_nOpenFile;

public :
    FileToken();
    FileToken(String strFileName);
	FileToken(INT32 nFd);
    ~FileToken();
                       
    BOOL IsMoreToken();

    BOOL _TokenAdd(void *aSrc, INT32 aLen);
    BOOL _TokenDel(void *aDest, INT32 aLen);

	BOOL TokenAdd_16(INT16 aSrc);
    BOOL TokenAdd_32(INT32 aSrc);
	BOOL TokenAdd_64(INT64 aSrc);
	BOOL TokenAdd_Char(char *acData);
	BOOL TokenAdd_String(String aSrc);
	BOOL TokenAdd_StringW(StringW aSrc);
	BOOL TokenAdd_PVOID(PVOID aSrc, INT32 nLen);
    
	BOOL TokenDel_16(INT16 &aDest);
    BOOL TokenDel_32(INT32 &aDest);
    BOOL TokenDel_64(INT64 &aDest);
	BOOL TokenDel_16(UINT16 &aDest);
    BOOL TokenDel_32(UINT32 &aDest);
	BOOL TokenDel_64(UINT64 &aDest);
	INT16 TokenDel_String(String& aSrc);
	INT16 TokenDel_StringW(StringW& aSrc);
	INT16 TokenDel_Char(char *acData, INT16 nMaxLen);
    INT32 TokenDel_PVOID(PVOID aSrc, INT32 nSrcLen);
    INT32 GetFD();
    INT32 GetLength(); 
	void Clear();
   
} ;

#endif /*_FILE_TOKEN_H__*/