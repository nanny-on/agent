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

#ifndef MemTokenH
#define MemTokenH

//---------------------------------------------------------------------------

class MemToken
{
private :
    PBYTE	m_pbBuf;
    INT32	m_nPos;
    INT32	m_nBufLen;
    BOOL	m_bAllocMem;

    BOOL Avaiable(INT32 nLen) ;

public :
    MemToken() ;
    MemToken(INT32 aInitialSize ) ;    
    MemToken(PVOID pSrc, INT32 nLen) ;
    ~MemToken() ;
                       
    BOOL IsMoreToken();

    void Set(PVOID pSrc, INT32 nLen) ;
	void Set(INT32 aInitialSize) ;
	void Release();
	void Copy(PVOID pSrc, INT32 nLen) ;
	void SetCopy(PVOID pSrc, INT32 nLen) ;
	BOOL _TokenAdd(PVOID pSrc, INT32 nLen) ;
	BOOL _TokenDel(void *aDest, INT32 nLen) ;
	BOOL _TokenAddType(UINT8 nType);
	BOOL _TokenDelType(UINT8& nType);
	BOOL _IsTokenType(UINT8 nType);

	BOOL TokenSet_Block();
	BOOL TokenSkip_Block();
	
	BOOL TokenAdd_8(INT8 pSrc) ;
	BOOL TokenAdd_16(INT16 pSrc) ;
	BOOL TokenAdd_32(INT32 pSrc) ;
	BOOL TokenAdd_64(INT64 pSrc) ;
	BOOL TokenAdd_String(String pSrc);
	BOOL TokenAdd_StringW(StringW pSrc);
	BOOL TokenAdd_CString(CString strSrc);
	BOOL TokenAdd_PVOID(PVOID pSrc, INT32 nLen);
	BOOL TokenAdd_IDList(TListID& tIDList);
	BOOL TokenAdd_IDMap(TMapID& tIDMap);
	BOOL TokenAdd_ID64Map(TMapID64& tID64Map);
	BOOL TokenAdd_IDMapList(TMapIDList& tIDMapList);
	BOOL TokenAdd_IDMapStr(TMapIDStr& tIDStrMap);
	BOOL TokenAdd_StringList(TListStr& tStrList);
	
	BOOL TokenDel_8(INT8 &aDest) ;
	BOOL TokenDel_16(INT16 &aDest) ;
    BOOL TokenDel_32(INT32 &aDest) ;
    BOOL TokenDel_64(INT64 &aDest) ;
	BOOL TokenDel_8(UINT8 &aDest) ;
	BOOL TokenDel_16(UINT16 &aDest) ;
    BOOL TokenDel_32(UINT32 &aDest) ;
	BOOL TokenDel_64(UINT64 &aDest) ;
	INT32 Conver_Euckr(char *pSrcData, int nSrcLen, char **ppDstData);
	INT32 Conver_Utf8(char *pSrcData, int nSrcLen, char **ppDstData);
	INT32 TokenDel_String(String& pSrc);
	INT32 TokenDel_StringW(StringW& pSrc);
	INT32 TokenDel_CString(CString& strSrc);
	INT32 TokenDel_char(LPSTR pSrc, INT32 nLen);
	INT32 TokenDel_PVOID(PVOID* ppSrc, PUINT32 pLen = NULL);
	INT32 TokenDel_IDList(TListID& tIDList);
	INT32 TokenDel_IDMap(TMapID& tIDMap);
	INT32 TokenDel_ID64Map(TMapID64& tID64Map);
	INT32 TokenDel_IDMapList(TMapIDList& tIDMapList);
	INT32 TokenDel_IDMapStr(TMapIDStr&	tIDStrMap);
	INT32 TokenDel_StrMap(TMapStr& tStrMap);
	INT32 TokenDel_StrList(TListStr& tStrList);

public:
	BOOL	TokenAdd_DPH(DB_PO_HEADER& tDPH);
	INT32	TokenDel_DPH(DB_PO_HEADER& tDPH);

public:	
    PVOID   GetData() ;
	INT32   GetLength() ;
	PVOID   GetRData();
	INT32   GetRLength();
	void	Clear();    
} ;

MemToken&	EmptyToken(MemToken& Token);

#endif
