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

#ifndef	__ARIAEX_H_
#define __ARIAEX_H_

#define	ARIA_HEADER			"ASI!ARIA"

#define SIZE_HEADER			sizeof(AINFO)
#define	SIZE_ARIA			8
#define	SIZE_BLOCK			16
#define	SIZE_USERKEY		32
#define	SIZE_KRYBITS		256

typedef struct ARIA_INFO
{
	char	szHeader[SIZE_ARIA];
	DWORD	dwCRC32;
	int		nSize;
}
AINFO, *PAINFO;

INT32 AriaEnCryptFile(char *szukey, char *szifile, char *szofile, char *acLogMsg);
INT32 AriaDeCryptFile(char *szukey, char *szifile, char *szofile, char *acLogMsg);

INT32 AriaEnCryptBuffer(char *szukey, char *szidata, int ilen, char *szodata, int *olen, char *acLogMsg);
INT32 AriaDeCryptBuffer(char *szukey, char *szidata, char *szodata, int *olen, char *acLogMsg);

#endif