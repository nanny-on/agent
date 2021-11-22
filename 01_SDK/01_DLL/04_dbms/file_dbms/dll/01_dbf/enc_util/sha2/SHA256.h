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

#ifndef _SHA256_H_
#define _SHA256_H_

typedef struct SHA256Context
{
     unsigned Message_Digest[8]; /* Message Digest (output) * */

     unsigned Length_Low; /* Message length in bits * */
     unsigned Length_High; /* Message length in bits * */

     unsigned char Message_Block[64]; /* 512-bit message blocks * */
     int Message_Block_Index; /* Index into message block array * */

     int Computed; /* Is the digest computed? * */
     int Corrupted; /* Is the message digest corruped? * */
} SHA256Context;


#endif
