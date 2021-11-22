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
 */

#ifndef __CRC32_H__
#define __CRC32_H__

#ifndef __CRC32_CPP__
#define __CRC32_CPP__
#endif

typedef struct {
  unsigned long ul;
  unsigned char ch4;
  unsigned char ch3;
  unsigned char ch2;
  unsigned char ch1;
} UL_UC_CVRT;

typedef struct {
  unsigned long crc32;
} CRC32_STRUCT;

#define UPDC32(c, crc) (crc32tab[((int)(crc) ^ (c)) & 0xff] ^ (((crc) >> 8) & 0x00FFFFFFL))

INT32 getCRC32(DWORD *CRC32, char *filename);
BOOL getCRC32Buffer(DWORD *CRC32, char *Buffer, INT32 len);
BOOL getCRC32Old(DWORD *CRC32, char *Buffer, INT32 len);


#endif /* __CRC32_H__ */
