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

#ifndef _AS_SEED_H_
#define _AS_SEED_H_


extern void seed_init(char *key, int key_len);
extern int seed_encrypt_len(int pdata_len);
extern int seed_encrypt_data(char *pdata, int pdata_len, char **cdata, int *cdata_len);
extern int seed_decrypt_data(char *cdata, char **pdata, int *pdata_len);


#endif /* _AS_SEED_H_ */


