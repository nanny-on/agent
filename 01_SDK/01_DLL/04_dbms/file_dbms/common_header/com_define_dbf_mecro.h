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

#ifndef COM_DEFINE_DBF_MECRO_H_938EF200_F60B_452A_BD30_A10E8507EDCC
#define COM_DEFINE_DBF_MECRO_H_938EF200_F60B_452A_BD30_A10E8507EDCC

/*
#define MAKEUINT64(a, b)      	((UINT64) (((UINT32) (a)) | ((UINT64) ((UINT32) (b))) << 32))

#ifndef LOUINT32
#define LOUINT32(l)           	((UINT32) (l))
#endif
#ifndef HIUINT32
#define HIUINT32(l)           	((UINT32) (((UINT64) (l) >> 32) & 0xFFFFFFFF))
#endif
*/

#define LOLONG(l)           	((LONG) (l))
#define HILONG(l)           	((LONG) (((UINT64) (l) >> 32) & 0xFFFFFFFF))


#endif	//COM_DEFINE_DBF_MECRO_H_938EF200_F60B_452A_BD30_A10E8507EDCC
