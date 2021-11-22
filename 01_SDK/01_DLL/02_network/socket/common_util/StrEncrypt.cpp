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
#include "stdafx.h"
#include "com_struct.h"
#include "StrEncrypt.h"
//---------------------------------------------------------------------------


CStrEncrypt::CStrEncrypt()
{
}
//---------------------------------------------------------------------------

CStrEncrypt::~CStrEncrypt()
{

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//- Packet Encrypt/Decrypt --------------------------------------------------
//---------------------------------------------------------------------------
void CStrEncrypt::ScrembleEncrypt(unsigned long key,
                               unsigned char *SrcData,
                               int SrcLen)
{
    int count = SrcLen / 4,
        remainCount = SrcLen % 4;
    int i ;
    unsigned char t ;
    unsigned long data ;
    unsigned long *p    = (unsigned long *)SrcData ;
    unsigned char *p2   = (unsigned char *)&key ;

	key = htonl(key);

    for( i = 0; i < count; i++ )
    {
        data = htonl((p[i] ^ key)) ;
        p[i] = htonl(((data >> 24L) & 0x000000FF)
	         | ((data << 8L)  & 0xFF000000)
	         | ((data << 8L)  & 0x00FF0000)
	         | ((data << 8L)  & 0x0000FF00));
    }
    for( i = 0; i < count; i++ )
    {
        data = htonl((p[i] ^ key)) ;
        p[i] = htonl(((data >> 16L) & 0x000000FF)
	         | ((data >> 16L) & 0x0000FF00)
	         | ((data << 16L) & 0x00FF0000)
	         | ((data << 16L) & 0xFF000000));
    }
    SrcData += 4*count ;

    for( i = 0; i < remainCount/2; i++ )
    {
        t = SrcData[i] ;

        SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
        SrcData[remainCount-i-1] = t ^ p2[i] ;
    }
    if( remainCount%2 != 0 )
    {
        SrcData[i] = SrcData[i] ^ p2[i] ;
    }
}
//---------------------------------------------------------------------------

void CStrEncrypt::ScrembleDecrypt(unsigned long key,
                               unsigned char *SrcData,
                               int SrcLen)
{
    int count = SrcLen / 4,
        remainCount = SrcLen % 4;
    int i ;
    unsigned char t ;
    unsigned long data ;
    unsigned long *p    = (unsigned long *)SrcData ;
    unsigned char *p2   = (unsigned char *)&key ;

	key = htonl(key);

    for( i = 0; i < count; i++ )
    {
        data = htonl(p[i]) ;
        p[i] = ((data << 16L) & 0xFF000000)
	         | ((data << 16L) & 0x00FF0000)
	         | ((data >> 16L) & 0x0000FF00)
	         | ((data >> 16L) & 0x000000FF);
		p[i] = htonl(p[i]);
        p[i] = (p[i] ^ key) ;
    }
    for( i = 0; i < count; i++ )
    {
        data = htonl(p[i]) ;
        p[i] = ((data << 24L) & 0xFF000000)
	         | ((data >> 8L)  & 0x00FF0000)
	         | ((data >> 8L)  & 0x0000FF00)
	         | ((data >> 8L)  & 0x000000FF) ;
		p[i] = htonl(p[i]);
        p[i] = (p[i] ^ key) ;
    }
    SrcData += 4*count ;

    for( i = 0; i < remainCount/2; i++ )
    {
        t = SrcData[i] ;
        
        SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
        SrcData[remainCount-i-1] = t ^ p2[i] ;
    }
    if( remainCount%2 != 0 )
    {
        SrcData[i] = SrcData[i] ^ p2[i] ;        
    }
}
//---------------------------------------------------------------------------

void  CStrEncrypt::ScrembleEncrypt_Win(unsigned long key, unsigned char *SrcData, int SrcLen)
{
    int count = SrcLen / 4,
        remainCount = SrcLen % 4;
    int i ;
    unsigned char t ;
    unsigned long data ;
    unsigned long *p    = (unsigned long *)SrcData ;
    unsigned char *p2   = (unsigned char *)&key ;
    for( i = 0; i < count; i++ )
    {
        data = (p[i] ^ key) ;
        p[i] = ((data >> 24L) & 0x000000FF)
	         | ((data << 8L)  & 0xFF000000)
	         | ((data << 8L)  & 0x00FF0000)
	         | ((data << 8L)  & 0x0000FF00);
    }
    for( i = 0; i < count; i++ )
    {
        data = (p[i] ^ key) ;
        p[i] = ((data >> 16L) & 0x000000FF)
	         | ((data >> 16L) & 0x0000FF00)
	         | ((data << 16L) & 0x00FF0000)
	         | ((data << 16L) & 0xFF000000);
    }
    SrcData += 4*count ;

    for( i = 0; i < remainCount/2; i++ )
    {
        t = SrcData[i] ;

        SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
        SrcData[remainCount-i-1] = t ^ p2[i] ;
    }
    if( remainCount%2 != 0 )
    {
        SrcData[i] = SrcData[i] ^ p2[i] ;
    }
}
//---------------------------------------------------------------------------

void	CStrEncrypt::ScrembleDecrypt_Win(unsigned long key, unsigned char *SrcData, int SrcLen)
{
    int count = SrcLen / 4,
        remainCount = SrcLen % 4;
    int i ;
    unsigned char t ;
    unsigned long data ;
    unsigned long *p    = (unsigned long *)SrcData ;
    unsigned char *p2   = (unsigned char *)&key ;
    for( i = 0; i < count; i++ )
    {
        data = p[i] ;
        p[i] = ((data << 16L) & 0xFF000000)
	         | ((data << 16L) & 0x00FF0000)
	         | ((data >> 16L) & 0x0000FF00)
	         | ((data >> 16L) & 0x000000FF);
        p[i] = (p[i] ^ key) ;
    }
    for( i = 0; i < count; i++ )
    {
        data = p[i] ;
        p[i] = ((data << 24L) & 0xFF000000)
	         | ((data >> 8L)  & 0x00FF0000)
	         | ((data >> 8L)  & 0x0000FF00)
	         | ((data >> 8L)  & 0x000000FF);
        p[i] = (p[i] ^ key) ;
    }
    SrcData += 4*count ;

    for( i = 0; i < remainCount/2; i++ )
    {
        t = SrcData[i] ;
        
        SrcData[i] = SrcData[remainCount-i-1] ^ p2[i] ;
        SrcData[remainCount-i-1] = t ^ p2[i] ;
    }
    if( remainCount%2 != 0 )
    {
        SrcData[i] = SrcData[i] ^ p2[i] ;        
    }
}
//---------------------------------------------------------------------------
