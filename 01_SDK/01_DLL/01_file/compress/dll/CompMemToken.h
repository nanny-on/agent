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


#ifndef _COMP_MEM_TOKEN_H__
#define _COMP_MEM_TOKEN_H__

//---------------------------------------------------------------------------

class CompMemToken
{
private :
    BYTE  *m_pBuf;
    INT32 m_nPos;
    INT32 m_nBufLen;
    BOOL m_bAutoMem;
    BOOL m_bDataReceived;

    BOOL Avaiable(INT32 aLen) ;

public :
    CompMemToken() ;
    CompMemToken(INT32 aInitialSize ) ;    
    CompMemToken(void *aSrc, INT32 aLen) ;
    ~CompMemToken() ;
                       
    BOOL IsMoreToken();

    void Set(void *aSrc, INT32 aLen) ;
    void Set(INT32 aInitialSize) ;
    BOOL _TokenAdd(void *aSrc, INT32 aLen) ;
    BOOL _TokenDel(void *aDest, INT32 aLen) ;

	BOOL TokenAdd_16(INT16 aSrc) ;
    BOOL TokenAdd_32(INT32 aSrc) ;
	BOOL TokenAdd_64(INT64 aSrc) ;
	BOOL TokenAdd_String(String aSrc);
	BOOL TokenAdd_StringW(StringW aSrc);
	BOOL TokenAdd_PVOID(PVOID aSrc, INT32 nLen);
    
	BOOL TokenDel_16(INT16 &aDest) ;
    BOOL TokenDel_32(INT32 &aDest) ;
    BOOL TokenDel_64(INT64 &aDest) ;
	BOOL TokenDel_16(UINT16 &aDest) ;
    BOOL TokenDel_32(UINT32 &aDest) ;
    BOOL TokenDel_64(UINT64 &aDest) ;
	INT16 TokenDel_String(String& aSrc);
	INT16 TokenDel_StringW(StringW& aSrc);
	INT32 TokenDel_PVOID(PVOID aSrc, INT32 nLen);

    PVOID   GetData() ;
    INT32   GetLength() ; 
    PVOID   GetRData();
    INT32   GetRLength();

public:
	void	Clear();
    
} ;

#endif /*_COMP_MEM_TOKEN_H__*/